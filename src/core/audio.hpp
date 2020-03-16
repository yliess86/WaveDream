#pragma once

#include <soundio/soundio.h>
#include <functional>
#include <iostream>
#include <thread>
#include <cmath>

#define ERROR_MSG(msg) { std::cout << msg << std::endl; exit(1); }
#define ERROR_CODE(msg, err) { \
    std::cout << msg << soundio_strerror(err) << std::endl; exit(1); \
}

namespace wavedream {

    template<typename T>
    class Audio {
        private:
            static Audio<T> *_instance;
            Audio() {};
            ~Audio();

            std::function<T(T)> _callback;
            std::thread _thread;
            bool _running = false;

            int _device_idx;
            struct SoundIo *_soundio;
            struct SoundIoDevice *_device;
            struct SoundIoOutStream *_outstream;

            void Process(void);

        public:
            static Audio<T>* GetInstance(void);

            T Clamp(T x, T lo=-1.0, T hi=1.0) { return std::min(std::max(x, lo), hi); }
            int16_t Convert(T x) { return x * ((T) INT16_MAX - (T) INT16_MIN) / 2; }
 
            void AttachProcessCallback(std::function<T(T)> &callback) { 
                this->_callback = callback; 
            }
            std::function<T(T)> GetProcessCallback(void) { return this->_callback; }

            void Init(void);
            void Run(void);
            void Stop(void);

            static void Callback(
                struct SoundIoOutStream *outstream,
                int frame_min, 
                int frame_max
            );
    };

    template<typename T>
    Audio<T>* Audio<T>::_instance = nullptr;
    
    template<typename T>
    Audio<T>* Audio<T>::GetInstance(void) {
        if(!_instance)  _instance = new Audio<T>();
        return _instance;
    }

    template<typename T>
    Audio<T>::~Audio(void) {
        soundio_outstream_destroy(this->_outstream);
        soundio_device_unref(this->_device);
        soundio_destroy(this->_soundio);
    }

    template<typename T>
    void Audio<T>::Init(void) {
        int err;
        
        this->_soundio = soundio_create();
        if(!this->_soundio) ERROR_MSG("Out of memory.");
        if((err = soundio_connect(this->_soundio))) ERROR_CODE("Connection error: ", err);
        soundio_flush_events(this->_soundio);

        this->_device_idx = soundio_default_output_device_index(this->_soundio);
        if(this->_device_idx < 0) ERROR_MSG("Not output device found.");
        
        this->_device = soundio_get_output_device(this->_soundio, this->_device_idx);
        if(!this->_device) ERROR_MSG("Out of memory.");
        std::cout << "Output device: " << this->_device->name << std::endl;
        
        this->_outstream = soundio_outstream_create(this->_device);
        if(!this->_outstream) ERROR_MSG("Out of memory");
        
        this->_outstream->format = SoundIoFormatS16NE;
        this->_outstream->write_callback = Audio<T>::Callback;

        if((err = soundio_outstream_open(this->_outstream)))
            ERROR_CODE("Unable to open device: ", err);
        if(this->_outstream->layout_error)
            ERROR_CODE("Unable to set channel layout: ", this->_outstream->layout_error);
        if((err = soundio_outstream_start(this->_outstream)))
            ERROR_CODE("Unable to start device: ", err);
    }

    template<typename T>
    void Audio<T>::Run(void) {
        this->_running = true; 
        this->_thread = std::thread(&Audio::Process, this);
    }

    template<typename T>
    void Audio<T>::Process(void) { 
        while(this->_running) soundio_wait_events(this->_soundio);
    }

    template<typename T>
    void Audio<T>::Stop(void) {
        this->_running = false;
        soundio_wakeup(this->_soundio); 
        this->_thread.join();
    }

    template<typename T>
    void Audio<T>::Callback(
        struct SoundIoOutStream *outstream,
        int frame_min, 
        int frame_max
    ) {  
        const struct SoundIoChannelLayout *LAYOUT = &outstream->layout;
        const T DT_PER_FRAME = (T) 1.0 / (T) outstream->sample_rate;
        struct SoundIoChannelArea *areas;
        
        int frames_left = frame_max;
        int err;

        while(frames_left > 0) {
            if((err = soundio_outstream_begin_write(outstream, &areas, &frames_left)))
                ERROR_CODE("", err);
            if(!frames_left) break;

            for(int frame = 0; frame < frames_left; frame++) {
                Audio<T> *audio = Audio<T>::GetInstance();  
                std::function<T(T)> callback = audio->GetProcessCallback();
                
                T signal = audio->Clamp(callback(DT_PER_FRAME));
                int16_t sample = audio->Convert(signal);
                
                for(int channel = 0; channel < LAYOUT->channel_count; channel++) {
                    int16_t *ptr = (int16_t*) (areas[channel].ptr + areas[channel].step * frame);
                    *ptr = sample;
                }
            }

            if((err = soundio_outstream_end_write(outstream))) ERROR_CODE("", err);
            frames_left -= frames_left;
        }
    }

}
