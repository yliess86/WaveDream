#pragma once

#include <vector>
#include <cmath>
#include <algorithm>

#include "const.hpp"

namespace wavedream {

    class Scale {
        private:
            static std::vector<int> Compute(
                int key, 
                const int steps[], 
                const int n_steps
            );

        public:
            static std::vector<int> Major(const int key) {
                const int STEPS[] = { 2, 2, 1, 2, 2, 2, 1 };
                return Compute(key, STEPS, 7);
            }

            static std::vector<int> Minor(int key) {
                const int STEPS[] = { 2, 1, 2, 2, 1, 2, 2 };
                return Compute(key, STEPS, 7);
            }
    };

    std::vector<int> Scale::Compute(
        int key, 
        const int steps[], 
        const int n_steps
    ) {
        key = std::min(std::max(key, 0), 126);
        std::vector<int> notes;

        int current = key;
        while(current > 0) {
            for(int i = n_steps - 1; i >= 0; i--) {
                current -= steps[i];
                if(current < 0) break;
                notes.push_back(current);
            }
        }
        std::reverse(notes.begin(), notes.end());

        notes.push_back(key);
        
        current = key;
        while(current < 127) {
            for(int i = 0; i < n_steps; i++) {
                current += steps[i];
                if(current >= 127) break;
                notes.push_back(current);
            }
        }

        return notes;
    }

}