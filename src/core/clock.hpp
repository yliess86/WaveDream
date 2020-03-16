#pragma once

namespace wavedream {

    template<typename T>
    class Clock {
        private:
            T _time;
            T _speed;
        
        public:
            Clock(T speed=1): _speed(speed) {}

            T GetTime(void) { return this->_time; };
            T GetSpeed() { return this->_speed; }
            
            void SetSpeed(T speed) { this->_speed = speed; }
            
            T Update(T dt) { this->_time += dt * this->_speed; return this->_time; }
    };

}