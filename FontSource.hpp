#ifndef FONT_SOURCE_HPP
#define FONT_SOURCE_HPP

#include "headers.hpp"
#include ".\FontTexData.hpp"

// #include ".\FontTexData_Test.hpp"

namespace Sseu::FontSource {
    ::uint32_t font_tex_ids[128];
    inline void source_init() {
        ::glGenTextures(128, ::Sseu::FontSource::font_tex_ids);
        for (char ch{'\x00'}; ch >= 0; ++ch) {
            ::glBindTexture(GL_TEXTURE_2D, ::Sseu::FontSource::font_tex_ids[ch]);
            ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            ::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 9, 18, 0, GL_RGBA, GL_UNSIGNED_BYTE, ::Sseu::FontSource::font_tex_data[ch]);
            // ::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 9, 18, 0, GL_RGBA, GL_UNSIGNED_BYTE, ::Sseu::FontSource::font_tex_data_Test[0]);
        }
    }

    inline void source_quit() {
        ::glDeleteTextures(128, ::Sseu::FontSource::font_tex_ids);
    }

    inline void draw_text(char const *text, ::double_t const font_size) {
        for (::double_t offset{0.0}; *text != '\x00'; ++text, offset += font_size) {
            ::glBindTexture(GL_TEXTURE_2D, font_tex_ids[*text]);
            ::glBegin(GL_QUADS);
            ::glTexCoord2i(0, 1);
            ::glVertex2d(offset, 0.0);
            ::glTexCoord2i(0, 0);
            ::glVertex2d(offset, font_size * 2);
            ::glTexCoord2i(1, 0);
            ::glVertex2d(offset + font_size, font_size * 2);
            ::glTexCoord2i(1, 1);
            ::glVertex2d(offset + font_size, 0.0);
            ::glEnd();
        }
    }
}

#endif
