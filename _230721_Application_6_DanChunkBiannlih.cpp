#include "MainActivity.hpp"

BlockInstance::BlockInstance(): not_refreshed{true}, rs_signal_strength{0}, linked_block_list{}, block{nullptr} { }

void PackageScreenInterface::select() {
    ::int32_t const x(::std::floor((::double_t(this->mouse_control.x) / ::Sseu::window_height - 0.25) * 8));
    ::int32_t const y(::std::floor((::double_t(this->mouse_control.y) / ::Sseu::window_height - 0.125) * 8));
    this->camera_entity->holding_block = ::Sseu::BlockSource::block_enum[::Sseu::BlockSource::block_base_default[this->grid_blocks[y * 8 + x]]];
    ::std::cout << '\n' << ::Sseu::BlockSource::block_name[this->camera_entity->holding_block] << '\n';
}

namespace __TEST__ {
    ::double_t xmin, xmax, ymin, ymax;
    bool flag;
    bool key_E_state;
    inline void info(::BlockInstance *bi) {
        ::std::cout << bi->coord[0] << ' ' << bi->coord[1] << ' ' << bi->coord[2] << ' ' << ::Sseu::BlockSource::block_name[bi->block] << ' ' << bi->rs_signal_strength << '\n';
    }
}

/*
■◣◢◥◤■■▬▮∎
*/

int32_t WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    ::Sseu::init("SseuCraft", 64, 64, 768, 512);
    ::Sseu::BlockSource::source_init();
    ::Sseu::FontSource::source_init();
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    ::Sseu::activity_stack.push<::MainActivity>();
    ::Sseu::mainloop();
    ::Sseu::BlockSource::source_quit();
    ::Sseu::FontSource::source_quit();
    ::Sseu::quit();
    TCO << "Sseucraft quit successfully\n";
    return 0;
}

void Camera::look() const {
    double_t const sin_ph{::sin(this->phi)};
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        this->x, this->y, this->z,
        this->x + ::sin(this->theta) * sin_ph, this->y + ::cos(this->theta) * sin_ph, this->z + ::cos(this->phi),
        0.0, 0.0, 1.0
    );
}
void Camera::set_viewport_vector(double_t const viewport_x, double_t const viewport_y) {
    double_t const khi{viewport_x * M_PI * 0.375}, upsilon{viewport_y * M_PI_4};
    // double_t const khi{viewport_x * M_PI * 0.25}, upsilon{viewport_y * M_PI / 6};
    double_t const cos_kh{::cos(khi)};
    double_t const sin_u{::sin(upsilon)};
    double_t const rel_x{::sin(khi)};
    double_t const rel_y{sin_u};
    double_t const rel_z{sqrt(cos_kh * cos_kh - sin_u * sin_u)};
    double_t const cos_ph{::cos(this->phi)}, sin_ph{::sin(this->phi)};
    double_t const cos_th{::cos(this->theta)}, sin_th{::sin(this->theta)};
    this->viewport_vector[0] = rel_x * cos_th - rel_y * cos_ph * sin_th + rel_z * sin_ph * sin_th;
    ::std::isnan(this->viewport_vector[1] = -rel_x * sin_th - rel_y * cos_ph * cos_th + rel_z * sin_ph * cos_th) && (*this->viewport_vector = nan(nullptr));
    ::std::isnan(this->viewport_vector[2] = rel_y * sin_ph +  rel_z * cos_ph) && (*this->viewport_vector = nan(nullptr));
}
bool Camera::get_viewport_vector(double_t const viewport_x, double_t const viewport_y, double_t *viewport_vector) const {
    double_t const khi{viewport_x * M_PI * 0.375}, upsilon{viewport_y * M_PI_4};
    // double_t const khi{viewport_x * M_PI * 0.25}, upsilon{viewport_y * M_PI / 6};
    double_t const cos_kh{::cos(khi)};
    double_t const sin_u{::sin(upsilon)};
    double_t const rel_x{::sin(khi)};
    double_t const rel_y{sin_u};
    double_t const rel_z{sqrt(cos_kh * cos_kh - sin_u * sin_u)};
    double_t const cos_ph{::cos(this->phi)}, sin_ph{::sin(this->phi)};
    double_t const cos_th{::cos(this->theta)}, sin_th{::sin(this->theta)};
    viewport_vector[0] = rel_x * cos_th - rel_y * cos_ph * sin_th + rel_z * sin_ph * sin_th;
    ::std::isnan(viewport_vector[1] = -rel_x * sin_th - rel_y * cos_ph * cos_th + rel_z * sin_ph * cos_th) && (*viewport_vector = nan(nullptr));
    ::std::isnan(viewport_vector[2] = rel_y * sin_ph +  rel_z * cos_ph) && (*viewport_vector = nan(nullptr));
    return !::std::isnan(*viewport_vector);
}
bool Camera::get_pixel_coord3dv(double_t const depth, double_t *world_coord) const {
    if (::std::isnan(*this->viewport_vector)) {
        return false;
    }
    world_coord[0] = this->x + depth * this->viewport_vector[0];
    world_coord[1] = this->y + depth * this->viewport_vector[1];
    world_coord[2] = this->z + depth * this->viewport_vector[2];
    return true;
}
bool Camera::get_pixel_coord3llv(double_t const depth, int64_t *world_coord) const {
    if (::std::isnan(*this->viewport_vector)) {
        return false;
    }
    world_coord[0] = ::std::floor(this->x + depth * this->viewport_vector[0]);
    world_coord[1] = ::std::floor(this->y + depth * this->viewport_vector[1]);
    world_coord[2] = ::std::floor(this->z + depth * this->viewport_vector[2]);
    return true;
}
bool Camera::v_get_pixel_coord3llv(double_t *viewport_vector, double_t const depth, int64_t *world_coord) const {
    if (::std::isnan(*viewport_vector)) {
        return false;
    }
    world_coord[0] = ::std::floor(this->x + depth * viewport_vector[0]);
    world_coord[1] = ::std::floor(this->y + depth * viewport_vector[1]);
    world_coord[2] = ::std::floor(this->z + depth * viewport_vector[2]);
    return true;
}
void Camera::select_block() {
    this->set_viewport_vector(0.0, 0.0);
    for (::double_t depth{0.0}, coord[3]; depth < 8.0; depth += 0.125) {
        if (this->get_pixel_coord3dv(depth, coord)) {
            this->selected_block = reinterpret_cast<MainActivity *>(::Sseu::activity_stack.top())->chunk_map.get_block_inst(::std::floor(coord[0]), ::std::floor(coord[1]), ::std::floor(coord[2]));
            if (this->selected_block->block->interaction_level >= 0x02) {
                this->selected_block->block->select(this->selected_block->coord[0], this->selected_block->coord[1], this->selected_block->coord[2]);
                break;
            }
        }
    }
}

