#ifndef DISPLAYER_HPP
#define DISPLAYER_HPP

#include "sseu_headers.hpp"

struct DisplayerBase{
    uint32_t tex_id;
    DisplayerBase(int32_t tex_width, int32_t tex_height, uint8_t *tex_data) {
        glGenTextures(1, &this->tex_id);
        glBindTexture(GL_TEXTURE_2D, this->tex_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
    }
    ~DisplayerBase() {
        glDeleteTextures(1, &this->tex_id);
    }
    virtual void display() = 0;
};


struct ComplexDisplayerBase{
    int32_t tex_numb;
    uint32_t *tex_ids;
    ComplexDisplayerBase(int32_t tex_numb, int32_t *tex_widths, int32_t *tex_heights, uint8_t **tex_data): tex_numb{tex_numb}, tex_ids{new uint32_t[tex_numb]} {
        glGenTextures(tex_numb, this->tex_ids);
        for (int32_t i{0}; i < tex_numb; ++i) {
            glBindTexture(GL_TEXTURE_2D, this->tex_ids[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_widths[i], tex_heights[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data[i]);
        }
    }
    ~ComplexDisplayerBase() {
        glDeleteTextures(this->tex_numb, this->tex_ids);
        delete[] this->tex_ids;
    }
    virtual void display() = 0;
};

#endif
