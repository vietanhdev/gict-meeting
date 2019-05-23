#ifndef AUDIO_STREAM_SERVICE_H
#define AUDIO_STREAM_SERVICE_H


#include <QObject>
#include <string>
#include <vector>
#include <mutex>
#include <atomic>
#include <thread>
#include <memory>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "portaudio.h"

#include "audio_frame_protocol.h"
#include "client_socket.h"

#include "conference.h"


#define PA_SAMPLE_TYPE paUInt8;

class AudioStreamService: public QObject {

    Q_OBJECT


    private:
        /* const int SAMPLE_RATE  = 17932; // Test failure to open with this value. */
        const static int SAMPLE_RATE;
        const static int FRAMES_PER_BUFFER;
        const static int NUM_CHANNELS;
        const static int NUM_SECONDS;
        // const static int DITHER_FLAG  = paDitherOff;
        const static int DITHER_FLAG; /**/

        /* @todo Underflow and overflow is disabled until we fix priming of blocking write. */
        const static int CHECK_OVERFLOW;
        const static int CHECK_UNDERFLOW;

        const static int SAMPLE_SIZE;
        const static int SAMPLE_SILENCE;

        PaStreamParameters inputParameters, outputParameters;
        PaError err;
        int numBytes;
        char *in_sampleBlock;
        char *out_sampleBlock;
        PaStream *in_stream = NULL;
        PaStream * out_stream = NULL;
        PaStream * out_streams[4];

       void Pa_onError();
       void Pa_onxRun();


    public:
        static AudioStreamService& instance(){
            static AudioStreamService stream_service;
            return stream_service;
        }

    private:

        // This flag will pass the start/stop streaming to the worker threads.
        // Set this to "true" to start streaming, otherwise, set it to "false"
        std::atomic<bool> streaming_signal_flag = false; 

        std::atomic<bool> streaming = false; // Current streaming status

        std::shared_ptr<std::thread> up_streaming_thread;
        std::shared_ptr<std::thread> down_streaming_thread;

    public:
        bool isStreaming();
        void setStreaming(bool streaming);

    private:
        AudioStreamService(); // Disallow instantiation outside of the class.
        

        static void upStreamingThread();
        static void downStreamingThread();

    public:
        ~AudioStreamService();
        AudioStreamService(const AudioStreamService&) = delete;
        AudioStreamService& operator=(const AudioStreamService &) = delete;
        AudioStreamService(AudioStreamService &&) = delete;
        AudioStreamService & operator=(AudioStreamService &&) = delete;

        bool startStreaming();
        bool stopStreaming();

    signals:
        void newConferenceFrame();
        void newClientCamFrame();
};

#endif
