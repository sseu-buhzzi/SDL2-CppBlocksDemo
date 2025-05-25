#include "MainActivity.hpp"

MainActivity::MainActivity(): gl_context{SDL_GL_CreateContext(Sseu::window)} {
    MainActivity::this_ptr = this;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Sseu::BlockSource::source_init();
    Trunk::null_block_info.block = Sseu::BlockSource::air;
    this->__TEST_TRUNK__ = new Trunk[4]{
        {0LL, 0LL},
        {0LL, 1LL},
        {1LL, 0LL},
        {1LL, 1LL}
    };
    this->entity_list.push_back(new Entity(this->__TEST_TRUNK__));
    this->camera_entity = this->entity_list[0];
    this->camera_entity->x = 2.5;
    this->camera_entity->y = 2.5;
    this->camera_entity->z = 1.0;
    this->camera_entity->box_radius = 0.375;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1875, 0.1875, -0.125, 0.125, 0.125, 64.0);
    // glOrtho(-1.5, 1.5, -1.0, 1.0, -64.0, 64.0);
    // glViewport(0, 0, 768, 512);
    this->camera.phi = M_PI_2;
    this->camera.theta = M_PI * 1.25;

    this->calculagraph.set();

    for (uint32_t i{0U}; i < Trunk::SIZE; ++i) {
        for (uint32_t j{0U}; j < Trunk::SIZE; ++j) {
            this->__TEST_TRUNK__[0].block_map[i][j][0].block = Sseu::BlockSource::white_block;
            this->__TEST_TRUNK__[1].block_map[i][j][0].block = Sseu::BlockSource::black_block;
            this->__TEST_TRUNK__[2].block_map[i][j][0].block = Sseu::BlockSource::black_block;
            this->__TEST_TRUNK__[3].block_map[i][j][0].block = Sseu::BlockSource::test_block;
            this->__TEST_TRUNK__[0].block_map[i][j][3].block = Sseu::BlockSource::black_block;
        }
    }
    // this->__TEST_TRUNK__->block_map[4][4][3].block = Sseu::BlockSource::test_block;
    // this->__TEST_TRUNK__->block_map[3][4][3].block = Sseu::BlockSource::test_block;
    // this->__TEST_TRUNK__->block_map[5][4][3].block = Sseu::BlockSource::test_block;
    // this->__TEST_TRUNK__->block_map[4][3][3].block = Sseu::BlockSource::test_block;
    // this->__TEST_TRUNK__->block_map[4][5][3].block = Sseu::BlockSource::test_block;
    if(false)for (uint8_t k{1U}; k < 8; ++k) {
        this->__TEST_TRUNK__->block_map[0][0][k].block = Sseu::BlockSource::white_block;
        this->__TEST_TRUNK__->block_map[0][1][k].block = Sseu::BlockSource::white_block;
        this->__TEST_TRUNK__->block_map[0][2][k].block = Sseu::BlockSource::white_block;
        this->__TEST_TRUNK__->block_map[0][3][k].block = Sseu::BlockSource::white_block;
        this->__TEST_TRUNK__->block_map[0][4][k].block = Sseu::BlockSource::white_block;
        this->__TEST_TRUNK__->block_map[0][5][k].block = Sseu::BlockSource::white_block;
        this->__TEST_TRUNK__->block_map[0][6][k].block = Sseu::BlockSource::white_block;
        this->__TEST_TRUNK__->block_map[0][7][k].block = Sseu::BlockSource::white_block;
        this->__TEST_TRUNK__->block_map[0][8][k].block = Sseu::BlockSource::white_block;
        this->__TEST_TRUNK__->block_map[1][0][k].block = Sseu::BlockSource::white_block;
        this->__TEST_TRUNK__->block_map[2][0][k].block = Sseu::BlockSource::white_block;
        this->__TEST_TRUNK__->block_map[3][0][k].block = Sseu::BlockSource::white_block;
        this->__TEST_TRUNK__->block_map[4][0][k].block = Sseu::BlockSource::white_block;
        this->__TEST_TRUNK__->block_map[5][0][k].block = Sseu::BlockSource::white_block;
        this->__TEST_TRUNK__->block_map[6][0][k].block = Sseu::BlockSource::white_block;
        this->__TEST_TRUNK__->block_map[7][0][k].block = Sseu::BlockSource::white_block;
        this->__TEST_TRUNK__->block_map[8][0][k].block = Sseu::BlockSource::white_block;
        this->__TEST_TRUNK__->block_map[8][8][k].block = Sseu::BlockSource::white_block;
        this->__TEST_TRUNK__->block_map[8][7][k].block = Sseu::BlockSource::white_block;
        this->__TEST_TRUNK__->block_map[8][6][k].block = Sseu::BlockSource::white_block;
        this->__TEST_TRUNK__->block_map[7][8][k].block = Sseu::BlockSource::white_block;
        this->__TEST_TRUNK__->block_map[6][8][k].block = Sseu::BlockSource::white_block;
    }
    BlockBase *__TEST_BLOCK_LIST__[3]{Sseu::BlockSource::white_block, Sseu::BlockSource::black_block, Sseu::BlockSource::test_block};
    for (uint32_t i{0U}; i < Trunk::SIZE; ++i) {
        for (uint32_t j{0U}; j < Trunk::SIZE; ++j) {
            for (uint32_t k{0U}; k < Trunk::SIZE; ++k) {
                if (this->random.rand_bool(0.01)) {
                    this->__TEST_TRUNK__[2].block_map[i][j][k].block = __TEST_BLOCK_LIST__[this->random.rand_range(3U)];
                }
            }
        }
    }
}

MainActivity::~MainActivity() {
    for (Entity *entity : this->entity_list) {
        entity->destroy();
    }
    for (Entity *entity : this->entity_list) {
        delete entity;
    }
    delete[] this->__TEST_TRUNK__;
    Sseu::BlockSource::source_quit();
    glDisable(GL_TEXTURE_2D);
    SDL_GL_DeleteContext(this->gl_context);
    MainActivity::this_ptr = nullptr;
}