uint32_t const Chunk::SIZE{64U};
// BlockInstance Chunk::null_block_inst;
Chunk::Chunk(): block_map{nullptr} { }
Chunk::~Chunk() {
    if (this->block_map != nullptr) {
        for (uint32_t i{0U}; i < Chunk::SIZE; ++i) {
            for (uint32_t j{0U}; j < Chunk::SIZE; ++j) {
                delete[] this->block_map[i][j];
            }
            delete[] this->block_map[i];
        }
        delete[] this->block_map;
    }
}
void Chunk::display_flush() const{
    for (uint32_t i{0U}; i < Chunk::SIZE; ++i) {
        for (uint32_t j{0U}; j < Chunk::SIZE; ++j) {
            for (uint32_t k{0U}; k < Chunk::SIZE; ++k) {
                // this->block_map[i][j][k].not_displayed = true;
            }
        }
    }
}
bool Chunk::contains_point_at(int32_t const x, int32_t const y, int32_t const z) const {
    return
        x >= 0 && y >= 0 && z >= 0
        && x < Chunk::SIZE && y < Chunk::SIZE && z < Chunk::SIZE
    ;
}

ChunkMap::ChunkMap(int64_t const front_offset, int64_t const back_offset, int64_t const left_offset, int64_t const right_offset): front_offset{front_offset}, back_offset{back_offset}, left_offset{left_offset}, right_offset{right_offset} {
    int64_t map_width{left_offset + right_offset};
    int64_t map_height{front_offset + back_offset};
    this->origin = new Chunk *[map_height];
    Chunk *trunk_data{new Chunk[map_width * map_height]};
    for (int64_t x{0LL}; x < map_width; ++x) {
        this->origin[x] = trunk_data + x * map_height + front_offset;
    }
    this->origin += left_offset;
}
ChunkMap::~ChunkMap() {
    this->origin -= this->left_offset;
    delete[] (*this->origin - this->front_offset);
    delete[] this->origin;
}
BlockInstance *ChunkMap::get_block_inst(int64_t const x, int64_t const y, uint32_t z) const {
    if (z < 0 || z >= Chunk::SIZE) {
        return &::Sseu::BlockSource::null_block_inst;
    }
    ::int32_t const rel_x(::uint64_t(x) % ::Chunk::SIZE), rel_y(::uint64_t(y) % ::Chunk::SIZE);
    ::int32_t const x_index((x - rel_x) / ::Chunk::SIZE), y_index((y - rel_y) / ::Chunk::SIZE);
    if (x_index < -this->left_offset || x_index >= this->right_offset || y_index < -this->front_offset || y_index >= this->back_offset) {
        return &::Sseu::BlockSource::null_block_inst;
    }
    ::BlockInstance ***block_map{this->origin[x_index][y_index].block_map};
    if (block_map == nullptr) {
        return &::Sseu::BlockSource::null_block_inst;
    }
    return block_map[::uint64_t(x) % ::Chunk::SIZE][::uint64_t(y) % ::Chunk::SIZE] + z;
}
void ChunkMap::generate_chunk_at(int64_t const x_index, int64_t const y_index) {
    Chunk *trunk{this->origin[x_index] + y_index};
    trunk->block_map = new BlockInstance **[Chunk::SIZE];
    for (uint32_t i{0U}; i < Chunk::SIZE; ++i) {
        trunk->block_map[i] = new BlockInstance *[Chunk::SIZE];
        for (uint32_t j{0U}; j < Chunk::SIZE; ++j) {
            trunk->block_map[i][j] = new BlockInstance[Chunk::SIZE];
            for (uint32_t k{0U}; k < Chunk::SIZE; ++k) {
                BlockInstance *block_inst{trunk->block_map[i][j] + k};
                block_inst->block = ::Sseu::BlockSource::block_enum[BBT_AIR];
                block_inst->coord[0] = x_index * Chunk::SIZE + i;
                block_inst->coord[1] = y_index * Chunk::SIZE + j;
                block_inst->coord[2] = k;
            }
        }
    }
}
void ChunkMap::link_chunk_at(int64_t const x_index, int64_t const y_index) {
    Chunk *trunk{this->origin[x_index] + y_index};
    for (int32_t i{0}; i < Chunk::SIZE; ++i) {
        for (int32_t j{0}; j < Chunk::SIZE; ++j) {
            for (int32_t k{0}; k < Chunk::SIZE; ++k) {
                int64_t x{x_index * Chunk::SIZE + i}, y{y_index * Chunk::SIZE + j};
                BlockInstance **linked_block_list{trunk->block_map[i][j][k].linked_block_list};
                linked_block_list[0] = this->get_block_inst(x, y, k - 1);
                linked_block_list[1] = this->get_block_inst(x, y, k + 1);
                linked_block_list[2] = this->get_block_inst(x, y - 1, k);
                linked_block_list[3] = this->get_block_inst(x, y + 1, k);
                linked_block_list[4] = this->get_block_inst(x - 1, y, k);
                linked_block_list[5] = this->get_block_inst(x + 1, y, k);
            }
        }
    }
}

