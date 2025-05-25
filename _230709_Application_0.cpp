#include "MainActivity.hpp"

namespace __TEST__ {
    double_t xmin, xmax, ymin, ymax;
    bool flag;
}

int32_t WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    // std::cout<<(std::isnan(nan(nullptr))?"true":"false")<<'\n';
    // return 0;
    Sseu::init("Application", 64, 64, 768, 512);
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    Sseu::push_activity<MainActivity>();
    Sseu::mainloop();
    return 0;
}

void Camera::look() {
    const double_t SIN_PH{sin(this->phi)};
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        this->x, this->y, this->z,
        this->x + sin(this->theta) * SIN_PH, this->y + cos(this->theta) * SIN_PH, this->z + cos(this->phi),
        0.0, 0.0, 1.0
    );
}
void Camera::attach_to(Entity *entity) {
    this->x = entity->x;
    this->y = entity->y;
    this->z = entity->z + entity->box_height;
}
void Camera::set_viewport_orientation(const double_t VIEWPORT_X, const double_t VIEWPORT_Y) {
    const double_t KHI{VIEWPORT_X * M_PI * 0.375}, UPSILON{VIEWPORT_Y * M_PI_4};
    // const double_t KHI{VIEWPORT_X * M_PI * 0.25}, UPSILON{VIEWPORT_Y * M_PI / 6};
    const double_t COS_KH{cos(KHI)};
    const double_t SIN_U{sin(UPSILON)};
    const double_t REL_X{sin(KHI)};
    const double_t REL_Y{SIN_U};
    const double_t REL_Z{sqrt(COS_KH * COS_KH - SIN_U * SIN_U)};
    const double_t COS_PH{cos(this->phi)}, SIN_PH{sin(this->phi)};
    const double_t COS_TH{cos(this->theta)}, SIN_TH{sin(this->theta)};
    this->viewport_orientation[0] = REL_X * COS_TH - REL_Y * COS_PH * SIN_TH + REL_Z * SIN_PH * SIN_TH;
    std::isnan(this->viewport_orientation[1] = -REL_X * SIN_TH - REL_Y * COS_PH * COS_TH + REL_Z * SIN_PH * COS_TH) && (*this->viewport_orientation = nan(nullptr));
    std::isnan(this->viewport_orientation[2] = REL_Y * SIN_PH +  REL_Z * COS_PH) && (*this->viewport_orientation = nan(nullptr));
}
bool Camera::get_pixel_coord3d(const double_t DEPTH, double_t *world_coord) {
    if (std::isnan(*this->viewport_orientation)) {
        return false;
    }
    world_coord[0] = this->x + DEPTH * this->viewport_orientation[0];
    world_coord[1] = this->y + DEPTH * this->viewport_orientation[1];
    world_coord[2] = this->z + DEPTH * this->viewport_orientation[2];
    return true;
}
bool Camera::get_pixel_coord3ll(const double_t DEPTH, int64_t *world_coord) {
    if (std::isnan(*this->viewport_orientation)) {
        return false;
    }
    world_coord[0] = std::floor(this->x + DEPTH * this->viewport_orientation[0]);
    world_coord[1] = std::floor(this->y + DEPTH * this->viewport_orientation[1]);
    world_coord[2] = std::floor(this->z + DEPTH * this->viewport_orientation[2]);
    return true;
}

const uint32_t Trunk::SIZE{64U};
Trunk::BlockInfo Trunk::null_block_info{true, nullptr};
Trunk::Trunk(): block_map{nullptr} { }
Trunk::~Trunk() {
    if (this->block_map != nullptr) {
        for (uint32_t i{0U}; i < Trunk::SIZE; ++i) {
            for (uint32_t j{0U}; j < Trunk::SIZE; ++j) {
                delete[] this->block_map[i][j];
            }
            delete[] this->block_map[i];
        }
        delete[] this->block_map;
    }
}
void Trunk::generate() {
// TCO<<this<<'\t'<<Trunk::SIZE<<'\t'<<this->block_map<<'\n';
    this->block_map = new Trunk::BlockInfo **[Trunk::SIZE];
                // TCO<<'\n';
    for (uint32_t i{0U}; i < Trunk::SIZE; ++i) {
        this->block_map[i] = new Trunk::BlockInfo *[Trunk::SIZE];
        for (uint32_t j{0U}; j < Trunk::SIZE; ++j) {
            this->block_map[i][j] = new BlockInfo[Trunk::SIZE];
            for (uint32_t k{0U}; k < Trunk::SIZE; ++k) {
                // TCO<<'\n';
                this->block_map[i][j][k] = {true, Sseu::BlockSource::air};
            }
        }
    }
}
void Trunk::display_flush() {
    for (uint32_t i{0U}; i < Trunk::SIZE; ++i) {
        for (uint32_t j{0U}; j < Trunk::SIZE; ++j) {
            for (uint32_t k{0U}; k < Trunk::SIZE; ++k) {
                this->block_map[i][j][k].not_displayed = true;
            }
        }
    }
}
bool Trunk::contains_point_at(int32_t x, int32_t y, int32_t z) {
    return
        x >= 0 && y >= 0 && z >= 0
        && x < Trunk::SIZE && y < Trunk::SIZE && z < Trunk::SIZE
    ;
}

