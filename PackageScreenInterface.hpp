#ifndef PACKAGE_SCREEN_INTERFACE_HPP
#define PACKAGE_SCREEN_INTERFACE_HPP

#include "BlockSource.hpp"
#include "FontSource.hpp"
#include "MouseControl.hpp"

struct CameraEntity;

struct PackageScreenInterface{
    static ::int32_t const GRID_SIZE;
    ::CameraEntity *camera_entity;
    ::uint64_t block_numb;
    ::BlockType *grid_blocks;
    ::uint32_t *tex_ids;
    ::MouseControl mouse_control;
    PackageScreenInterface(::CameraEntity *camera_entity, ::std::initializer_list<::BlockType> grid_blocks, ::std::initializer_list<uint8_t *> tex_data): camera_entity{camera_entity}, block_numb{grid_blocks.size()}, grid_blocks{new ::BlockType[this->block_numb]}, tex_ids{new ::uint32_t[this->block_numb]} {
        ::glGenTextures(this->block_numb, this->tex_ids);
        for (::uint64_t index{0ULL}; index < this->block_numb; ++index) {
            this->grid_blocks[index] = grid_blocks.begin()[index];
            ::glBindTexture(GL_TEXTURE_2D, this->tex_ids[index]);
            ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            ::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data.begin()[index]);
        }
    }
    ~PackageScreenInterface() {
        delete[] this->grid_blocks;
        ::glDeleteTextures(this->block_numb, this->tex_ids);
        delete[] this->tex_ids;
    }
    inline void update() {
        // std::cout<<this->mouse_control.get()->set()->down_diff<<'\n';
        if (this->mouse_control.get()->set()->down_diff & 0x1) {
            this->select();
        }
        this->display();
    }
    inline void select();
    inline void display() const {
        ::glColor4ub(0x00, 0x00, 0x00, 0x80);
        ::glBegin(GL_QUADS);
        ::glVertex2f(-::Sseu::window_height * 0.5, -::Sseu::window_height * 0.375);
        ::glVertex2f(-::Sseu::window_height * 0.5, ::Sseu::window_height * 0.375);
        ::glVertex2f(::Sseu::window_height * 0.5, ::Sseu::window_height * 0.375);
        ::glVertex2f(::Sseu::window_height * 0.5, -::Sseu::window_height * 0.375);
        ::glEnd();

        ::glEnable(GL_TEXTURE_2D);
        for (::uint64_t index{0ULL}; index < this->block_numb; ++index) {
            ::int32_t const x((index % 8 * 0.125 - 0.5) * ::Sseu::window_height);
            ::int32_t const y((0.375 - index / 8 * 0.125) * ::Sseu::window_height);
            ::glBindTexture(GL_TEXTURE_2D, this->tex_ids[index]);
            ::glColor3ub(0xff, 0xff, 0xff);
            ::glBegin(GL_QUADS);
            ::glTexCoord2i(0, 1);
            ::glVertex2f(x, y - ::Sseu::window_height * 0.125);
            ::glTexCoord2i(0, 0);
            ::glVertex2f(x, y);
            ::glTexCoord2i(1, 0);
            ::glVertex2f(x + ::Sseu::window_height * 0.125, y);
            ::glTexCoord2i(1, 1);
            ::glVertex2f(x + ::Sseu::window_height * 0.125, y - ::Sseu::window_height * 0.125);
            ::glEnd();
        }
                                                                // glDeleteTextures(1, &tex_id);
        ::glDisable(GL_TEXTURE_2D);
    }
};

#endif