Entity::Entity(::ChunkMap *chunk_map, ::double_t const x, ::double_t const y, ::double_t const z, ::double_t const box_radius, ::double_t const box_height): chunk_map{chunk_map}, x{x}, y{y}, z{z}, dx{0.0}, dy{0.0}, dz{0.0}, box_radius{box_radius}, box_height{box_height}, is_existing{true} { }
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
            this->update_vertical_motion(::std::min(this->dz * calculagraph.diff, 0.25));
            this->update_horizon_motion(::std::min(this->dx * calculagraph.diff, 0.25), ::std::min(this->dy * calculagraph.diff, 0.25));
            ::std::this_thread::sleep_for(::std::chrono::milliseconds(16));
        }
    });
    this->update_thrs.back().detach();
}
void Entity::display() const {
    ::glColor3ub(0xff, 0x00, 0x00);
    ::glBegin(GL_LINE_LOOP);
    for (::double_t theta{0.0}; theta < M_PI * 2; theta += M_PI / 12) {
        ::glVertex3f(x + this->box_radius * ::sin(theta), y + this->box_radius * ::cos(theta), z + box_height);
    }
    ::glEnd();

    ::glBegin(GL_LINES);
    ::glVertex3f(x, y, z + box_height);
    ::glColor3ub(0x00, 0x00, 0xff);
    ::glVertex3f(x, y, z);
    ::glEnd();

    ::glBegin(GL_LINE_LOOP);
    for (::double_t theta{0.0}; theta < M_PI * 2; theta += M_PI / 12) {
        ::glVertex3f(x + this->box_radius * ::sin(theta), y + this->box_radius * ::cos(theta), z);
    }
    ::glEnd();
}
void Entity::update_horizon_motion(::double_t const dx, ::double_t const dy) {
    ::double_t const box_left{this->x - this->box_radius + dx};
    ::double_t const box_right{this->x + this->box_radius + dx};
    ::double_t const box_front{this->y - this->box_radius + dy};
    ::double_t const box_back{this->y + this->box_radius + dy};
    ::double_t const box_top{this->z + this->box_height};
    ::int64_t const x_floor(::std::floor(this->x + dx));
    ::int64_t const y_floor(::std::floor(this->y + dy));
    ::int64_t const z_floor(::std::floor(this->z));
    ::int64_t const box_left_floor(::std::floor(box_left));
    ::int64_t const box_right_floor(::std::floor(box_right));
    ::int64_t const box_front_floor(::std::floor(box_front));
    ::int64_t const box_back_floor(::std::floor(box_back));
    ::int64_t const box_top_floor(::std::floor(box_top));
    ::double_t left_xmin{::std::max(
        this->chunk_map->get_block_inst(box_left_floor, y_floor, z_floor)->block->get_right_border(this->z - z_floor),
        this->chunk_map->get_block_inst(box_left_floor, y_floor, box_top)->block->get_right_border(box_top - box_top_floor)
    ) + box_left_floor};
    ::double_t right_xmax{::std::min(
        this->chunk_map->get_block_inst(box_right_floor, y_floor, z_floor)->block->get_left_border(this->z - z_floor),
        this->chunk_map->get_block_inst(box_right_floor, y_floor, box_top)->block->get_left_border(box_top - box_top_floor)
    ) + box_right_floor};
    ::double_t front_ymin{::std::max(
        this->chunk_map->get_block_inst(x_floor, box_front_floor, z_floor)->block->get_back_border(this->z - z_floor),
        this->chunk_map->get_block_inst(x_floor, box_front_floor, box_top)->block->get_back_border(box_top - box_top_floor)
    ) + box_front_floor};
    ::double_t back_ymax{::std::min(
        this->chunk_map->get_block_inst(x_floor, box_back_floor, z_floor)->block->get_front_border(this->z - z_floor),
        this->chunk_map->get_block_inst(x_floor, box_back_floor, box_top)->block->get_front_border(box_top - box_top_floor)
    ) + box_back_floor};
    if (box_left + dx < left_xmin) {
        this->dx = ::std::max(this->dx, 0.0);
    }
    if (box_right > right_xmax) {
        this->dx = ::std::min(this->dx, 0.0);
    }
    if (box_front < front_ymin) {
        this->dy = ::std::max(this->dy, 0.0);
    }
    if (box_back > back_ymax) {
        this->dy = ::std::min(this->dy, 0.0);
    }
    if (this->dx != 0.0) {
        this->x += dx;
    }
    if (this->dy != 0.0) {
        this->y += dy;
    }
                                                            ::__TEST__::xmin = left_xmin;
                                                            ::__TEST__::xmax = right_xmax;
                                                            ::__TEST__::ymin = front_ymin;
                                                            ::__TEST__::ymax = back_ymax;
}
void Entity::update_vertical_motion(::double_t const dz) {
    ::double_t const box_left{this->x - this->box_radius};
    ::double_t const box_right{this->x + this->box_radius};
    ::double_t const box_front{this->y - this->box_radius};
    ::double_t const box_back{this->y + this->box_radius};
    ::double_t const box_top{this->z + this->box_height + dz};
    ::int64_t const x_floor(::std::floor(this->x));
    ::int64_t const y_floor(::std::floor(this->y));
    ::int64_t const z_floor(::std::floor(this->z + dz));
    ::int64_t const box_left_floor(::std::floor(box_left));
    ::int64_t const box_right_floor(::std::floor(box_right));
    ::int64_t const box_front_floor(::std::floor(box_front));
    ::int64_t const box_back_floor(::std::floor(box_back));
    ::int64_t const box_top_floor(::std::floor(box_top));
    ::double_t bottom_zmin{::std::max({
        this->chunk_map->get_block_inst(x_floor, y_floor, z_floor)->block->get_top_border(this->x - x_floor, this->y - y_floor),
        this->chunk_map->get_block_inst(x_floor, y_floor, z_floor)->block->get_top_border(this->x - x_floor, this->y - y_floor),
        this->chunk_map->get_block_inst(box_left_floor, y_floor, z_floor)->block->get_top_border(box_left - box_left_floor, this->y - y_floor),
        this->chunk_map->get_block_inst(box_right_floor, y_floor, z_floor)->block->get_top_border(box_right - box_right_floor, this->y - y_floor),
        this->chunk_map->get_block_inst(x_floor, box_front_floor, z_floor)->block->get_top_border(this->x - x_floor, box_front - box_front_floor),
        this->chunk_map->get_block_inst(x_floor, box_back_floor, z_floor)->block->get_top_border(this->x - x_floor, box_back - box_back_floor)
    }) + z_floor};
    ::double_t top_zmax{::std::min({
        this->chunk_map->get_block_inst(x_floor, y_floor, box_top + dz)->block->get_bottom_border(this->x - x_floor, this->y - y_floor),
        this->chunk_map->get_block_inst(box_left_floor, y_floor, box_top + dz)->block->get_bottom_border(box_left - box_left_floor, this->y - y_floor),
        this->chunk_map->get_block_inst(box_right_floor, y_floor, box_top + dz)->block->get_bottom_border(box_right - box_right_floor, this->y - y_floor),
        this->chunk_map->get_block_inst(x_floor, box_front_floor, box_top + dz)->block->get_bottom_border(this->x - x_floor, box_front - box_front_floor),
        this->chunk_map->get_block_inst(x_floor, box_back_floor, box_top + dz)->block->get_bottom_border(this->x - x_floor, box_back - box_back_floor)
    }) + box_top};
    if (this->z + dz < bottom_zmin) {
        this->dz = ::std::max(this->dz, 0.0);
    }
    if (box_top + dz > top_zmax) {
        this->dz = ::std::min(this->dz, 0.0);
    }
    if (this->dz != 0.0) {
        this->z += dz;
    }
}

