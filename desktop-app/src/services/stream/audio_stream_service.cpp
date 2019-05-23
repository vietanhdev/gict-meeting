
#include "audio_stream_service.h"


/* const int AudioStreamService::SAMPLE_RATE  = 17932; // Test failure to open with this value. */
const int AudioStreamService::SAMPLE_RATE  = 8000;
const int AudioStreamService::FRAMES_PER_BUFFER = 44;
const int AudioStreamService::NUM_CHANNELS    = 1;
// const int AudioStreamService::DITHER_FLAG  = paDitherOff;
const int AudioStreamService::DITHER_FLAG     = 1; /**/

/* @todo Underflow and overflow is disabled until we fix priming of blocking write. */
const int AudioStreamService::CHECK_OVERFLOW  = 0;
const int AudioStreamService::CHECK_UNDERFLOW  = 0;

const int AudioStreamService::SAMPLE_SIZE = 1;
const int AudioStreamService::SAMPLE_SILENCE  = 128;

AudioStreamService::AudioStreamService() {
    std::cout << "Initializing Audio Service" << std::endl;

    // ALLOCATING MEMORY

    // Allocating sample input block
    numBytes = FRAMES_PER_BUFFER * NUM_CHANNELS * SAMPLE_SIZE;
    in_sampleBlock = (char *)malloc(numBytes);
    if (in_sampleBlock == NULL) {
        printf("Could not allocate record array.\n");
        exit(1);
    }

    // Clear sample input block
    for (int i = 0; i < FRAMES_PER_BUFFER * NUM_CHANNELS; i++) {
        ((unsigned char *)in_sampleBlock)[i] = (SAMPLE_SILENCE);
    }

    // Allocating sample output block
    numBytes = FRAMES_PER_BUFFER * NUM_CHANNELS * SAMPLE_SIZE;
    out_sampleBlock = (char *)malloc(numBytes);
    if (out_sampleBlock == NULL) {
        printf("Could not allocate record array.\n");
        exit(1);
    }

    // Clear sample output block
    for (int i = 0; i < FRAMES_PER_BUFFER * NUM_CHANNELS; i++) {
        ((unsigned char *)out_sampleBlock)[i] = (SAMPLE_SILENCE);
    }

    err = Pa_Initialize();
    if (err != paNoError) {
        Pa_onError();
    }

}

AudioStreamService::~AudioStreamService() {
    AudioStreamService &audio_service = AudioStreamService::instance();
    audio_service.err = Pa_StopStream(audio_service.in_stream);
    if (audio_service.err != paNoError) {
        audio_service.Pa_onError();
    }
    audio_service.err = Pa_StopStream(audio_service.out_stream);
    if (audio_service.err != paNoError) {
        audio_service.Pa_onError();
    }
    free(in_sampleBlock);
    free(out_sampleBlock);
    Pa_Terminate();
}

void AudioStreamService::Pa_onError() {
    if (in_stream) {
        Pa_AbortStream(in_stream);
        Pa_CloseStream(in_stream);
    }
    free(in_sampleBlock);
    if (out_stream) {
        Pa_AbortStream(out_stream);
        Pa_CloseStream(out_stream);
    }
    free(out_sampleBlock);
    Pa_Terminate();
    fprintf(stderr, "An error occured while using the portaudio stream\n");
    fprintf(stderr, "Error number: %d\n", err);
    fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
}

void AudioStreamService::Pa_onxRun() {
    if (in_stream) {
        Pa_AbortStream(in_stream);
        Pa_CloseStream(in_stream);
    }
    free(in_sampleBlock);
    if (out_stream) {
        Pa_AbortStream(out_stream);
        Pa_CloseStream(out_stream);
    }
    free(out_sampleBlock);
    Pa_Terminate();
    if (err & paInputOverflow) fprintf(stderr, "Input Overflow.\n");
    if (err & paOutputUnderflow) fprintf(stderr, "Output Underflow.\n");
}

