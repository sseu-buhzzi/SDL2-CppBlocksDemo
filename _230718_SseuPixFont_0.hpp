#ifndef SSEU_PIX_FONT_HPP
#define SSEU_PIX_FONT_HPP

#include "sseu_headers.hpp"

namespace Sseu {
    namespace PixFont {
        bool char_patterns[][25]{{
            false, true, true, true, false,
            true, false, false, false, true,
            true, true, true, true, true,
            true, false, false, false, true,
            true, false, false, false, true
        }, {
            true, true, true, true, false,
            true, false, false, false, true,
            true, true, true, true, false,
            true, false, false, false, true,
            true, true, true, true, false
        }, {
            false, true, true, true, true,
            true, false, false, false, false,
            true, false, false, false, false,
            true, false, false, false, false,
            false, true, true, true, true
        }, {
            true, true, true, true, false,
            true, false, false, false, true,
            true, false, false, false, true,
            true, false, false, false, true,
            true, true, true, true, false
        }, {
            true, true, true, true, true,
            true, false, false, false, false,
            true, true, true, true, true,
            true, false, false, false, false,
            true, true, true, true, true
        }, {
            true, true, true, true, true,
            true, false, false, false, false,
            true, true, true, true, true,
            true, false, false, false, false,
            true, false, false, false, false
        }, {
            false, true, true, true, true,
            true, false, false, false, false,
            true, false, false, false, true,
            true, false, false, false, true,
            false, true, true, true, true
        }, {
            true, false, false, false, true,
            true, false, false, false, true,
            true, true, true, true, true,
            true, false, false, false, true,
            true, false, false, false, true
        }, {
            true, true, true, true, true,
            false, false, true, false, false,
            false, false, true, false, false,
            false, false, true, false, false,
            true, true, true, true, true
        }, {
            true, true, true, true, true,
            false, false, true, false, false,
            false, false, true, false, false,
            false, false, true, false, false,
            true, true, false, false, false
        }, {
            true, false, false, false, true,
            true, false, false, true, false,
            true, true, true, false, false,
            true, false, false, true, false,
            true, false, false, false, true
        }, {
            true, false, false, false, false,
            true, false, false, false, false,
            true, false, false, false, false,
            true, false, false, false, false,
            true, true, true, true, true
        }, {
            true, false, false, false, true,
            true, true, false, true, true,
            true, false, true, false, true,
            true, false, false, false, true,
            true, false, false, false, true
        }, {
            true, false, false, false, true,
            true, true, false, false, true,
            true, false, true, false, true,
            true, false, false, true, true,
            true, false, false, false, true
        }, {
            false, true, true, true, false,
            true, false, false, false, true,
            true, false, false, false, true,
            true, false, false, false, true,
            false, true, true, true, false
        }, {
            true, true, true, true, false,
            true, false, false, false, true,
            true, true, true, true, false,
            true, false, false, false, false,
            true, false, false, false, false
        }, {
            false, true, true, true, false,
            true, false, false, false, true,
            true, false, false, false, true,
            true, false, false, true, false,
            false, true, true, false, true
        }, {
            true, true, true, true, false,
            true, false, false, false, true,
            true, true, true, true, false,
            true, false, false, false, true,
            true, false, false, false, true
        }, {
            false, true, true, true, true,
            true, false, false, false, false,
            false, true, true, true, false,
            false, false, false, false, true,
            true, true, true, true, false
        }, {
            true, true, true, true, true,
            false, false, true, false, false,
            false, false, true, false, false,
            false, false, true, false, false,
            false, false, true, false, false
        }, {
            true, false, false, false, true,
            true, false, false, false, true,
            true, false, false, false, true,
            true, false, false, false, true,
            false, true, true, true, false
        }, {
            true, false, false, false, true,
            true, false, false, false, true,
            true, false, false, false, true,
            false, true, false, true, false,
            false, false, true, false, false
        }, {
            true, false, false, false, true,
            true, false, true, false, true,
            true, false, true, false, true,
            true, false, true, false, true,
            false, true, false, true, false
        }, {
            true, false, false, false, true,
            false, true, false, true, false,
            false, false, true, false, false,
            false, true, false, true, false,
            true, false, false, false, true
        }, {
            true, false, false, false, true,
            true, false, false, false, true,
            false, true, false, true, false,
            false, false, true, false, false,
            false, false, true, false, false
        }, {
            true, true, true, true, true,
            false, false, false, true, false,
            false, false, true, false, false,
            false, true, false, false, false,
            true, true, true, true, true
        }, {
// a
        }, {
// b
        }, {
// c
        }, {
// d
        }, {
// e
        }, {
// f
        }, {
// g
        }, {
// h
        }, {
// i
        }, {
// j
        }, {
// k
        }, {
// l
        }, {
// m
        }, {
// n
        }, {
// o
        }, {
// p
        }, {
// q
        }, {
// r
        }, {
// s
        }, {
// t
        }, {
// u
        }, {
// v
        }, {
// w
        }, {
// x
        }, {
// y
        }, {
// z
        }, {
            false, true, true, true, false,
            true, false, false, false, true,
            true, false, true, false, true,
            true, false, false, false, true,
            false, true, true, true, false
        }, {
            false, false, true, false, false,
            true, true, true, false, false,
            false, false, true, false, false,
            false, false, true, false, false,
            true, true, true, true, true
        }, {
            false, true, true, true, false,
            true, false, false, false, true,
            false, false, true, true, false,
            false, true, false, false, false,
            true, true, true, true, true
        }, {
            false, true, true, true, false,
            true, false, false, false, true,
            false, false, true, true, false,
            true, false, false, false, true,
            false, true, true, true, false
        }, {
            true, false, false, false, true,
            true, false, false, false, true,
            false, true, true, true, true,
            false, false, false, false, true,
            false, false, false, false, true
        }, {
            true, true, true, true, true,
            true, false, false, false, false,
            true, true, true, true, false,
            false, false, false, false, true,
            true, true, true, true, false
        }, {
            false, true, true, true, true,
            true, false, false, false, false,
            true, true, true, true, false,
            true, false, false, false, true,
            false, true, true, true, false
        }, {
            true, true, true, true, true,
            false, false, false, false, true,
            false, false, false, true, false,
            false, false, true, false, false,
            false, true, false, false, false
        }, {
            false, true, true, true, false,
            true, false, false, false, true,
            false, true, true, true, false,
            true, false, false, false, true,
            false, true, true, true, false
        }, {
            false, true, true, true, false,
            true, false, false, false, true,
            false, true, true, true, true,
            false, false, false, false, true,
            true, true, true, true, false
        }, {

        }, {

        }, {

        }, {

        }, {

        }, {

        }, {

        }};
        void fill_char(::SDL_Renderer *renderer, char ch, int32_t x, int32_t y, int32_t h_pix, int32_t v_pix) {
            bool *char_pattern;
            if (ch >= 'A' && ch <= 'Z') {
                char_pattern = char_patterns[ch - 'A'];
            } else if (ch >= 'a' && ch <= 'z') {
                char_pattern = char_patterns[ch - 'a' + 26];
            } else if (ch >= '0' && ch <= '9') {
                char_pattern = char_patterns[ch - '0' + 52];
            } else { }
            for (uint8_t i{0}; i < 25; ++i) {
                if (char_pattern[i]) {
                    SDL_Rect pix{x + i % 5 * h_pix, y + i / 5 * v_pix, h_pix, v_pix};
                    SDL_RenderFillRect(renderer, &pix);
                }
            }
        }
    }
}

#endif
