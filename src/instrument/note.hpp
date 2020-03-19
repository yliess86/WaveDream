#pragma once

#include "../core/const.hpp"

namespace wavedream {

    template<typename T>
    struct Note {
        int id;
        T on;
        T off;
        T max_life;
        bool active;

        Note(int id, T on): 
            id(id), on(on), off((T) 0.0), max_life((T) 5.0), active(true) {}
        Note(int id, T on, T max_life): 
            id(id), on(on), off((T) 0.0), max_life(max_life), active(true) {}
    };

}