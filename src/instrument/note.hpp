#pragma once

#include "../core/const.hpp"

namespace wavedream {

    template<typename T>
    struct Note {
        int id;
        T on;
        T off;
        bool active;

        Note(int id, T on): id(id), on(on), off(0.0), active(true) {}
    };

}