TrunkMap::TrunkMap(int64_t front_offset, int64_t back_offset, int64_t left_offset, int64_t right_offset): front_offset{front_offset}, back_offset{back_offset}, left_offset{left_offset}, right_offset{right_offset} {
    int64_t map_width{left_offset + right_offset};
    int64_t map_height{front_offset + back_offset};
    // int64_t origin_offset{left_offset * map_width + front_offset};
    this->origin = new Trunk *[map_height];
    Trunk *trunk_data{new Trunk[map_width * map_height]};
// std::cout<<"origin: "<<this->origin<<'\t'<<"data: "<<trunk_data<<'\n';
    for (int64_t x{0LL}; x < map_width; ++x) {
        this->origin[x] = trunk_data + x * map_height + front_offset;
    }
    this->origin += left_offset;
// TCO<<trunk_data<<' '<<trunk_data+map_width*map_height<<'\n';
}
TrunkMap::~TrunkMap() {
    this->origin -= this->left_offset;
// std::cout<<"origin: "<<this->origin<<'\t'<<"data: "<<*this->origin-this->front_offset+1<<'\n';
    delete[] (*this->origin - this->front_offset);
    delete[] this->origin;
}
void TrunkMap::display(Camera *camera) {
    int64_t coord[3];
    for (double_t viewport_x{-1.0}; viewport_x <= 1.0; viewport_x += 0.1/*0.02*/) {
        for (double_t viewport_y{-1.0}; viewport_y <= 1.0; viewport_y += 0.1) {
            camera->set_viewport_orientation(viewport_x, viewport_y);
            for (double_t depth{1.0}; depth < 32.0; ++depth) {
                if (camera->get_pixel_coord3ll(depth, coord)) {
// TCO<<coord[0]<<'\t'<<coord[1]<<'\t'<<coord[2]<<'\n';
__TEST__::flag=true;
                    Trunk::BlockInfo *block_info{this->get_block_info(coord[0], coord[1], coord[2])};
__TEST__::flag=false;
// TCO<<block_info<<'\t'<<block_info->block<<'\t'<<int32_t(block_info->block->interaction_level)<<'\n';
// TCO<<coord[0]<<coord[1]<<coord[2]<<'\n';
                    if (block_info->block->interaction_level >= 0x01) {
// TCO<<Sseu::BlockSource::block_enum[block_info->block]<<'\n';
// TCO<<coord[0]<<'\t'<<coord[1]<<'\t'<<coord[2]<<'\n';
                    block_info->block->display(coord[0], coord[1], coord[2]);
                    // block_info->not_displayed = false;
                        // if (block_info->block->interaction_level >= 0x02) {
                        //     break;
                        // }
                    }
                }
            }
        }
    }
}
Trunk::BlockInfo *TrunkMap::get_block_info(int64_t x, int64_t y, uint32_t z) {
    if (z < 0 || z >= Trunk::SIZE) {
        return &Trunk::null_block_info;
    }
    const int32_t REL_X(uint64_t(x) % Trunk::SIZE), REL_Y(uint64_t(y) % Trunk::SIZE);
    const int32_t X_INDEX((x - REL_X) / Trunk::SIZE), Y_INDEX((y - REL_Y) / Trunk::SIZE);
// if(x>=0&&x<4&&y>=0&&y<=4){
//  std::cout<<X_INDEX<<Y_INDEX;
// }
// if(__TEST__::flag){std::cout<<this->left_offset<<'\t'<<this->right_offset<<'\t'<<this->front_offset<<'\t'<<back_offset<<'\n';}
    if (X_INDEX < -this->left_offset || X_INDEX >= this->right_offset || Y_INDEX < -this->front_offset || Y_INDEX >= this->back_offset) {
        return &Trunk::null_block_info;
    }
// if(__TEST__::flag){TCO<<this->left_offset<<' '<<X_INDEX<<' '<<this->right_offset<<'\t'<<this->front_offset<<' '<<Y_INDEX<<' '<<this->back_offset<<'\n';}
// if(X_INDEX==0&&Y_INDEX==0){
//  TCO<<'\n';
// }
// if(__TEST__::flag){TCO<<X_INDEX<<'\t'<<Y_INDEX<<'\n';}
    Trunk::BlockInfo ***block_map{this->origin[X_INDEX][Y_INDEX].block_map};
// std::cout<<int32_t(y/Trunk::SIZE)<<'\t'<<this->origin[x/Trunk::SIZE][y/Trunk::SIZE].block_map<<'\n';
// TCO<<x/Trunk::SIZE<<'\t'<<y/Trunk::SIZE<<'\n';
    if (block_map == nullptr) {
        return &Trunk::null_block_info;
    }
// if(__TEST__::flag){std::cout<<z;std::cout<<'\t'<<block_map[x%Trunk::SIZE][y%Trunk::SIZE]+z<<'\n';}
// std::cout<<block_map[x%Trunk::SIZE][y%Trunk::SIZE]+z<<'\t'<<Sseu::BlockSource::block_enum[block_map[x%Trunk::SIZE][y%Trunk::SIZE][z].block]<<'\n';
    return block_map[uint64_t(x) % Trunk::SIZE][uint64_t(y) % Trunk::SIZE] + z;
}

