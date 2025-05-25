#ifndef FRAME_RATE_COUNTER_HPP
#define FRAME_RATE_COUNTER_HPP

#include "headers.hpp"

namespace Sseu {
    struct FrameRateCounter{
        int32_t fps;
        bool is_counting;
        std::thread count_thr;
        FrameRateCounter(): is_counting{true}, count_thr([this]() -> void {
            // ::std::this_thread::sleep_for(::std::chrono::seconds(4));
            while (this->is_counting) {
                this->count_func();
            }
        }) {
            this->count_thr.detach();
        }
        ~FrameRateCounter() {
            this->is_counting = false;
        }
        inline void count_func();
    };
}

#endif