void AudioStreamService::upStreamingThread() {
    ClientSocket socket;

    // Get up port
    int port = Conference::instance().getAudioUpPort();
    // Server ip addresss
    std::string ip_address = Conference::instance().getServerIp();
    // Init a socket to the server
    socket.init(ip_address, port);

    AudioFrameProtocolData protocol_data;

    AudioStreamService &audio_service = AudioStreamService::instance();

    for (;;) {
        AudioStreamService &audio_service = AudioStreamService::instance();
        bool streaming = audio_service.isStreaming();
        bool new_streaming_status = audio_service.streaming_signal_flag;
        if (new_streaming_status != streaming) {
            if (new_streaming_status == true) {  // Start streaming

                // Get up port
                int port = Conference::instance().getAudioUpPort();

                // Server ip addresss
                std::string ip_address = Conference::instance().getServerIp();

                // Init a socket to the server
                socket.init(ip_address, port);


                audio_service.inputParameters.device =
                    Pa_GetDefaultInputDevice(); /* default input device */
                printf("Input device # %d.\n", audio_service.inputParameters.device);
                printf("Input LL: %g s\n",
                    Pa_GetDeviceInfo(audio_service.inputParameters.device)->defaultLowInputLatency);
                printf("Input HL: %g s\n",
                    Pa_GetDeviceInfo(audio_service.inputParameters.device)->defaultHighInputLatency);
                audio_service.inputParameters.channelCount = audio_service.NUM_CHANNELS;
                audio_service.inputParameters.sampleFormat = PA_SAMPLE_TYPE;
                audio_service.inputParameters.suggestedLatency =
                    Pa_GetDeviceInfo(audio_service.inputParameters.device)->defaultHighInputLatency;
                audio_service.inputParameters.hostApiSpecificStreamInfo = NULL;


                audio_service.err = Pa_OpenStream(&audio_service.in_stream, &audio_service.inputParameters, NULL, audio_service.SAMPLE_RATE,
                    audio_service.FRAMES_PER_BUFFER,
                    paClipOff, /* we won't output out of range samples so
                                don't bother clipping them */
                    NULL,      /* no callback, use blocking API */
                    NULL);     /* no callback, so no callback userData */
                if (audio_service.err != paNoError) {
                    audio_service.Pa_onError();
                }
                

                audio_service.err = Pa_StartStream(audio_service.in_stream);
                if (audio_service.err != paNoError) {
                    audio_service.Pa_onError();
                }


            } else {  // Stop streaming

                // Destroy socket connection to the server
                socket.destroy();

                audio_service.err = Pa_StopStream(audio_service.in_stream);
                if (audio_service.err != paNoError) {
                    audio_service.Pa_onError();
                }
            }

            audio_service.streaming = new_streaming_status;
        }

        if (new_streaming_status) {
            audio_service.err =
                Pa_ReadStream(audio_service.in_stream,
                               audio_service.in_sampleBlock, FRAMES_PER_BUFFER);
            if (audio_service.err && CHECK_UNDERFLOW) {
                audio_service.Pa_onxRun();
            }


            std::vector<unsigned char> data(
                audio_service.in_sampleBlock,
                audio_service.in_sampleBlock + FRAMES_PER_BUFFER);
            socket.sendPacket(protocol_data.packageClientFrame(data));

			// std::cout << "Sent size = " << data.size() << std::endl;
        }
    }

}

