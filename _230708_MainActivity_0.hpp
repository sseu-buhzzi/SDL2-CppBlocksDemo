#ifndef MAIN_ACTIVITY_HPP
#define MAIN_ACTIVITY_HPP

#include "headers.hpp"

#include "Calculagraph.hpp"
#include "Random.hpp"
#include "Trunk.hpp"

// void inverse_matrix(const double* m, double* invOut)
// {
//     // 计算逆矩阵的算法，例如高斯-约旦消元法或LU分解

//     // 这里只是一个示例，使用高斯-约旦消元法计算逆矩阵

//     // 构建增广矩阵 [m | I]
//     double augMatrix[16];
//     for (int i = 0; i < 16; i++)
//     {
//         augMatrix[i] = (i % 5 == 0) ? 1.0 : 0.0;
//     }

//     // 高斯-约旦消元
//     for (int col = 0; col < 4; col++)
//     {
//         // 如果对角元素为0，交换行
//         if (augMatrix[col * 5] == 0.0)
//         {
//             int swapRow = -1;
//             for (int row = col + 1; row < 4; row++)
//             {
//                 if (augMatrix[row * 4 + col] != 0.0)
//                 {
//                     swapRow = row;
//                     break;
//                 }
//             }
//             if (swapRow == -1)
//             {
//                 // 矩阵不可逆
//                 return;
//             }
//             // 交换行
//             for (int i = 0; i < 4; i++)
//             {
//                 double temp = augMatrix[col * 4 + i];
//                 augMatrix[col * 4 + i] = augMatrix[swapRow * 4 + i];
//                 augMatrix[swapRow * 4 + i] = temp;
//             }
//         }

//         // 将对角元素缩放为1
//         double scale = augMatrix[col * 5];
//         for (int i = 0; i < 4; i++)
//         {
//             augMatrix[col * 4 + i] /= scale;
//         }

//         // 将其他元素消为0
//         for (int row = 0; row < 4; row++)
//         {
//             if (row != col)
//             {
//                 double factor = augMatrix[row * 4 + col];
//                 for (int i = 0; i < 4; i++)
//                 {
//                     augMatrix[row * 4 + i] -= factor * augMatrix[col * 4 + i];
//                 }
//             }
//         }
//     }

//     // 提取逆矩阵
//     for (int i = 0; i < 16; i++)
//     {
//         invOut[i] = augMatrix[i + 4];
//     }
// }


void get_world_orientation(Camera *camera, double_t viewport_x, double_t viewport_y, double_t *orientation) {
    const double_t KHI{viewport_x * M_PI * 0.3125}, UPSILON{viewport_y * M_PI_4};
    const double_t COS_KH{cos(KHI)};
    const double_t SIN_U{sin(UPSILON)};
    const double_t VX{sin(KHI)};
    const double_t VY{SIN_U};
    const double_t VZ{sqrt(COS_KH * COS_KH - SIN_U * SIN_U)};
    const double_t COS_PH{cos(camera->phi)}, SIN_PH{sin(camera->phi)};
    const double_t COS_TH{cos(camera->theta)}, SIN_TH{sin(camera->theta)};
    orientation[0] = VX * COS_TH + VY * COS_PH * SIN_TH + VZ * SIN_PH * SIN_TH;
    orientation[1] = -VX * SIN_TH - VY * COS_PH * COS_TH + VZ * SIN_PH * COS_TH;
    orientation[2] = VY * SIN_PH +  VZ * COS_PH;
}

double world_orientation_0[3], world_orientation_1[3];
std::thread get_world_orientation_thr;