CameraEntity::CameraEntity(::ChunkMap *chunk_map, ::Camera *camera, ::double_t const x, ::double_t const y, ::double_t const z, ::double_t const theta, ::double_t const phi, ::double_t const box_radius, ::double_t const box_height): Entity(chunk_map, x, y, z, box_radius, box_height), camera{camera}, phi{phi}, theta{theta}, holding_block{::Sseu::BlockSource::block_enum[BBT_AIR]}, is_updating{true} { }
void CameraEntity::start_thrs() {
    this->update_thrs.emplace_back([this]() -> void {
        ::Calculagraph calculagraph, click_counter;
        ::MouseControl mouse_control;
        calculagraph.set();
        click_counter.set();
        mouse_control.set();
        ::SDL_SetRelativeMouseMode(SDL_TRUE);
        while (this->is_existing) {
            calculagraph.get(4e-9)->set();
            if (this->is_updating) {
                ::double_t const diff_cos_th{calculagraph.diff * ::cos(this->theta) * 2}, diff_sin_th{calculagraph.diff * ::sin(this->theta) * 2};
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
                    this->box_height = 1.75;
                }
                this->update_vertical_motion(::std::min(this->dz * calculagraph.diff, 0.25));
                this->update_horizon_motion(::std::min(this->dx * calculagraph.diff, 0.25), ::std::min(this->dy * calculagraph.diff, 0.25));
                this->control(&mouse_control);
                if (mouse_control.x <= 1 || mouse_control.x >= ::Sseu::window_width - 1 || mouse_control.y <= 0 || mouse_control.y >= ::Sseu::window_height - 1) {
                    ::SDL_WarpMouseInWindow(::Sseu::window, ::Sseu::window_width / 2, ::Sseu::window_height / 2);
                    mouse_control.set();
                }
                if (mouse_control.down_diff & 0x5) {
                    if (mouse_control.down_diff & 0x1) {
                        this->break_block();
                    }
                    if (mouse_control.down_diff & 0x4) {
                        this->place_block();
                    }
                }
            }
            ::std::this_thread::sleep_for(::std::chrono::milliseconds(16));
        }
        ::SDL_SetRelativeMouseMode(SDL_FALSE);
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
void CameraEntity::control(::MouseControl *mouse_control) {
    ::double_t new_phi{this->phi + mouse_control->y_diff * 0.01};
    if (new_phi >= 0 && new_phi < M_PI) {
        this->phi = new_phi;
    }
    this->theta += mouse_control->x_diff * 0.01;
    mouse_control->get()->set();
}
void CameraEntity::place_block() {
    ::int64_t place_x, place_y, place_z;
    if (this->phi < M_PI_4) {
        place_x = this->camera->selected_block->coord[0];
        place_y = this->camera->selected_block->coord[1];
        place_z = this->camera->selected_block->coord[2] - 1;
    } else if (this->phi < M_PI_4 * 3) {
        switch (::uint8_t(::std::floor(this->theta * M_1_PI * 2 + 0.5)) & 0x3) {
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
    ::BlockInstance *placed_block{reinterpret_cast<::MainActivity *>(::Sseu::activity_stack.top())->chunk_map.get_block_inst(place_x, place_y, place_z)};
    if (placed_block->block->interaction_level == 0x00 && placed_block != &::Sseu::BlockSource::null_block_inst) {
        placed_block->block = this->holding_block;
        ::Sseu::activity_stack.top<::MainActivity>()->event_deque.emplace_back(::GameEvent{ET_PLACE, this, placed_block});
    }
}
void CameraEntity::break_block() {
    ::Sseu::activity_stack.top<::MainActivity>()->event_deque.emplace_back(::GameEvent{ET_BREAK, this, this->camera->selected_block});
}

MainActivity::MainActivity():
    is_existing{true}, chunk_map(1LL, 2LL, 1LL, 2LL), camera_entity(&this->chunk_map, &this->camera, 2.5, 2.5, 16.0, 0.0, M_PI_2, 0.25, 1.75),
    screen_interface{::ScreenInterfaceType::GAME}, package_screen_interface(&this->camera_entity, {BT_TEST_BLOCK, BT_BLACK_BLOCK, BT_WHITE_BLOCK, BT_RED_STONE_POWDER, BT_RED_STONE_BLOCK}, {
        ::Sseu::BlockSource::test_block_btx.data[0],
        ::Sseu::BlockSource::black_block_btx.data[0],
        ::Sseu::BlockSource::white_block_btx.data[0],
        ::Sseu::BlockSource::red_stone_powder_btx_prune.data[0],
        ::Sseu::BlockSource::red_stone_block_btx.data[0]
    })
{
    glEnable(GL_BLEND);
    glDepthFunc(GL_LESS);
    glLineWidth(2.0F);

    this->camera_entity.start_thrs();

    this->__TEST_TERRAIN__();

    this->start_thrs();
}
MainActivity::~MainActivity() {
    this->is_existing = false;
    for (::Entity *entity : this->entity_list) {
        delete entity;
    }
}
void MainActivity::init() { }
void MainActivity::update() {
    this->start_world_painting();
    this->camera_entity.hold_camera();
    this->camera.look();

    ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    this->clear_background();
    ::glEnable(GL_DEPTH_TEST);
    this->world_display();
    this->camera_entity.display();
    this->camera.select_block();
                                                            ::glBegin(GL_LINE_LOOP);
                                                            ::glColor3ub(0x00, 0xff, 0x00);
                                                            ::glVertex3f(__TEST__::xmin, __TEST__::ymin, 1.01);
                                                            ::glVertex3f(__TEST__::xmin, __TEST__::ymax, 1.01);
                                                            ::glVertex3f(__TEST__::xmax, __TEST__::ymax, 1.01);
                                                            ::glVertex3f(__TEST__::xmax, __TEST__::ymin, 1.01);
                                                            ::glEnd();
    ::glDisable(GL_DEPTH_TEST);
    this->start_screen_painting();
    this->screen_display();

    this->camera_entity.is_jumping = ::GetAsyncKeyState(VK_SPACE);
    this->camera_entity.is_crouching = ::GetAsyncKeyState(VK_SHIFT);
    this->camera_entity.is_walking_lati = ::GetAsyncKeyState('A') ? -0x01 : ::GetAsyncKeyState('D') ? 0x01 : 0x0;
    this->camera_entity.is_walking_longi = ::GetAsyncKeyState('S') ? -0x01 : ::GetAsyncKeyState('W') ? 0x01 : 0x0;
    if (::GetAsyncKeyState('0')) {
        this->camera_entity.holding_block = ::Sseu::BlockSource::block_enum[BBT_AIR];
        ::std::cout << "air\n";
    }
    if (::GetAsyncKeyState('1')) {
        this->camera_entity.holding_block = ::Sseu::BlockSource::block_enum[BBT_TEST_BLOCK];
        ::std::cout << "test_block\n";
    }
    if (::GetAsyncKeyState('2')) {
        this->camera_entity.holding_block = ::Sseu::BlockSource::block_enum[BBT_BLACK_BLOCK];
        ::std::cout << "black_powder\n";
    }
    if (::GetAsyncKeyState('3')) {
        this->camera_entity.holding_block = ::Sseu::BlockSource::block_enum[BBT_WHITE_BLOCK];
        ::std::cout << "white_powder\n";
    }
    if (::GetAsyncKeyState('4')) {
        this->camera_entity.holding_block = ::Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER];
        ::std::cout << "red_stone_powder\n";
    }
    if (::GetAsyncKeyState('5')) {
        this->camera_entity.holding_block = ::Sseu::BlockSource::block_enum[BBT_RED_STONE_BLOCK];
        ::std::cout << "red_stone_block\n";
    }
    if (!__TEST__::key_E_state && ::GetAsyncKeyState('E')) {
        if (this->screen_interface == ::ScreenInterfaceType::GAME) {
            ::SDL_SetRelativeMouseMode(SDL_FALSE);
            this->camera_entity.is_updating = false;
            this->screen_interface = ::ScreenInterfaceType::PACKAGE;
        } else {
            ::SDL_SetRelativeMouseMode(SDL_TRUE);
            this->camera_entity.is_updating = true;
            this->screen_interface = ::ScreenInterfaceType::GAME;
        }
    }
    __TEST__::key_E_state = ::GetAsyncKeyState('E');
    if (::GetAsyncKeyState(VK_ESCAPE)) {
        ::Sseu::activity_stack.pop();
    }

    SDL_GL_SwapWindow(::Sseu::window);
    ++this->frame_rate_counter.fps;
}
void MainActivity::start_thrs() {
    (this->event_thr = std::thread([this]() -> void {
        while (this->is_existing) {
            while (!this->event_deque.empty()) {
                this->event_func();
                this->event_deque.pop_front();
            }
        }
    })).detach();
    (this->filter_thr = ::std::thread([this]() -> void {
        while (this->is_existing) {
            this->filter_imvisible_blocks();
        }
    })).detach();
}
void MainActivity::clear_background() const {
    glClearColor(0.125F, 0.125F, 0.125F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void MainActivity::start_world_painting() const {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1875, 0.1875, -0.125, 0.125, 0.125, 1024.0);
}
void MainActivity::start_screen_painting() const {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-::Sseu::window_width * 0.5, ::Sseu::window_width * 0.5, -::Sseu::window_height * 0.5, ::Sseu::window_height * 0.5, 0.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F);
}
// template<typename... Args> using v = ::std::vector<Args...>;
void MainActivity::filter_imvisible_blocks() {
                                                                // ::std::array<::double_t, 3ULL> aLd_3R;
                                                                // ::std::vector<::std::array<::double_t, 3ULL>> vLaLd_3RR;
                                                                // ::std::vector<::int32_t> vLiR;
                                                                // ::std::vector<::std::pair<::std::array<::double_t, 3ULL>, ::int32_t>> vLpLaLd_3RiRR;
    // ::int64_t coord[3];
    // // ::std::vector<BlockInstance *> temp_visible_block_list;
    // ::std::stack<::std::unordered_set<::BlockInstance *>> temp_visible_block_stack;
    // temp_visible_block_stack.emplace();
    ::std::multimap<::double_t, ::BlockInstance *> temp_visible_block_pair;
    ::std::vector<::BlockInstance *> temp_visible_block_list;
    for (::uint32_t i{0U}; i < ::Chunk::SIZE; ++i) {
        for (::uint32_t j{0U}; j < ::Chunk::SIZE; ++j) {
            for (::uint32_t k{0U}; k < ::Chunk::SIZE; ++k) {
                BlockInstance *block_inst{this->chunk_map.origin[0][0].block_map[i][j] + k};
                if (block_inst->block->interaction_level >= 0x01) {
                    temp_visible_block_pair.emplace(i+j+k, block_inst);
                }
            }
        }
    }
    for (::std::multimap<::double_t, ::BlockInstance *>::iterator visible_block{temp_visible_block_pair.begin()}; visible_block != temp_visible_block_pair.end(); ++visible_block) {
        temp_visible_block_list.emplace_back(visible_block->second);
    }
    this->visible_block_list_mutex.lock();
    this->visible_block_list.swap(temp_visible_block_list);
    this->visible_block_list_mutex.unlock();
}
void MainActivity::world_display() {
    ::glEnable(GL_TEXTURE_2D);
    this->visible_block_list_mutex.lock();
                                                                static ::uint64_t __TEST_M__{1ULL};
                                                                ::uint64_t __TEST_C__{0ULL};
    for (::BlockInstance *visible_block : this->visible_block_list) {
        visible_block->block->display(visible_block->coord[0], visible_block->coord[1], visible_block->coord[2]);
                                                                if(++__TEST_C__>__TEST_M__){__TEST_M__+=this->random.rand_range(16U);break;}
    }
    this->visible_block_list_mutex.unlock();
    ::glDisable(GL_TEXTURE_2D);
}
void MainActivity::screen_display() {
    switch (this->screen_interface) {
        case ::ScreenInterfaceType::GAME: {
            ::glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
            ::glColor3ub(0xff, 0xff, 0xff);
            ::glBegin(GL_LINES);
            ::glVertex2i(-16, 0);
            ::glVertex2i(16, 0);
            ::glVertex2i(0, -16);
            ::glVertex2i(0, 16);
            ::glEnd();
            break;
        }
        case ::ScreenInterfaceType::PACKAGE: {
            this->package_screen_interface.update();
            break;
        }
    }

    ::glEnable(GL_TEXTURE_2D);
    // ::glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ZERO);
    ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // ::glColor3ub(0x80, 0x00, 0xff);
    if (this->camera.selected_block->rs_signal_strength != 0) {
        ::glColor3ub(0x00, 0x80, 0x80);
        ::glPushMatrix();
        ::std::ostringstream rs_signal_strength_text;
        rs_signal_strength_text << this->camera.selected_block->rs_signal_strength / 16.0;
        ::glTranslatef(rs_signal_strength_text.str().length() * -9.0F, 18.0F, 0.0F);
        ::Sseu::FontSource::draw_text(rs_signal_strength_text.str().c_str(), 18.0);
        ::glPopMatrix();
    }
    ::glColor3ub(0x00, 0x00, 0x00);
    ::glPushMatrix();
    std::string const *block_name{&::Sseu::BlockSource::block_name[this->camera_entity.holding_block]};
    ::glTranslatef(0.0F, -::Sseu::window_height * 0.5, 0.0F);
    ::Sseu::FontSource::draw_text(block_name->c_str(), 9.0);
    ::glPopMatrix();
    ::glDisable(GL_TEXTURE_2D);
}
void MainActivity::event_func() {
    GameEvent *event{&this->event_deque.front()};
    if (event->block_inst != &::Sseu::BlockSource::null_block_inst) {
        switch (event->type) {
            case ET_PLACE: {
                switch (event->block_inst->block->block_type) {
                    case BT_RED_STONE_BLOCK: {
                        event->block_inst->rs_signal_strength = 16;
                        for (BlockInstance **linked_block{event->block_inst->linked_block_list}; linked_block - event->block_inst->linked_block_list < 6; ++linked_block) {
                            if (*linked_block != nullptr && (*linked_block)->block->block_type == BT_RED_STONE_POWDER) {
                                this->activate_rs_circuit(*linked_block);
                            }
                        }
                        break;
                    }
                    case BT_RED_STONE_POWDER: {
                        this->activate_rs_circuit(event->block_inst);
                        break;
                    }
                }
// __TEST__::info(event->block_inst);
// __TEST__::info(event->block_inst->linked_block_list[4]);
// __TEST__::info(event->block_inst->linked_block_list[5]);
                for (BlockInstance **linked_block{event->block_inst->linked_block_list}; linked_block - event->block_inst->linked_block_list < 6; ++linked_block) {
                    if (*linked_block != nullptr) {
                        this->refresh_block(*linked_block);
                    }
                }
                this->refresh_block(event->block_inst);
                break;
            }
            case ET_BREAK: {
                BlockBase *broken_block{event->block_inst->block};
                event->block_inst->block = ::Sseu::BlockSource::block_enum[BBT_AIR];
                switch (broken_block->block_type) {
                    case BT_RED_STONE_BLOCK: case BT_RED_STONE_POWDER: {
                        ::std::vector<BlockInstance *> rs_sources;
                        event->block_inst->rs_signal_strength = 0;
                        for (BlockInstance **linked_block{event->block_inst->linked_block_list}; linked_block - event->block_inst->linked_block_list < 6; ++linked_block) {
                            this->deactivate_rs_circuit(*linked_block, &rs_sources);
                        }
                        for (BlockInstance *rs_source : rs_sources) {
                            this->activate_rs_circuit(rs_source);
                        }
                        break;
                    }
                }
                for (BlockInstance **linked_block{event->block_inst->linked_block_list}; linked_block - event->block_inst->linked_block_list < 6; ++linked_block) {
                    if (*linked_block != nullptr) {
                        this->refresh_block(*linked_block);
                    }
                }
                break;
            }
        }
    }
}
void MainActivity::refresh_block(BlockInstance *block_inst) {
    switch (block_inst->block->block_type) {
        case BT_RED_STONE_POWDER: {
            uint8_t linking_state{0x00}, mask{0x08};
            for (BlockInstance **linked_block{block_inst->linked_block_list + 2}; linked_block - block_inst->linked_block_list < 6; ++linked_block) {
                if (*linked_block != nullptr) {
                    if ((*linked_block)->block->block_type == BT_RED_STONE_POWDER) {
                        linking_state |= mask;
                    }
                }
                mask >>= 1;
            }
            switch (linking_state) {
                case 0x00: {
                    block_inst->block = block_inst->rs_signal_strength != 0U ? ::Sseu::BlockSource::block_enum[BBT_ACTIVE_RED_STONE_POWDER] : ::Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER];
                    break;
                }
                case 0x01 ... 0x03: {
                    block_inst->block = block_inst->rs_signal_strength != 0U ? ::Sseu::BlockSource::block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_LR] : ::Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_LR];
                    break;
                }
                case 0x04: case 0x08: case 0x0c: {
                    block_inst->block = block_inst->rs_signal_strength != 0U ? ::Sseu::BlockSource::block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_FB] : ::Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_FB];
                    break;
                }
                case 0x05: {
                    block_inst->block = block_inst->rs_signal_strength != 0U ? ::Sseu::BlockSource::block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_BR] : ::Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_BR];
                    break;
                }
                case 0x06: {
                    block_inst->block = block_inst->rs_signal_strength != 0U ? ::Sseu::BlockSource::block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_BL] : ::Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_BL];
                    break;
                }
                case 0x07: {
                    block_inst->block = block_inst->rs_signal_strength != 0U ? ::Sseu::BlockSource::block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_BLR] : ::Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_BLR];
                    break;
                }
                case 0x09: {
                    block_inst->block = block_inst->rs_signal_strength != 0U ? ::Sseu::BlockSource::block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_FR] : ::Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_FR];
                    break;
                }
                case 0x0a: {
                    block_inst->block = block_inst->rs_signal_strength != 0U ? ::Sseu::BlockSource::block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_FL] : ::Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_FL];
                    break;
                }
                case 0x0b: {
                    block_inst->block = block_inst->rs_signal_strength != 0U ? ::Sseu::BlockSource::block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_FLR] : ::Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_FLR];
                    break;
                }
                case 0x0d: {
                    block_inst->block = block_inst->rs_signal_strength != 0U ? ::Sseu::BlockSource::block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBR] : ::Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_FBR];
                    break;
                }
                case 0x0e: {
                    block_inst->block = block_inst->rs_signal_strength != 0U ? ::Sseu::BlockSource::block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBL] : ::Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_FBL];
                    break;
                }
                case 0x0f: {
                    block_inst->block = block_inst->rs_signal_strength != 0U ? ::Sseu::BlockSource::block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBLR] : ::Sseu::BlockSource::block_enum[BBT_RED_STONE_POWDER_JOINT_FBLR];
                    break;
                }
            }
            break;
        }
    }
}
void MainActivity::activate_rs_circuit(BlockInstance *start) {
    if (start->block->block_type == BT_RED_STONE_POWDER) {
        for (BlockInstance **linked_block{start->linked_block_list}; linked_block - start->linked_block_list < 6; ++linked_block) {
            if ((*linked_block)->rs_signal_strength > start->rs_signal_strength + 1) {
                start->rs_signal_strength = (*linked_block)->rs_signal_strength - 1;
            }
        }
        this->refresh_block(start);
        for (BlockInstance **linked_block{start->linked_block_list}; linked_block - start->linked_block_list < 6; ++linked_block) {
            if ((*linked_block)->block->block_type == BT_RED_STONE_POWDER && (*linked_block)->rs_signal_strength < start->rs_signal_strength - 1) {
                this->activate_rs_circuit(*linked_block);
            }
        }
    }
}
void MainActivity::deactivate_rs_circuit(BlockInstance *start, ::std::vector<BlockInstance *> *rs_sources) {
    if (start->block->block_type == BT_RED_STONE_POWDER) {
        start->rs_signal_strength = 0;
        this->refresh_block(start);
        for (BlockInstance **linked_block{start->linked_block_list}; linked_block - start->linked_block_list < 6; ++linked_block) {
            if ((*linked_block)->block->block_type == BT_RED_STONE_BLOCK) {
                rs_sources->emplace_back(start);
            }
            if ((*linked_block)->block->block_type == BT_RED_STONE_POWDER && (*linked_block)->rs_signal_strength != 0) {
                deactivate_rs_circuit(*linked_block, rs_sources);
            }
        }
    }
}