void AudioStreamService::downStreamingThread() {

    ClientSocket socket;
    AudioFrameProtocolData protocol_data;
    AudioStreamService &audio_service = AudioStreamService::instance();

    // Get up port
    int port = Conference::instance().getAudioDownPort();
    // Server ip addresss
    std::string ip_address = Conference::instance().getServerIp();
    // Init a socket to the server
    socket.init(ip_address, port);

    // Send stream request message
    socket.sendPacket(protocol_data.packageStreamRequestMessage());
    

    for (;;) {
        AudioStreamService &streaming_service = AudioStreamService::instance();
        bool streaming = streaming_service.isStreaming();
        bool new_streaming_status = streaming_service.streaming_signal_flag;
        if (new_streaming_status != streaming) {
            if (new_streaming_status == true) {  // Start streaming


                // Init audio stream
                streaming_service.outputParameters.device =
                    Pa_GetDefaultOutputDevice(); /* default output device */
                printf("Output device # %d.\n", streaming_service.outputParameters.device);
                printf("Output LL: %g s\n",
                    Pa_GetDeviceInfo(streaming_service.outputParameters.device)->defaultLowOutputLatency);
                printf("Output HL: %g s\n",
                    Pa_GetDeviceInfo(streaming_service.outputParameters.device)->defaultHighOutputLatency);
                streaming_service.outputParameters.channelCount = streaming_service.NUM_CHANNELS;
                streaming_service.outputParameters.sampleFormat = PA_SAMPLE_TYPE;
                streaming_service.outputParameters.suggestedLatency =
                    Pa_GetDeviceInfo(streaming_service.outputParameters.device)->defaultHighOutputLatency;
                streaming_service.outputParameters.hostApiSpecificStreamInfo = NULL;


                streaming_service.err = Pa_OpenStream(&streaming_service.out_stream, NULL, &streaming_service.outputParameters, streaming_service.SAMPLE_RATE,
                                    streaming_service.FRAMES_PER_BUFFER,
                                    paClipOff, /* we won't output out of range samples so
                                                don't bother clipping them */
                                    NULL,      /* no callback, use blocking API */
                                    NULL);     /* no callback, so no callback userData */
                if (streaming_service.err != paNoError) {
                    streaming_service.Pa_onError();
                }

                streaming_service.err = Pa_StartStream(streaming_service.out_stream);
                if (streaming_service.err != paNoError) {
                    streaming_service.Pa_onError();
                }

                // Get up port
                int port = Conference::instance().getAudioDownPort();

                // Server ip addresss
                std::string ip_address = Conference::instance().getServerIp();

                // Init a socket to the server
                socket.init(ip_address, port);

                // Send stream request message
                socket.sendPacket(protocol_data.packageStreamRequestMessage());

                std::cout << "Requesting audio streaming from server"
                          << std::endl;
                          

            } else {  // Stop streaming

                // Destroy socket connection to the server
                socket.destroy();

                audio_service.err = Pa_StopStream(audio_service.out_stream);
                if (audio_service.err != paNoError) {
                    audio_service.Pa_onError();
                }
            }

            streaming_service.streaming = new_streaming_status;
        }

        // Streaming
        if (new_streaming_status) {

            std::vector<unsigned char> data = socket.getPacket();

            if (data.empty()) {
                continue;
            }

            std::vector<unsigned char> payload = protocol_data.unpackPayload(data);

            // Select audio out stream
            int clientID = (int) data[1];
            
            // if (clientID >= 4) clientID = 3;
            // else if (clientID <= 0) clientID = 0;
            // else clientID -= 1;

			if (payload.empty()) continue;


            if ( Conference::instance().getClientId() == data[1]) {
                // DO nothing!!!
            } else {
                audio_service.err =
                // Pa_WriteStream(audio_service.out_stream,
                Pa_WriteStream(audio_service.out_stream,
                                payload.data(), AudioStreamService::FRAMES_PER_BUFFER);
                if (audio_service.err && CHECK_UNDERFLOW) {
                    audio_service.Pa_onxRun();
                }
            }

			// std::cout << "Received size = " << payload.size() << std::endl;
			// std::cout << "Receive content:" << static_cast<int>(payload[30]) << std::endl;

        }
    }
}

bool AudioStreamService::isStreaming() { return this->streaming; }

void AudioStreamService::setStreaming(bool streaming) {
    this->streaming_signal_flag = streaming;
}

bool AudioStreamService::startStreaming() {
    if (AudioStreamService::instance().isStreaming()) {
        return false;
    }

    if (up_streaming_thread == nullptr) {
        up_streaming_thread = std::make_shared<std::thread>(upStreamingThread);
    }

    if (down_streaming_thread == nullptr) {
        down_streaming_thread =
            std::make_shared<std::thread>(downStreamingThread);
    }

    AudioStreamService::instance().setStreaming(true);

    return true;
}

bool AudioStreamService::stopStreaming() {
    if (!AudioStreamService::instance().isStreaming()) {
        return false;
    }

    AudioStreamService::instance().setStreaming(false);
    return true;
}