struct MainActivity : Sseu::Activity{
    static MainActivity *this_ptr;
    SDL_GLContext gl_context;
    Sseu::Random random;
    Camera camera;
    Calculagraph calculagraph;
    // BlockBase __TEST_BLOCK__;
    Trunk __TEST_TRUNK__;
    MainActivity(): gl_context{SDL_GL_CreateContext(Sseu::window)} {
        MainActivity::this_ptr = this;

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_TEXTURE_2D);
                                                                // glDisable(GL_TEXTURE_2D);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Sseu::BlockSource::source_init();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(-0.1875, 0.1875, -0.125, 0.125, 0.125, 64.0);
        // glOrtho(-1.5, 1.5, -1.0, 1.0, -64.0, 64.0);
        // glViewport(0, 0, 768, 512);
        this->camera.x = 0.0;
        this->camera.y = 0.0;
        this->camera.z = 1.875;
        // this->camera.rho = 0.125;
        this->camera.phi = M_PI_2;
        this->camera.theta = 0.0;

        this->calculagraph.set();

        for (uint32_t i{0U}; i < Trunk::SIZE; ++i) {
            for (uint32_t j{0U}; j < Trunk::SIZE; ++j) {
                this->__TEST_TRUNK__.block_map[i][j][0] = Sseu::BlockSource::white_block;
            }
        }

