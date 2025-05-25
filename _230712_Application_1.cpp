#include "MainActivity.hpp"

namespace __TEST__ {
    double_t xmin, xmax, ymin, ymax;
    bool flag;
}

int32_t WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    // double_t theta, mod_th;
    // modf(M_PI * 2 * 1 * M_1_PI * 0.5 + 0.125, &mod_th);
    // std::cout<<mod_th<<'\n';
    // while (true) {
    //     std::cin >> theta;
    //     theta *= M_PI;
    //     mod_th = modf(theta * M_1_PI * 0.5 + 0.125, nullptr);
    //     std::cout << theta * M_1_PI * 0.5 << '\t' << mod_th << '\n';
    // }
    // return 0;
    Sseu::init("Application", 64, 64, 768, 512);
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    // Sseu::push_activity<MainActivity>();
    Sseu::activity_stack.push<MainActivity>();
    Sseu::mainloop();
    return 0;
}

BlockInstance::BlockInstance(): not_refreshed{true}, rs_signal_strength{0U}, linked_block{}, block{nullptr} { }

void Camera::look() {
    const double_t sin_ph{sin(this->phi)};
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        this->x, this->y, this->z,
        this->x + sin(this->theta) * sin_ph, this->y + cos(this->theta) * sin_ph, this->z + cos(this->phi),
        0.0, 0.0, 1.0
    );
}
void Camera::set_viewport_vector(const double_t viewport_x, const double_t viewport_y) {
    const double_t khi{viewport_x * M_PI * 0.375}, upsilon{viewport_y * M_PI_4};
    // const double_t khi{viewport_x * M_PI * 0.25}, upsilon{viewport_y * M_PI / 6};
    const double_t cos_kh{cos(khi)};
    const double_t sin_u{sin(upsilon)};
    const double_t rel_x{sin(khi)};
    const double_t rel_y{sin_u};
    const double_t rel_z{sqrt(cos_kh * cos_kh - sin_u * sin_u)};
    const double_t cos_ph{cos(this->phi)}, sin_ph{sin(this->phi)};
    const double_t cos_th{cos(this->theta)}, sin_th{sin(this->theta)};
    this->viewport_vector[0] = rel_x * cos_th - rel_y * cos_ph * sin_th + rel_z * sin_ph * sin_th;
    std::isnan(this->viewport_vector[1] = -rel_x * sin_th - rel_y * cos_ph * cos_th + rel_z * sin_ph * cos_th) && (*this->viewport_vector = nan(nullptr));
    std::isnan(this->viewport_vector[2] = rel_y * sin_ph +  rel_z * cos_ph) && (*this->viewport_vector = nan(nullptr));
}
bool Camera::get_pixel_coord3dv(const double_t depth, double_t *world_coord) {
    if (std::isnan(*this->viewport_vector)) {
        return false;
    }
    world_coord[0] = this->x + depth * this->viewport_vector[0];
    world_coord[1] = this->y + depth * this->viewport_vector[1];
    world_coord[2] = this->z + depth * this->viewport_vector[2];
    return true;
}
bool Camera::get_pixel_coord3llv(const double_t depth, int64_t *world_coord) {
    if (std::isnan(*this->viewport_vector)) {
        return false;
    }
    world_coord[0] = std::floor(this->x + depth * this->viewport_vector[0]);
    world_coord[1] = std::floor(this->y + depth * this->viewport_vector[1]);
    world_coord[2] = std::floor(this->z + depth * this->viewport_vector[2]);
    return true;
}
void Camera::select_block() {
    this->set_viewport_vector(0.0, 0.0);
    for (double_t depth{0.0}, coord[3]; depth < 8.0; depth += 0.125) {
        if (this->get_pixel_coord3dv(depth, coord)) {
            this->selected_block = reinterpret_cast<MainActivity *>(Sseu::activity_stack.top())->trunk_map.get_block_inst(std::floor(coord[0]), std::floor(coord[1]), std::floor(coord[2]));
            if (this->selected_block->block->interaction_level >= 0x02) {
                this->selected_block->block->select(this->selected_block->coord[0], this->selected_block->coord[1], this->selected_block->coord[2]);
                break;
            }
        }
    }
}
void Camera::show_cursor() {
    glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // glOrtho(0.0, Sseu::window_width, 0.0, Sseu::window_height, -64.0, 64.0);
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
    // gluLookAt(
    //     Sseu::window_width / 2, Sseu::window_height / 2, 0.0,
    //     Sseu::window_width / 2, Sseu::window_height / 2, -1.0,
    //     0.0, 1.0, 0.0
    // );
    double_t coord[3];
    glColor3ub(0xff, 0xff, 0xff);
    glBegin(GL_LINES);
    this->set_viewport_vector(-0.05, 0.0);
    this->get_pixel_coord3dv(0.25, coord);
    glVertex3dv(coord);
    this->set_viewport_vector(0.05, 0.0);
    this->get_pixel_coord3dv(0.25, coord);
    glVertex3dv(coord);
    this->set_viewport_vector(0.0, -0.075);
    this->get_pixel_coord3dv(0.25, coord);
    glVertex3dv(coord);
    this->set_viewport_vector(0.0, 0.075);
    this->get_pixel_coord3dv(0.25, coord);
    glVertex3dv(coord);
    glEnd();
}