Entity::Entity(): dx{0.0}, dy{0.0}, dz{0.0}, is_existing{true}, update_thr([this]() -> void {
    Calculagraph calculagraph;
    calculagraph.set();
    while (this->is_existing) {
        calculagraph.get(4e-9)->set();

        this->dx *= 0.875;
        this->dy *= 0.875;
        this->dz -= calculagraph.diff;

        this->update_vertical_motion(std::min(this->dz * calculagraph.diff, 0.25));
        this->update_horizon_motion(std::min(this->dx * calculagraph.diff, 0.25), std::min(this->dy * calculagraph.diff, 0.25));
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}) { }
Entity::~Entity() {
    this->update_thr.join();
}
void Entity::destroy() {
    this->is_existing = false;
}
void Entity::display() {
    glColor3ub(0xff, 0x00, 0x00);
    glBegin(GL_LINE_LOOP);
    for (double_t theta{0.0}; theta < M_PI * 2; theta += M_PI / 12) {
        glVertex3f(x + this->box_radius * sin(theta), y + this->box_radius * cos(theta), z + box_height);
    }
    glEnd();

    glLineWidth(4.0F);
    glBegin(GL_LINES);
    glVertex3f(x, y, z + box_height);
    glColor3ub(0x00, 0x00, 0xff);
    glVertex3f(x, y, z);
    glEnd();

    glBegin(GL_LINE_LOOP);
    for (double_t theta{0.0}; theta < M_PI * 2; theta += M_PI / 12) {
        glVertex3f(x + this->box_radius * sin(theta), y + this->box_radius * cos(theta), z);
    }
    glEnd();
}
void Entity::update_horizon_motion(const double_t DX, const double_t DY) {
    TrunkMap *trunk_map{&reinterpret_cast<MainActivity *>(Sseu::activity_stack.top())->trunk_map};
    const double_t BOX_LEFT{this->x - this->box_radius};
    const double_t BOX_RIGHT{this->x + this->box_radius};
    const double_t BOX_FRONT{this->y - this->box_radius};
    const double_t BOX_BACK{this->y + this->box_radius};
    const double_t BOX_TOP{this->z + this->box_height};
    double_t left_xmin{std::max(
        trunk_map->get_block_info(BOX_LEFT + DX, this->y, this->z)->block->get_right_border(this->z - int32_t(this->z)),
        trunk_map->get_block_info(BOX_LEFT + DX, this->y, BOX_TOP)->block->get_right_border(BOX_TOP - int32_t(BOX_TOP))
    ) + int32_t(BOX_LEFT) + DX};
    double_t right_xmax{std::min(
        trunk_map->get_block_info(BOX_RIGHT + DX, this->y, this->z)->block->get_left_border(this->z - int32_t(this->z)),
        trunk_map->get_block_info(BOX_RIGHT + DX, this->y, BOX_TOP)->block->get_left_border(BOX_TOP - int32_t(BOX_TOP))
    ) + int32_t(BOX_RIGHT) + DX};
    double_t front_ymin{std::max(
        trunk_map->get_block_info(this->x, BOX_FRONT + DY, this->z)->block->get_back_border(this->z - int32_t(this->z)),
        trunk_map->get_block_info(this->x, BOX_FRONT + DY, BOX_TOP)->block->get_back_border(BOX_TOP - int32_t(BOX_TOP))
    ) + int32_t(BOX_FRONT) + DY};
    double_t back_ymax{std::min(
        trunk_map->get_block_info(this->x, BOX_BACK + DY, this->z)->block->get_front_border(this->z - int32_t(this->z)),
        trunk_map->get_block_info(this->x, BOX_BACK + DY, BOX_TOP)->block->get_front_border(BOX_TOP - int32_t(BOX_TOP))
    ) + int32_t(BOX_BACK) + DY};
    if (BOX_LEFT + DX < left_xmin) {
        this->dx = std::max(this->dx, 0.0);
    }
    if (BOX_RIGHT + DX > right_xmax) {
        this->dx = std::min(this->dx, 0.0);
    }
    if (BOX_FRONT + DY < front_ymin) {
        this->dy = std::max(this->dy, 0.0);
    }
    if (BOX_BACK + DY > back_ymax) {
        this->dy = std::min(this->dy, 0.0);
    }
    if (this->dx != 0.0) {
        this->x += DX;
    }
    if (this->dy != 0.0) {
        this->y += DY;
    }
                                                            __TEST__::xmin = left_xmin;
                                                            __TEST__::xmax = right_xmax;
                                                            __TEST__::ymin = front_ymin;
                                                            __TEST__::ymax = back_ymax;
}
void Entity::update_vertical_motion(const double_t DZ) {
    TrunkMap *trunk_map{&reinterpret_cast<MainActivity *>(Sseu::activity_stack.top())->trunk_map};
    const double_t BOX_TOP{this->z + this->box_height};
    const double_t BOX_FRONT{this->y - this->box_radius};
    const double_t BOX_BACK{this->y + this->box_radius};
    const double_t BOX_LEFT{this->x - this->box_radius};
    const double_t BOX_RIGHT{this->x + this->box_radius};
    double_t bottom_zmin{std::max({
        trunk_map->get_block_info(this->x, this->y, this->z + DZ)->block->get_top_border(this->x - int32_t(this->x), this->y - int32_t(this->y)),
        trunk_map->get_block_info(this->x, this->y, this->z + DZ)->block->get_top_border(this->x - int32_t(this->x), this->y - int32_t(this->y)),
        trunk_map->get_block_info(BOX_LEFT, this->y, this->z + DZ)->block->get_top_border(BOX_LEFT - int32_t(BOX_LEFT), this->y - int32_t(this->y)),
        trunk_map->get_block_info(BOX_RIGHT, this->y, this->z + DZ)->block->get_top_border(BOX_RIGHT - int32_t(BOX_RIGHT), this->y - int32_t(this->y)),
        trunk_map->get_block_info(this->x, BOX_FRONT, this->z + DZ)->block->get_top_border(this->x - int32_t(this->x), BOX_FRONT - int32_t(BOX_FRONT)),
        trunk_map->get_block_info(this->x, BOX_BACK, this->z + DZ)->block->get_top_border(this->x - int32_t(this->x), BOX_BACK - int32_t(BOX_BACK))
    }) + int32_t(this->z + DZ)};
    double_t top_zmax{std::min({
        trunk_map->get_block_info(this->x, this->y, BOX_TOP + DZ)->block->get_bottom_border(this->x - int32_t(this->x), this->y - int32_t(this->y)),
        trunk_map->get_block_info(BOX_LEFT, this->y, BOX_TOP + DZ)->block->get_bottom_border(BOX_LEFT - int32_t(BOX_LEFT), this->y - int32_t(this->y)),
        trunk_map->get_block_info(BOX_RIGHT, this->y, BOX_TOP + DZ)->block->get_bottom_border(BOX_RIGHT - int32_t(BOX_RIGHT), this->y - int32_t(this->y)),
        trunk_map->get_block_info(this->x, BOX_FRONT, BOX_TOP + DZ)->block->get_bottom_border(this->x - int32_t(this->x), BOX_FRONT - int32_t(BOX_FRONT)),
        trunk_map->get_block_info(this->x, BOX_BACK, BOX_TOP + DZ)->block->get_bottom_border(this->x - int32_t(this->x), BOX_BACK - int32_t(BOX_BACK))
    }) + int32_t(BOX_TOP + DZ)};
    if (this->z + DZ < bottom_zmin) {
        this->dz = std::max(this->dz, 0.0);
    }
    if (BOX_TOP + DZ > top_zmax) {
        this->dz = std::min(this->dz, 0.0);
    }
    if (this->dz != 0.0) {
        this->z += DZ;
    }
}

MainActivity::MainActivity(): gl_context{SDL_GL_CreateContext(Sseu::window)}, trunk_map(0LL, 1LL, 0LL, 1LL) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Sseu::BlockSource::source_init();
    Trunk::null_block_info.block = Sseu::BlockSource::air;
// TCO<<'\n';
    BlockBase *__TEST_BLOCK_LIST__[3]{Sseu::BlockSource::white_block, Sseu::BlockSource::black_block, Sseu::BlockSource::test_block};
    for (int64_t x{this->trunk_map.left_offset}; x < this->trunk_map.right_offset; ++x) {
        for (int64_t y{this->trunk_map.front_offset}; y < this->trunk_map.right_offset; ++y) {
            this->trunk_map.origin[x][y].generate();
            if (x == 0LL && y == 0LL) {
                for (uint32_t i{0U}; i < Trunk::SIZE; ++i) {
                    for (uint32_t j{0U}; j < Trunk::SIZE; ++j) {
                        for (uint32_t k{0U}; k < Trunk::SIZE; ++k) {
                            if (this->random.rand_bool(0.01)) {
                                this->trunk_map.origin[x][y].block_map[i][j][k].block = __TEST_BLOCK_LIST__[this->random.rand_range(3U)];
                            }
                        }
                    }
                }
            }
        }
    }
// TCO<<'\n';
    this->entity_list.push_back(new Entity);
    this->camera_entity = this->entity_list[0];
    this->camera_entity->x = 2.0;
    this->camera_entity->y = 2.0;
    this->camera_entity->z = 16.0;
    this->camera_entity->box_radius = 0.375;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1875, 0.1875, -0.125, 0.125, 0.125, 64.0);
    // glOrtho(-1.5, 1.5, -1.0, 1.0, -64.0, 64.0);
    // glViewport(0, 0, 768, 512);
    this->camera.phi = M_PI_2;
    this->camera.theta = 0.0;

    this->calculagraph.set();

    for (uint32_t i{0U}; i < Trunk::SIZE; ++i) {
        for (uint32_t j{0U}; j < Trunk::SIZE; ++j) {
            this->trunk_map.origin[0][0].block_map[i][j][0].block = Sseu::BlockSource::white_block;
            // this->trunk_map.origin[0][1].block_map[i][j][0].block = Sseu::BlockSource::black_block;
            // this->trunk_map.origin[1][0].block_map[i][j][0].block = Sseu::BlockSource::black_block;
            // this->trunk_map.origin[1][1].block_map[i][j][0].block = Sseu::BlockSource::test_block;
            // this->trunk_map.origin[-1][-1].block_map[i][j][3].block = Sseu::BlockSource::black_block;
        }
    }
    // this->trunk_map.origin[0][0].block_map[4][4][3].block = Sseu::BlockSource::test_block;
    // this->trunk_map.origin[0][0].block_map[3][4][3].block = Sseu::BlockSource::test_block;
    // this->trunk_map.origin[0][0].block_map[5][4][3].block = Sseu::BlockSource::test_block;
    // this->trunk_map.origin[0][0].block_map[4][3][3].block = Sseu::BlockSource::test_block;
    // this->trunk_map.origin[0][0].block_map[4][5][3].block = Sseu::BlockSource::test_block;
    if(false)for (uint8_t k{1U}; k < 8; ++k) {
        this->trunk_map.origin[0][0].block_map[0][0][k].block = Sseu::BlockSource::white_block;
        this->trunk_map.origin[0][0].block_map[0][1][k].block = Sseu::BlockSource::white_block;
        this->trunk_map.origin[0][0].block_map[0][2][k].block = Sseu::BlockSource::white_block;
        this->trunk_map.origin[0][0].block_map[0][3][k].block = Sseu::BlockSource::white_block;
        this->trunk_map.origin[0][0].block_map[0][4][k].block = Sseu::BlockSource::white_block;
        this->trunk_map.origin[0][0].block_map[0][5][k].block = Sseu::BlockSource::white_block;
        this->trunk_map.origin[0][0].block_map[0][6][k].block = Sseu::BlockSource::white_block;
        this->trunk_map.origin[0][0].block_map[0][7][k].block = Sseu::BlockSource::white_block;
        this->trunk_map.origin[0][0].block_map[0][8][k].block = Sseu::BlockSource::white_block;
        this->trunk_map.origin[0][0].block_map[1][0][k].block = Sseu::BlockSource::white_block;
        this->trunk_map.origin[0][0].block_map[2][0][k].block = Sseu::BlockSource::white_block;
        this->trunk_map.origin[0][0].block_map[3][0][k].block = Sseu::BlockSource::white_block;
        this->trunk_map.origin[0][0].block_map[4][0][k].block = Sseu::BlockSource::white_block;
        this->trunk_map.origin[0][0].block_map[5][0][k].block = Sseu::BlockSource::white_block;
        this->trunk_map.origin[0][0].block_map[6][0][k].block = Sseu::BlockSource::white_block;
        this->trunk_map.origin[0][0].block_map[7][0][k].block = Sseu::BlockSource::white_block;
        this->trunk_map.origin[0][0].block_map[8][0][k].block = Sseu::BlockSource::white_block;
        this->trunk_map.origin[0][0].block_map[8][8][k].block = Sseu::BlockSource::white_block;
        this->trunk_map.origin[0][0].block_map[8][7][k].block = Sseu::BlockSource::white_block;
        this->trunk_map.origin[0][0].block_map[8][6][k].block = Sseu::BlockSource::white_block;
        this->trunk_map.origin[0][0].block_map[7][8][k].block = Sseu::BlockSource::white_block;
        this->trunk_map.origin[0][0].block_map[6][8][k].block = Sseu::BlockSource::white_block;
    }
    if(false)for (uint32_t i{0U}; i < Trunk::SIZE; ++i) {
        for (uint32_t j{0U}; j < Trunk::SIZE; ++j) {
            for (uint32_t k{0U}; k < Trunk::SIZE; ++k) {
                if (this->random.rand_bool(0.01)) {
                    this->trunk_map.origin[1][0].block_map[i][j][k].block = __TEST_BLOCK_LIST__[this->random.rand_range(3U)];
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
    Sseu::BlockSource::source_quit();
    glDisable(GL_TEXTURE_2D);
    SDL_GL_DeleteContext(this->gl_context);
}
void MainActivity::init() { }
void MainActivity::update() {
    this->calculagraph.get(4e-9)->set();
    this->camera.attach_to(this->camera_entity);
    this->camera.look();
                                                            // TCO<<int32_t(this->camera.x)<<' '<<int32_t(this->camera.y)<<' '<<int32_t(this->camera.z)<<'\n';
    this->__TEST_BACKGROUND__();
    // this->__TEST_BLOCK_GROUND__();
                                                            // glEnable(GL_TEXTURE_2D);
    // Sseu::BlockSource::test_block->display(512, 512, 0);
// TCO<<"LHG-GK5-890G-8058-684\n";
// TCO<<'\n';
    this->trunk_map.display(&this->camera);

// for(uint32_t i{0U};i<Trunk::SIZE;++i){
//  for(uint32_t j{0U};j<Trunk::SIZE;++j){
//   this->trunk_map.origin[0][0].block_map[i][j][0].block->display(i,j,0);
//  }
// }
    // glDisable(GL_TEXTURE_2D);
    // for (Entity *entity : this->entity_list) {
    //     entity->display();
    //     if (!entity->trunk_appended->contains_point_at(entity->x - entity->trunk_appended->x, entity->y - entity->trunk_appended->y, entity->z)) {
    //         ;
    //     }
    // }
    // glEnable(GL_TEXTURE_2D);
                                                            // if (::GetAsyncKeyState(VK_RETURN)) {
                                                            //     MainActivity::this_ptr->camera.set_viewport_orientation(-1.0, -1.0);
                                                            //     MainActivity::this_ptr->camera.get_pixel_coord(4.0, world_coord);
                                                            //     MainActivity::this_ptr->camera.set_viewport_orientation(-1.0, 1.0);
                                                            //     MainActivity::this_ptr->camera.get_pixel_coord(4.0, world_coord + 3);
                                                            //     MainActivity::this_ptr->camera.set_viewport_orientation(1.0, 1.0);
                                                            //     MainActivity::this_ptr->camera.get_pixel_coord(4.0, world_coord + 6);
                                                            //     MainActivity::this_ptr->camera.set_viewport_orientation(1.0, -1.0);
                                                            //     MainActivity::this_ptr->camera.get_pixel_coord(4.0, world_coord + 9);
                                                            //     // for (auto i{0};i<3;++i){
                                                            //     //     std::cout<<"xyz"[i]<<'\t'<<world_coord_0[i]<<'\t'<<world_coord_1[i]<<'\n';
                                                            //     // }
                                                            //     // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                                                            // }
    glDisable(GL_TEXTURE_2D);
    this->camera_entity->display();
                                                            glBegin(GL_LINE_LOOP);
                                                            glColor3ub(0x00, 0xff, 0x00);
                                                            glVertex3f(__TEST__::xmin, __TEST__::ymin, 1.01);
                                                            glVertex3f(__TEST__::xmin, __TEST__::ymax, 1.01);
                                                            glVertex3f(__TEST__::xmax, __TEST__::ymax, 1.01);
                                                            glVertex3f(__TEST__::xmax, __TEST__::ymin, 1.01);
                                                            glEnd();
    // this->__TEST_GROUND__();
                                                            // glLineWidth(16.0F);
                                                            // glBegin(GL_LINE_LOOP);
                                                            // glColor3ub(0x00, 0x00, 0xff);
                                                            // glVertex3dv(world_coord);
                                                            // glColor3ub(0xff, 0x00, 0x00);
                                                            // glVertex3dv(world_coord + 3);
                                                            // glColor3ub(0x00, 0xff, 0x00);
                                                            // glVertex3dv(world_coord + 6);
                                                            // glColor3ub(0xff, 0xff, 0x00);
                                                            // glVertex3dv(world_coord + 9);
                                                            // glEnd();
    glEnable(GL_TEXTURE_2D);
                                                            // glDisable(GL_TEXTURE_2D);
// if(::GetAsyncKeyState('Q')){this->camera_entity->is_existing=false;}
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
        // this->camera.z -= this->calculagraph.diff;
        this->camera_entity->box_height = 1.0;
    } else {
        this->camera_entity->box_height = 1.75;
    }
    if (::GetAsyncKeyState(VK_SPACE)) {
        // this->camera.z += this->calculagraph.diff;
        this->camera_entity->dz = this->calculagraph.diff * 96;
    }
// "Useful, don't delete it";
    if (::GetAsyncKeyState('A')) {
        this->camera_entity->dx -= this->calculagraph.diff * cos(this->camera.theta);
        this->camera_entity->dy += this->calculagraph.diff * sin(this->camera.theta);
    }
    if (::GetAsyncKeyState('D')) {
        this->camera_entity->dx += this->calculagraph.diff * cos(this->camera.theta);
        this->camera_entity->dy -= this->calculagraph.diff * sin(this->camera.theta);
    }
    if (::GetAsyncKeyState('S')) {
        this->camera_entity->dx -= this->calculagraph.diff * sin(this->camera.theta);
        this->camera_entity->dy -= this->calculagraph.diff * cos(this->camera.theta);
    }
    {
        static bool is_W_pressed{false};
        static bool is_fast{false};
        static std::chrono::system_clock::time_point last_W_time{std::chrono::system_clock::now() - std::chrono::milliseconds(1000)};
        if (::GetAsyncKeyState('W')) {
            is_fast |= std::chrono::system_clock::now() - last_W_time < std::chrono::milliseconds(100);
            if (is_fast) {
                this->camera_entity->dx += this->calculagraph.diff * 4 * sin(this->camera.theta);
                this->camera_entity->dy += this->calculagraph.diff * 4 * cos(this->camera.theta);
            } else {
                this->camera_entity->dx += this->calculagraph.diff * sin(this->camera.theta);
                this->camera_entity->dy += this->calculagraph.diff * cos(this->camera.theta);
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
    // std::this_thread::sleep_for(std::chrono::milliseconds(250));
}
