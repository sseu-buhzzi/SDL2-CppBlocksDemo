#ifndef BLOCK_BASE_HPP
#define BLOCK_BASE_HPP

#include "headers.hpp"

uint8_t __TEST_TEX_DATA__[16]{
    255U, 0U, 0U, 255U,
    0U, 255U, 0U, 255U,
    0U, 0U, 255U, 255U,
    255U, 255U, 0U, 255U
};
struct __TEST_FACE_TEX__{
    static uint8_t tex_data[1024];
    __TEST_FACE_TEX__() {
        int32_t size{2};
        for (uint8_t i{0U}; i < size; ++i) {
            for (uint8_t j{0U}; j < size; ++j) {
                bool is_top{i < size / 2};
                bool is_left{j < size / 2};
                uint8_t *pos{__TEST_FACE_TEX__::tex_data + (i * size + j) * 4};
                if (is_top) {
                    if (is_left) {
                        pos[0] = 255U;
                    } else {
                        pos[1] = 255U;
                    }
                } else {
                    if (is_left) {
                        pos[2] = 255U;
                    } else {
                        pos[0] = pos[1] = 255U;
                    }
                }
                pos[3] = 255U;
            }
        }
    }
};
uint8_t __TEST_FACE_TEX__::tex_data[1024]{};
__TEST_FACE_TEX__ __TEST_FACE_TEX_INSTANCE__;
uint8_t *__TEST_BLOCK_TEX__[6]{
    __TEST_FACE_TEX__::tex_data,
    __TEST_FACE_TEX__::tex_data,
    __TEST_FACE_TEX__::tex_data,
    __TEST_FACE_TEX__::tex_data,
    __TEST_FACE_TEX__::tex_data,
    __TEST_FACE_TEX__::tex_data
};

struct BlockBase{
    int32_t x, y, z;
    static int32_t faces[6][12];
    uint32_t *tex_ids;
    // BlockBase(int32_t x, int32_t y, int32_t z, uint8_t **tex_data): x{x}, y{y}, z{z} {
    BlockBase(int32_t x, int32_t y, int32_t z, uint8_t **tex_data): x{x}, y{y}, z{z}, tex_ids{new uint32_t[6]} {
        glGenTextures(6, this->tex_ids);
        for (uint8_t face{0U}; face < 6U; ++face) {
            glBindTexture(GL_TEXTURE_2D, this->tex_ids[face]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data[face]);
            // // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, __TEST_TEX_DATA__);
        }
        // // for(int32_t i{0};i<16;++i){std::cout<<i<<':'<<int32_t((*tex_data)[i])<<'\n';}
        //                                                         // for(int32_t i{0};i<16;++i){std::cout<<i<<':'<<int32_t((*tex_data)[i])<<'\t';}
                                                                
        // // GLuint textureID;
        // // glGenTextures(1, &textureID);
        // glGenTextures(1, this->tex_ids);
        // glBindTexture(GL_TEXTURE_2D, this->tex_ids[0]);

        // // 设置纹理参数
        // // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        // // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // // 加载图片数据到纹理
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, __TEST_FACE_TEX__::tex_data);
        // // glDeleteTextures(1, this->tex_ids);
        // // this->tex_ids[0] = textureID;
    }
    ~BlockBase() {
        glDeleteTextures(6, this->tex_ids);
        delete[] this->tex_ids;
    }
    void display() {

        glColor3ub(255U, 255U, 255U);
            // 创建纹理对象
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // 设置纹理参数
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // 加载图片数据到纹理
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, __TEST_FACE_TEX__::tex_data);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // 绘制矩形并映射纹理
        glBegin(GL_QUADS);
        glTexCoord2f(0, 1);
        glVertex3f(-0.5f, -0.5f, 2.0f);
        glTexCoord2f(0, 0);
        glVertex3f(-0.5f, 0.5f, 2.0f);
        glTexCoord2f(1, 0);
        glVertex3f(0.5f, 0.5f, 2.0f);
        glTexCoord2f(1, 1);
        glVertex3f(0.5f, -0.5f, 2.0f);
        glEnd();


        glPushMatrix();
        glTranslatef(this->x, this->y, this->z);
        glColor3ub(255U, 255U, 255U);
        for (int32_t (*face)[12]{BlockBase::faces}; face - BlockBase::faces < 6; ++face) {
            glBindTexture(GL_TEXTURE_2D, this->tex_ids[face - BlockBase::faces]);
            // glBindTexture(GL_TEXTURE_2D, textureID);
            // TCO<<face-BlockBase::faces<<'\n';
            glBegin(GL_QUADS);
            glTexCoord2f(0, 1);
            glVertex3iv(*face);
            glTexCoord2f(0, 0);
            glVertex3iv(*face + 3);
            glTexCoord2f(1, 0);
            glVertex3iv(*face + 6);
            glTexCoord2f(1, 1);
            glVertex3iv(*face + 9);
            glEnd();
        }
        glPopMatrix();



        // 删除纹理对象
        glDeleteTextures(1, &textureID);
    }
};
int32_t BlockBase::faces[6][12]{{
    1, 0, 0,
    1, 1, 0,
    0, 1, 0,
    0, 0, 0 // Bottom
}, {
    0, 0, 1,
    0, 1, 1,
    1, 1, 1,
    1, 0, 1 // Top
}, {
    0, 0, 0,
    0, 0, 1,
    1, 0, 1,
    1, 0, 0 // Front
}, {
    1, 1, 0,
    1, 1, 1,
    0, 1, 1,
    0, 1, 0 // Back
}, {
    0, 1, 0,
    0, 1, 1,
    0, 0, 1,
    0, 0, 0 //Left
}, {
    1, 0, 0,
    1, 0, 1,
    1, 1, 1,
    1, 1, 0 // Right
}};

#endif
