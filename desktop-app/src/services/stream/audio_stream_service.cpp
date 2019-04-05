
#include "audio_stream_service.h"

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

    inputParameters.device =
        Pa_GetDefaultInputDevice(); /* default input device */
    printf("Input device # %d.\n", inputParameters.device);
    printf("Input LL: %g s\n",
           Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency);
    printf("Input HL: %g s\n",
           Pa_GetDeviceInfo(inputParameters.device)->defaultHighInputLatency);
    inputParameters.channelCount = NUM_CHANNELS;
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency =
        Pa_GetDeviceInfo(inputParameters.device)->defaultHighInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    outputParameters.device =
        Pa_GetDefaultOutputDevice(); /* default output device */
    printf("Output device # %d.\n", outputParameters.device);
    printf("Output LL: %g s\n",
           Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency);
    printf("Output HL: %g s\n",
           Pa_GetDeviceInfo(outputParameters.device)->defaultHighOutputLatency);
    outputParameters.channelCount = NUM_CHANNELS;
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency =
        Pa_GetDeviceInfo(outputParameters.device)->defaultHighOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(&in_stream, &inputParameters, NULL, SAMPLE_RATE,
                        FRAMES_PER_BUFFER,
                        paClipOff, /* we won't output out of range samples so
                                      don't bother clipping them */
                        NULL,      /* no callback, use blocking API */
                        NULL);     /* no callback, so no callback userData */
    if (err != paNoError) {
        Pa_onError();
    }

    err = Pa_OpenStream(&out_stream, NULL, &outputParameters, SAMPLE_RATE,
                        FRAMES_PER_BUFFER,
                        paClipOff, /* we won't output out of range samples so
                                      don't bother clipping them */
                        NULL,      /* no callback, use blocking API */
                        NULL);     /* no callback, so no callback userData */
    if (err != paNoError) {
        Pa_onError();
    }

    err = Pa_StartStream(in_stream);
    if (err != paNoError) {
        Pa_onError();
    }

    err = Pa_StartStream(out_stream);
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

            } else {  // Stop streaming

                // Destroy socket connection to the server
                socket.destroy();
            }

            audio_service.streaming = new_streaming_status;
        }

        if (new_streaming_status) {
            audio_service.err =
                Pa_WriteStream(audio_service.in_stream,
                               audio_service.in_sampleBlock, FRAMES_PER_BUFFER);
            if (audio_service.err && CHECK_UNDERFLOW) {
                audio_service.Pa_onxRun();
            }

            std::vector<unsigned char> data(
                audio_service.out_sampleBlock,
                audio_service.out_sampleBlock + FRAMES_PER_BUFFER);
            socket.sendPacket(protocol_data.packageClientFrame(data));
        }
    }

}

void AudioStreamService::downStreamingThread() {

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
        AudioStreamService &streaming_service = AudioStreamService::instance();
        bool streaming = streaming_service.isStreaming();
        bool new_streaming_status = streaming_service.streaming_signal_flag;
        if (new_streaming_status != streaming) {
            if (new_streaming_status == true) {  // Start streaming

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

			if (payload.empty()) continue;

			audio_service.err =
			Pa_ReadStream(audio_service.out_stream,
							payload.data(), FRAMES_PER_BUFFER);
            if (audio_service.err && CHECK_UNDERFLOW) {
                audio_service.Pa_onxRun();
            }

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