        get_world_orientation_thr = std::thread([]() {
            while (MainActivity::this_ptr != nullptr) {
                if (::GetAsyncKeyState(VK_RETURN)) {
                    get_world_orientation(&MainActivity::this_ptr->camera, 0, 0, world_orientation_0);
                    get_world_orientation(&MainActivity::this_ptr->camera, -1, 0, world_orientation_1);
                    for (auto i{0};i<3;++i){
                        std::cout<<"xyz"[i]<<'\t'<<world_orientation_0[i]<<'\t'<<world_orientation_1[i]<<'\n';
                    }
                }
                // std::this_thread::sleep_for(std::chrono::seconds(2));
            }
        });
    }
    ~MainActivity() override {
        Sseu::BlockSource::source_quit();
                                                                // glEnable(GL_TEXTURE_2D);
        glDisable(GL_TEXTURE_2D);
        SDL_GL_DeleteContext(this->gl_context);
        MainActivity::this_ptr = nullptr;
        get_world_orientation_thr.join();
    }
    void init() override { }
    void update() override {
        this->camera.look();
                                                                // TCO<<int32_t(this->camera.x)<<' '<<int32_t(this->camera.y)<<' '<<int32_t(this->camera.z)<<'\n';
        this->__TEST_BACKGROUND__();
        // this->__TEST_BLOCK_GROUND__();
                                                                // glEnable(GL_TEXTURE_2D);
        // Sseu::BlockSource::test_block->display(512, 512, 0);
        // this->__TEST_TRUNK__.display(&this->camera);
        glDisable(GL_TEXTURE_2D);
        this->__TEST_GROUND__();
        glLineWidth(5.0F);
        glBegin(GL_LINES);
        glColor3ub(0xff, 0x00, 0x00);
        glVertex3dv(world_orientation_0);
        glColor3ub(0x00, 0x00, 0xff);
        glVertex3dv(world_orientation_1);
        glEnd();
        glEnable(GL_TEXTURE_2D);
                                                                // glDisable(GL_TEXTURE_2D);

        if (::GetAsyncKeyState(VK_UP)) {
            if (this->camera.phi >= this->calculagraph.diff) {
                this->camera.phi -= this->calculagraph.diff;
            }
        }
        if (::GetAsyncKeyState(VK_DOWN)) {
            if (this->camera.phi < M_PI - this->calculagraph.diff) {
                this->camera.phi += this->calculagraph.diff;
            }
        }
        if (::GetAsyncKeyState(VK_LEFT)) {
            this->camera.theta -= this->calculagraph.diff;
        }
        if (::GetAsyncKeyState(VK_RIGHT)) {
            this->camera.theta += this->calculagraph.diff;
        }
        if (::GetAsyncKeyState(VK_ESCAPE)) {
            Sseu::pop_activity();
        }
        if(::GetAsyncKeyState(VK_SHIFT)) {
            this->camera.z -= this->calculagraph.diff;
        }
        if (::GetAsyncKeyState(VK_SPACE)) {
            this->camera.z += this->calculagraph.diff;
        }
        if (::GetAsyncKeyState('A')) {
            this->camera.x -= this->calculagraph.diff * cos(this->camera.theta);
            this->camera.y += this->calculagraph.diff * sin(this->camera.theta);
        }
        if (::GetAsyncKeyState('D')) {
            this->camera.x += this->calculagraph.diff * cos(this->camera.theta);
            this->camera.y -= this->calculagraph.diff * sin(this->camera.theta);
        }
        if (::GetAsyncKeyState('S')) {
            this->camera.x -= this->calculagraph.diff * sin(this->camera.theta);
            this->camera.y -= this->calculagraph.diff * cos(this->camera.theta);
        }
        {
            static bool is_W_pressed{false};
            static bool is_fast{false};
            static std::chrono::system_clock::time_point last_W_time{std::chrono::system_clock::now() - std::chrono::milliseconds(1000)};
            if (::GetAsyncKeyState('W')) {
                is_fast |= std::chrono::system_clock::now() - last_W_time < std::chrono::milliseconds(100);
                if (is_fast) {
                    this->camera.x += this->calculagraph.diff * 16 * sin(this->camera.theta);
                    this->camera.y += this->calculagraph.diff * 16 * cos(this->camera.theta);
                } else {
                    this->camera.x += this->calculagraph.diff * sin(this->camera.theta);
                    this->camera.y += this->calculagraph.diff * cos(this->camera.theta);
                }
                is_W_pressed = true;
            } else {
                if (is_W_pressed) {
                    is_fast = false;
                    last_W_time = std::chrono::system_clock::now();
                }
                is_W_pressed = false;
            }
        }

        SDL_GL_SwapWindow(Sseu::window);
        this->calculagraph.get(4e-9)->set();

        // std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    void __TEST_BACKGROUND__() {
        glClearColor(0.125F, 0.125F, 0.125F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void __TEST_GROUND__() {
        glBegin(GL_QUADS);
        const int32_t SIZE{4};
        for (int32_t i{0}; i < 1024; i += SIZE) {
            for (int32_t j{0}; j < 1024; j += SIZE) {
                if ((i + j) / SIZE % 2) {
                    glColor3ub(255U, 255U, 255U);
                } else {
                    glColor3ub(128U, 128U, 128U);
                }
                glVertex3f(i, j, 0.0F);
                glVertex3f(i + SIZE, j, 0.0F);
                glVertex3f(i + SIZE, j + SIZE, 0.0F);
                glVertex3f(i, j + SIZE, 0.0F);
            }
        }
        glEnd();
        glBegin(GL_LINES);
        glColor3ub(255U, 0U, 0U);
        glVertex3f(this->camera.x, this->camera.y, this->camera.z * 0);
        glVertex3f(this->camera.x + 1, this->camera.y, this->camera.z * 0);
        glColor3ub(0U, 255U, 0U);
        glVertex3f(this->camera.x, this->camera.y, this->camera.z * 0);
        glVertex3f(this->camera.x, this->camera.y + 1, this->camera.z * 0);
        glColor3ub(0U, 0U, 255U);
        glVertex3f(this->camera.x, this->camera.y, this->camera.z * 0);
        glVertex3f(this->camera.x, this->camera.y, this->camera.z * 0 + 1);
        glEnd();
    }
    void __TEST_BLOCK_GROUND__() {
        for (int32_t i{0U}; i < 1024U; ++i) {
            for (int32_t j{0U}; j < 1024U; ++j) {
                if ([](const int32_t DX, const int32_t DY) -> bool {
                    return DX * DX + DY * DY < 1024;
                }(this->camera.x - i, this->camera.y - j)) {
                    Sseu::BlockSource::white_block->display(i, j, 0);
                }
            }
        }
    }
};
MainActivity *MainActivity::this_ptr{nullptr};

#endif