const uint32_t Trunk::SIZE{64U};
BlockInstance Trunk::null_block_inst;
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
void Trunk::generate(int64_t x_index, int64_t y_index) {
    this->block_map = new BlockInstance **[Trunk::SIZE];
    for (uint32_t i{0U}; i < Trunk::SIZE; ++i) {
        this->block_map[i] = new BlockInstance *[Trunk::SIZE];
        for (uint32_t j{0U}; j < Trunk::SIZE; ++j) {
            this->block_map[i][j] = new BlockInstance[Trunk::SIZE];
            for (uint32_t k{0U}; k < Trunk::SIZE; ++k) {
                BlockInstance *block_inst{this->block_map[i][j] + k};
                block_inst->block = Sseu::BlockSource::block_enum[BBT_AIR];
                block_inst->coord[0] = x_index * Trunk::SIZE + i;
                block_inst->coord[1] = y_index * Trunk::SIZE + j;
                block_inst->coord[2] = k;
            }
        }
    }
}
void Trunk::display_flush() {
    for (uint32_t i{0U}; i < Trunk::SIZE; ++i) {
        for (uint32_t j{0U}; j < Trunk::SIZE; ++j) {
            for (uint32_t k{0U}; k < Trunk::SIZE; ++k) {
                // this->block_map[i][j][k].not_displayed = true;
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
    this->origin = new Trunk *[map_height];
    Trunk *trunk_data{new Trunk[map_width * map_height]};
    for (int64_t x{0LL}; x < map_width; ++x) {
        this->origin[x] = trunk_data + x * map_height + front_offset;
    }
    this->origin += left_offset;
}
TrunkMap::~TrunkMap() {
    this->origin -= this->left_offset;
    delete[] (*this->origin - this->front_offset);
    delete[] this->origin;
}
BlockInstance *TrunkMap::get_block_inst(int64_t x, int64_t y, uint32_t z) {
    if (z < 0 || z >= Trunk::SIZE) {
        return &Trunk::null_block_inst;
    }
    const int32_t rel_x(uint64_t(x) % Trunk::SIZE), rel_y(uint64_t(y) % Trunk::SIZE);
    const int32_t x_index((x - rel_x) / Trunk::SIZE), y_index((y - rel_y) / Trunk::SIZE);
    if (x_index < -this->left_offset || x_index >= this->right_offset || y_index < -this->front_offset || y_index >= this->back_offset) {
        return &Trunk::null_block_inst;
    }
    BlockInstance ***block_map{this->origin[x_index][y_index].block_map};
    if (block_map == nullptr) {
        return &Trunk::null_block_inst;
    }
    return block_map[uint64_t(x) % Trunk::SIZE][uint64_t(y) % Trunk::SIZE] + z;
}

Entity::Entity(TrunkMap *trunk_map): trunk_map{trunk_map}, dx{0.0}, dy{0.0}, dz{0.0}, is_existing{true} { }
Entity::~Entity() {
    this->is_existing = false;
}
void Entity::start_thrs() {
    this->update_thrs.emplace_back([this]() -> void {
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
    });
    this->update_thrs.back().detach();
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
void Entity::update_horizon_motion(const double_t dx, const double_t dy) {
    const double_t box_left{this->x - this->box_radius + dx};
    const double_t box_right{this->x + this->box_radius + dx};
    const double_t box_front{this->y - this->box_radius + dy};
    const double_t box_back{this->y + this->box_radius + dy};
    const double_t box_top{this->z + this->box_height};
    const int64_t x_floor(std::floor(this->x + dx));
    const int64_t y_floor(std::floor(this->y + dy));
    const int64_t z_floor(std::floor(this->z));
    const int64_t box_left_floor(std::floor(box_left));
    const int64_t box_right_floor(std::floor(box_right));
    const int64_t box_front_floor(std::floor(box_front));
    const int64_t box_back_floor(std::floor(box_back));
    const int64_t box_top_floor(std::floor(box_top));
    double_t left_xmin{std::max(
        this->trunk_map->get_block_inst(box_left_floor, y_floor, z_floor)->block->get_right_border(this->z - z_floor),
        this->trunk_map->get_block_inst(box_left_floor, y_floor, box_top)->block->get_right_border(box_top - box_top_floor)
    ) + box_left_floor};
    double_t right_xmax{std::min(
        this->trunk_map->get_block_inst(box_right_floor, y_floor, z_floor)->block->get_left_border(this->z - z_floor),
        this->trunk_map->get_block_inst(box_right_floor, y_floor, box_top)->block->get_left_border(box_top - box_top_floor)
    ) + box_right_floor};
    double_t front_ymin{std::max(
        this->trunk_map->get_block_inst(x_floor, box_front_floor, z_floor)->block->get_back_border(this->z - z_floor),
        this->trunk_map->get_block_inst(x_floor, box_front_floor, box_top)->block->get_back_border(box_top - box_top_floor)
    ) + box_front_floor};
    double_t back_ymax{std::min(
        this->trunk_map->get_block_inst(x_floor, box_back_floor, z_floor)->block->get_front_border(this->z - z_floor),
        this->trunk_map->get_block_inst(x_floor, box_back_floor, box_top)->block->get_front_border(box_top - box_top_floor)
    ) + box_back_floor};
    if (box_left + dx < left_xmin) {
        this->dx = std::max(this->dx, 0.0);
    }
    if (box_right > right_xmax) {
        this->dx = std::min(this->dx, 0.0);
    }
    if (box_front < front_ymin) {
        this->dy = std::max(this->dy, 0.0);
    }
    if (box_back > back_ymax) {
        this->dy = std::min(this->dy, 0.0);
    }
    if (this->dx != 0.0) {
        this->x += dx;
    }
    if (this->dy != 0.0) {
        this->y += dy;
    }
    
                                                            __TEST__::xmin = left_xmin;
                                                            __TEST__::xmax = right_xmax;
                                                            __TEST__::ymin = front_ymin;
                                                            __TEST__::ymax = back_ymax;
}
void Entity::update_vertical_motion(const double_t dz) {
    const double_t box_left{this->x - this->box_radius};
    const double_t box_right{this->x + this->box_radius};
    const double_t box_front{this->y - this->box_radius};
    const double_t box_back{this->y + this->box_radius};
    const double_t box_top{this->z + this->box_height + dz};
    const int64_t x_floor(std::floor(this->x));
    const int64_t y_floor(std::floor(this->y));
    const int64_t z_floor(std::floor(this->z + dz));
    const int64_t box_left_floor(std::floor(box_left));
    const int64_t box_right_floor(std::floor(box_right));
    const int64_t box_front_floor(std::floor(box_front));
    const int64_t box_back_floor(std::floor(box_back));
    const int64_t box_top_floor(std::floor(box_top));
    double_t bottom_zmin{std::max({
        this->trunk_map->get_block_inst(x_floor, y_floor, z_floor)->block->get_top_border(this->x - x_floor, this->y - y_floor),
        this->trunk_map->get_block_inst(x_floor, y_floor, z_floor)->block->get_top_border(this->x - x_floor, this->y - y_floor),
        this->trunk_map->get_block_inst(box_left_floor, y_floor, z_floor)->block->get_top_border(box_left - box_left_floor, this->y - y_floor),
        this->trunk_map->get_block_inst(box_right_floor, y_floor, z_floor)->block->get_top_border(box_right - box_right_floor, this->y - y_floor),
        this->trunk_map->get_block_inst(x_floor, box_front_floor, z_floor)->block->get_top_border(this->x - x_floor, box_front - box_front_floor),
        this->trunk_map->get_block_inst(x_floor, box_back_floor, z_floor)->block->get_top_border(this->x - x_floor, box_back - box_back_floor)
    }) + z_floor};
    double_t top_zmax{std::min({
        this->trunk_map->get_block_inst(x_floor, y_floor, box_top + dz)->block->get_bottom_border(this->x - x_floor, this->y - y_floor),
        this->trunk_map->get_block_inst(box_left_floor, y_floor, box_top + dz)->block->get_bottom_border(box_left - box_left_floor, this->y - y_floor),
        this->trunk_map->get_block_inst(box_right_floor, y_floor, box_top + dz)->block->get_bottom_border(box_right - box_right_floor, this->y - y_floor),
        this->trunk_map->get_block_inst(x_floor, box_front_floor, box_top + dz)->block->get_bottom_border(this->x - x_floor, box_front - box_front_floor),
        this->trunk_map->get_block_inst(x_floor, box_back_floor, box_top + dz)->block->get_bottom_border(this->x - x_floor, box_back - box_back_floor)
    }) + box_top};
    if (this->z + dz < bottom_zmin) {
        this->dz = std::max(this->dz, 0.0);
    }
    if (box_top + dz > top_zmax) {
        this->dz = std::min(this->dz, 0.0);
    }
    if (this->dz != 0.0) {
        this->z += dz;
    }
}

CameraEntity::CameraEntity(TrunkMap *trunk_map, Camera *camera): Entity(trunk_map), camera{camera}, phi{M_PI_2}, theta{0.0}, holding_block{Sseu::BlockSource::block_enum[BBT_AIR]} { }
void CameraEntity::start_thrs() {
    this->update_thrs.emplace_back([this]() -> void {
        Calculagraph calculagraph, click_counter;
        MouseControl mouse_control;
        // int32_t mouse_state;
        calculagraph.set();
        click_counter.set();
        mouse_control.set();
        while (this->is_existing) {
            calculagraph.get(4e-9)->set();
            const double_t diff_cos_th{calculagraph.diff * cos(this->theta) * 2}, diff_sin_th{calculagraph.diff * sin(this->theta) * 2};
            (this->dx += this->is_walking_lati * diff_cos_th + this->is_walking_longi * diff_sin_th) *= 0.875;
            (this->dy += this->is_walking_longi * diff_cos_th - this->is_walking_lati * diff_sin_th) *= 0.875;
            if (this->is_jumping) {
                this->dz = 16 * calculagraph.diff;
            } else {
                this->dz -= calculagraph.diff;
            }
            if (this->is_crouching) {
                this->box_height = 1.5;
            } else {
                this->box_height = 1.875;
            }
            this->update_vertical_motion(std::min(this->dz * calculagraph.diff, 0.25));
            this->update_horizon_motion(std::min(this->dx * calculagraph.diff, 0.25), std::min(this->dy * calculagraph.diff, 0.25));
            // this->hold_camera();
            // this->camera->look();
// TCO<<this->x<<'\t'<<this->y<<'\t'<<this->z<<'\n';
            this->control(&mouse_control);
            if (mouse_control.x <= 1 || mouse_control.x >= Sseu::window_width - 1 || mouse_control.y <= 0 || mouse_control.y >= Sseu::window_height - 1) {
                SDL_WarpMouseInWindow(Sseu::window, Sseu::window_width / 2, Sseu::window_height / 2);
                mouse_control.set();
            }
            // mouse_state = SDL_GetMouseState(nullptr, nullptr);
            if (mouse_control.down_diff & 0x5) {
                if (mouse_control.down_diff & 0x1) {
                    this->break_block();
                }
                if (mouse_control.down_diff & 0x4) {
                    this->place_block();
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    });
    this->update_thrs.back().detach();
}
void CameraEntity::hold_camera() {
    this->camera->x = this->x;
    this->camera->y = this->y;
    this->camera->z = this->z + this->box_height;
    this->camera->phi = this->phi;
    this->camera->theta = this->theta;
}
void CameraEntity::control(MouseControl *mouse_control) {
    double_t new_phi{this->phi + mouse_control->y_diff * 0.01};
    if (new_phi >= 0 && new_phi < M_PI) {
        this->phi = new_phi;
    }
    this->theta += mouse_control->x_diff * 0.01;
    mouse_control->get()->set();
}
void CameraEntity::place_block() {
    int64_t place_x, place_y, place_z;
    if (this->phi < M_PI_4) {
        place_x = this->camera->selected_block->coord[0];
        place_y = this->camera->selected_block->coord[1];
        place_z = this->camera->selected_block->coord[2] - 1;
    } else if (this->phi < M_PI_4 * 3) {
        switch (uint8_t(std::floor(this->theta * M_1_PI * 2 + 0.5)) & 0x3) {
            case 0x00: {
                place_x = this->camera->selected_block->coord[0];
                place_y = this->camera->selected_block->coord[1] - 1;
                break;
            }
            case 0x01: {
                place_x = this->camera->selected_block->coord[0] - 1;
                place_y = this->camera->selected_block->coord[1];
                break;
            }
            case 0x02: {
                place_x = this->camera->selected_block->coord[0];
                place_y = this->camera->selected_block->coord[1] + 1;
                break;
            }
            case 0x03: {
                place_x = this->camera->selected_block->coord[0] + 1;
                place_y = this->camera->selected_block->coord[1];
                break;
            }
        }
        place_z = this->camera->selected_block->coord[2];
    } else {
        place_x = this->camera->selected_block->coord[0];
        place_y = this->camera->selected_block->coord[1];
        place_z = this->camera->selected_block->coord[2] + 1;
    }
    BlockInstance *placed_block{reinterpret_cast<MainActivity *>(Sseu::activity_stack.top())->trunk_map.get_block_inst(place_x, place_y, place_z)};
    if (placed_block->block->interaction_level == 0x00 && placed_block != &Trunk::null_block_inst) {
        // placed_block->block = std::next(Sseu::BlockSource::block_enum.begin(), reinterpret_cast<MainActivity *>(Sseu::activity_stack.top())->random.rand_range(Sseu::BlockSource::block_enum.size()))->second;
        placed_block->block = this->holding_block;
        Sseu::activity_stack.top<MainActivity>()->event_deque.emplace_back(GameEvent{GameEvent::PLACE, this, placed_block});
    }
}
void CameraEntity::break_block() {
    Sseu::activity_stack.top<MainActivity>()->event_deque.emplace_back(GameEvent{GameEvent::BREAK, this, this->camera->selected_block});
    this->camera->selected_block->block = Sseu::BlockSource::block_enum[BBT_AIR];
}

MainActivity::MainActivity(): is_existing{true}, gl_context{SDL_GL_CreateContext(Sseu::window)}, trunk_map(1LL, 2LL, 1LL, 2LL), event_thr([this]() -> void {
    while (this->is_existing) {
        while (!this->event_deque.empty()) {
            this->event_func();
            this->event_deque.pop_front();
        }
    }
}) {
    this->event_thr.detach();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);

    Sseu::BlockSource::source_init();
    Trunk::null_block_inst.block = Sseu::BlockSource::block_enum[BBT_AIR];

    this->camera_entity = new CameraEntity(&this->trunk_map, &this->camera);
    this->entity_list.push_back(this->camera_entity);
    this->camera_entity->x = 2.5;
    this->camera_entity->y = 2.5;
    this->camera_entity->z = 16.0;
    this->camera_entity->box_radius = 0.25;
    this->camera_entity->start_thrs();

    // glOrtho(-1.5, 1.5, -1.0, 1.0, -64.0, 64.0);
    // glViewport(0, 0, 768, 512);
    this->camera.phi = M_PI_2;
    this->camera.theta = 0.0;

    this->__TEST_TERRAIN__();
}
MainActivity::~MainActivity() {
    this->is_existing = false;
    for (Entity *entity : this->entity_list) {
        delete entity;
    }
    Sseu::BlockSource::source_quit();
    glDisable(GL_TEXTURE_2D);
    SDL_GL_DeleteContext(this->gl_context);
}
void MainActivity::init() { }
void MainActivity::update() {
    // this->calculagraph.get(4e-9)->set();
    // this->camera_entity->update_vertical_motion(std::min(this->camera_entity->dz * calculagraph.diff, 0.25));
    // this->camera_entity->update_horizon_motion(std::min(this->camera_entity->dx * calculagraph.diff, 0.25), std::min(this->camera_entity->dy * calculagraph.diff, 0.25));
    // this->camera_entity->hold_camera();
    // this->camera.look();
    this->start_world_painting();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    this->clear_background();
    this->world_display();
    // this->trunk_map.origin[0][0].block_map[2][2][1].block->display(2, 2, 1);
    // this->trunk_map.origin[0][0].block_map[2][2][0].block->display(2, 2, 0);
    glDisable(GL_TEXTURE_2D);
    this->camera_entity->display();
    this->camera.select_block();
                                                            glBegin(GL_LINE_LOOP);
                                                            glColor3ub(0x00, 0xff, 0x00);
                                                            glVertex3f(__TEST__::xmin, __TEST__::ymin, 1.01);
                                                            glVertex3f(__TEST__::xmin, __TEST__::ymax, 1.01);
                                                            glVertex3f(__TEST__::xmax, __TEST__::ymax, 1.01);
                                                            glVertex3f(__TEST__::xmax, __TEST__::ymin, 1.01);
                                                            glEnd();
    this->camera.show_cursor();

    glEnable(GL_TEXTURE_2D);

    this->camera_entity->is_jumping = ::GetAsyncKeyState(VK_SPACE);
    this->camera_entity->is_crouching = ::GetAsyncKeyState(VK_SHIFT);
    this->camera_entity->is_walking_lati = ::GetAsyncKeyState('A') ? -0x01 : ::GetAsyncKeyState('D') ? 0x01 : 0x0;
    this->camera_entity->is_walking_longi = ::GetAsyncKeyState('S') ? -0x01 : ::GetAsyncKeyState('W') ? 0x01 : 0x0;
    if (::GetAsyncKeyState('0')) {
        this->camera_entity->holding_block = Sseu::BlockSource::block_enum[BBT_AIR];
        std::cout << "air\n";
    }
    if (::GetAsyncKeyState('1')) {
        this->camera_entity->holding_block = Sseu::BlockSource::block_enum[BBT_RED_STONE_BLOCK];
        std::cout << "red_stone_block\n";
    }
    if (::GetAsyncKeyState('2')) {
        this->camera_entity->holding_block = Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER];
        std::cout << "red_stone_powder\n";
    }
    if (::GetAsyncKeyState(VK_ESCAPE)) {
        // Sseu::pop_activity();
        Sseu::activity_stack.pop();
    }

    SDL_GL_SwapWindow(Sseu::window);
}
void MainActivity::clear_background() {
    glClearColor(0.125F, 0.125F, 0.125F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void MainActivity::start_world_painting() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1875, 0.1875, -0.125, 0.125, 0.125, 64.0);
    this->camera_entity->hold_camera();
    this->camera.look();
}
void MainActivity::start_screen_painting() {

}
void MainActivity::world_display() {
    int64_t coord[3];
    for (double_t viewport_x{-1.0}; viewport_x <= 1.0; viewport_x += 0.075/*0.02*/) {
        for (double_t viewport_y{-1.0}; viewport_y <= 1.0; viewport_y += 0.05) {
            this->camera.set_viewport_vector(viewport_x, viewport_y);
            for (double_t depth{16.0}; depth > 0.0 && this->camera.get_pixel_coord3llv(depth, coord); --depth) {
                this->trunk_map.get_block_inst(coord[0], coord[1], coord[2])->block->display(coord[0], coord[1], coord[2]);
            }
        }
    }
}
void MainActivity::event_func() {
    GameEvent *event{&this->event_deque.front()};
// TCO<<event->type<<'\n';
    // std::cout << event->type << '\t' << Sseu::BlockSource::block_name[event->block_inst->block] << '\n';
    if (event->type == GameEvent::PLACE) {
        BlockInstance *neighbour;
        if ((neighbour = this->trunk_map.get_block_inst(event->block_inst->coord[0], event->block_inst->coord[1], event->block_inst->coord[2] - 1)) != &Trunk::null_block_inst) {
            neighbour->not_refreshed = true;
            event->block_inst->linked_block[0] = neighbour;
        }
        if ((neighbour = this->trunk_map.get_block_inst(event->block_inst->coord[0], event->block_inst->coord[1], event->block_inst->coord[2] + 1)) != &Trunk::null_block_inst) {
            neighbour->not_refreshed = true;
            event->block_inst->linked_block[1] = neighbour;
        }
        if ((neighbour = this->trunk_map.get_block_inst(event->block_inst->coord[0], event->block_inst->coord[1] - 1, event->block_inst->coord[2])) != &Trunk::null_block_inst) {
            neighbour->not_refreshed = true;
            event->block_inst->linked_block[2] = neighbour;
        }
        if ((neighbour = this->trunk_map.get_block_inst(event->block_inst->coord[0], event->block_inst->coord[1] + 1, event->block_inst->coord[2])) != &Trunk::null_block_inst) {
            neighbour->not_refreshed = true;
            event->block_inst->linked_block[3] = neighbour;
        }
        if ((neighbour = this->trunk_map.get_block_inst(event->block_inst->coord[0] - 1, event->block_inst->coord[1], event->block_inst->coord[2])) != &Trunk::null_block_inst) {
            neighbour->not_refreshed = true;
            event->block_inst->linked_block[4] = neighbour;
        }
        if ((neighbour = this->trunk_map.get_block_inst(event->block_inst->coord[0] + 1, event->block_inst->coord[1], event->block_inst->coord[2])) != &Trunk::null_block_inst) {
            neighbour->not_refreshed = true;
            event->block_inst->linked_block[5] = neighbour;
        }
        this->refresh_block(event->block_inst);
        if (event->block_inst->block->block_type == BT_RED_STONE_BLOCK) {
            this->event_deque.emplace_back(GameEvent{GameEvent::ACTIVATE, nullptr, event->block_inst});
        }
    } else if (event->type == GameEvent::BREAK) {
        if (event->block_inst->is_rs_activated) {
            this->event_deque.emplace_back(GameEvent{GameEvent::DEACTIVATE, nullptr, event->block_inst});
        }
    } else if (event->type == GameEvent::ACTIVATE) {
// TCO<<Sseu::BlockSource::block_name[event->block_inst->block]<<'\t'<<event->block_inst->coord[0]<<'\t'<<event->block_inst->coord[1]<<'\t'<<event->block_inst->coord[2]<<'\n';
        event->block_inst->is_rs_activated = true;
        for (BlockInstance **linked_block{event->block_inst->linked_block}; linked_block - event->block_inst->linked_block < 0x06; ++linked_block) {
            if (*linked_block != nullptr && (*linked_block)->block->block_type == BT_RED_STONE_POWDER) {
                this->event_deque.emplace_back(GameEvent{GameEvent::ACTIVATE, nullptr, *linked_block});
                (*linked_block)->block = Sseu::BlockSource::block_enum[BBT_ACTIVE_RED_STONE_POWDER];
            }
        }
    } else if (event->type == GameEvent::DEACTIVATE) {
        event->block_inst->is_rs_activated = false;
        for (BlockInstance **linked_block{event->block_inst->linked_block}; linked_block - event->block_inst->linked_block < 0x06; ++linked_block) {
            if (*linked_block != nullptr && (*linked_block)->block->block_type == BT_RED_STONE_POWDER && event->block_inst->is_rs_activated) {
                event->block_inst->is_rs_activated |= (*linked_block)->is_rs_activated;
                this->event_deque.emplace_back(GameEvent{GameEvent::DEACTIVATE, nullptr, *linked_block});
                (*linked_block)->block = Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER];
            }
        }
        // if (!event->block_inst->is_rs_activated) {
        //     this->event_deque.emplace_back(GameEvent{GameEvent::DEACTIVATE, nullptr, *linked_block});

        // }
    }
}
void MainActivity::refresh_block(BlockInstance *block_inst) {
    block_inst->not_refreshed = false;
    if (block_inst->block->block_type == BT_RED_STONE_POWDER) {
        uint8_t linking_state{0x00}, mask{0x08};
        for (BlockInstance **linked_block{block_inst->linked_block + 2}; linked_block - block_inst->linked_block < 6; ++linked_block) {
            if ((*linked_block)->block->block_type == BT_RED_STONE_POWDER) {
                linking_state |= mask;
                if ((*linked_block)->not_refreshed) {
                    this->refresh_block(*linked_block);
                }
            }
            mask >>= 1;
        }
        switch (linking_state) {
            case 0x00: {
                block_inst->block = Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER];
                break;
            }
            case 0x01 ... 0x03: {
                block_inst->block = Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_LR];
                break;
            }
            case 0x04: case 0x08: case 0x0c: {
                block_inst->block = Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_FB];
                break;
            }
            case 0x05: {
                block_inst->block = Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_BR];
                break;
            }
            case 0x06: {
                block_inst->block = Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_BL];
                break;
            }
            case 0x07: {
                block_inst->block = Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_BLR];
                break;
            }
            case 0x09: {
                block_inst->block = Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_FR];
                break;
            }
            case 0x0a: {
                block_inst->block = Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_FL];
                break;
            }
            case 0x0b: {
                block_inst->block = Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_FLR];
                break;
            }
            case 0x0d: {
                block_inst->block = Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_FBR];
                break;
            }
            case 0x0e: {
                block_inst->block = Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_FBL];
                break;
            }
            case 0x0f: {
                block_inst->block = Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_FBLR];
                break;
            }
        }
    }
}
