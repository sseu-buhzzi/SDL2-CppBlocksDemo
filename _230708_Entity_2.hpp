#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Calculagraph.hpp"
#include "Trunk.hpp"

struct Entity{
    double_t x, y, z;
    double_t dx, dy, dz;
    double_t box_radius, box_height;
    Trunk *trunk_appended;
    bool is_existing;
    std::thread update_thr;
    Entity(Trunk *trunk_appended): dx{0.0}, dy{0.0}, dz{0.0}, trunk_appended{trunk_appended}, is_existing{true}, update_thr([this]() -> void {
        Calculagraph calculagraph;
        calculagraph.set();
// std::cout<<this->dz<<'\t';
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
// std::cout<<this->dz<<'\n';
        double_t rel_box_front, rel_box_back, rel_box_left, rel_box_right, box_top, new_x, new_y, new_z, rel_x, rel_y, rel_new_x, rel_new_y;
        while (this->is_existing) {
            calculagraph.get(4e-9)->set();
            this->dz -= calculagraph.diff;

            rel_box_front = this->y - this->box_radius - this->trunk_appended->y;
            rel_box_back = this->y + this->box_radius - this->trunk_appended->y;
            rel_box_left = this->x - this->box_radius - this->trunk_appended->x;
            rel_box_right = this->x + this->box_radius - this->trunk_appended->x;
            box_top = this->z + this->box_height;
            new_x = this->x + std::min(this->dx * calculagraph.diff, 0.125);
            new_y = this->y + std::min(this->dy * calculagraph.diff, 0.125);
            new_z = this->z + std::min(this->dz * calculagraph.diff, 0.125);
            rel_x = this->x - this->trunk_appended->x;
            rel_y = this->y - this->trunk_appended->y;
            rel_new_x = new_x - this->trunk_appended->x;
            rel_new_y = new_y - this->trunk_appended->y;

// TCO<<int32_t(this->trunk_appended->get_block_info(this->x-this->trunk_appended->x,this->y-this->trunk_appended->y,this->z)->get_interaction_level())<<'\n';
// TCO<<this->trunk_appended->get_block_info(rel_new_x - this->box_radius, rel_y, this->z)->block;
            // double_t left_lower_edge{this->trunk_appended->get_block_info(rel_new_x - this->box_radius, rel_y, this->z)->block->get_right_edge(this->z - int32_t(this->z))};
            // double_t left_upper_edge{this->trunk_appended->get_block_info(rel_new_x - this->box_radius, rel_y, box_top)->block->get_right_edge(box_top - int32_t(box_top))};
            // double_t right_lower_edge{this->trunk_appended->get_block_info(rel_new_x + this->box_radius, rel_y, this->z)->block->get_left_edge(this->z - int32_t(this->z))};
            // double_t right_upper_edge{this->trunk_appended->get_block_info(rel_new_x + this->box_radius, rel_y, box_top)->block->get_left_edge(box_top - int32_t(box_top))};
            // double_t front_lower_edge{this->trunk_appended->get_block_info(rel_x, rel_new_y - this->box_radius, this->z)->block->get_back_edge(this->z - int32_t(this->z))};
            // double_t front_upper_edge{this->trunk_appended->get_block_info(rel_x, rel_new_y - this->box_radius, box_top)->block->get_back_edge(box_top - int32_t(box_top))};
            // double_t back_lower_edge{this->trunk_appended->get_block_info(rel_x, rel_new_y + this->box_radius, this->z)->block->get_front_edge(this->z - int32_t(this->z))};
            // double_t back_upper_edge{this->trunk_appended->get_block_info(rel_x, rel_new_y + this->box_radius, box_top)->block->get_front_edge(box_top - int32_t(box_top))};
            // double_t bottom_lower_edge{this->trunk_appended->get_block_info(rel_x, rel_y, new_z)->block->get_top_edge(rel_x - int32_t(rel_x), rel_y - int32_t(rel_y))};
            // double_t top_upper_edge{this->trunk_appended->get_block_info(rel_x, rel_y, new_z + this->box_height)->block->get_bottom_edge(rel_x - int32_t(rel_x), rel_y - int32_t(rel_y))};

            double_t left_xmin{std::max(
                this->trunk_appended->get_block_info(rel_new_x - this->box_radius, rel_y, this->z)->block->get_right_edge(this->z - int32_t(this->z)),
                this->trunk_appended->get_block_info(rel_new_x - this->box_radius, rel_y, box_top)->block->get_right_edge(box_top - int32_t(box_top))
            ) + int32_t(new_x - this->box_radius)};
            double_t right_xmax{std::min(
                this->trunk_appended->get_block_info(rel_new_x + this->box_radius, rel_y, this->z)->block->get_left_edge(this->z - int32_t(this->z)),
                this->trunk_appended->get_block_info(rel_new_x + this->box_radius, rel_y, box_top)->block->get_left_edge(box_top - int32_t(box_top))
            ) + int32_t(new_x + this->box_radius)};
            double_t front_ymin{std::max(
                this->trunk_appended->get_block_info(rel_x, rel_new_y - this->box_radius, this->z)->block->get_back_edge(this->z - int32_t(this->z)),
                this->trunk_appended->get_block_info(rel_x, rel_new_y - this->box_radius, box_top)->block->get_back_edge(box_top - int32_t(box_top))
            ) + int32_t(new_y - this->box_radius)};
            double_t back_ymax{std::min(
                this->trunk_appended->get_block_info(rel_x, rel_new_y + this->box_radius, this->z)->block->get_front_edge(this->z - int32_t(this->z)),
                this->trunk_appended->get_block_info(rel_x, rel_new_y + this->box_radius, box_top)->block->get_front_edge(box_top - int32_t(box_top))
            ) + int32_t(new_y + this->box_radius)};
            double_t bottom_zmin{std::max({
                this->trunk_appended->get_block_info(rel_x, rel_new_y + this->box_radius, new_z)->block->get_top_edge(rel_x - int32_t(rel_x), rel_y - int32_t(rel_y)),
                this->trunk_appended->get_block_info(rel_x, rel_y, new_z)->block->get_top_edge(rel_x - int32_t(rel_x), rel_y - int32_t(rel_y)),
                this->trunk_appended->get_block_info(rel_box_left, rel_y, new_z)->block->get_top_edge(rel_box_left - int32_t(rel_box_left), rel_y - int32_t(rel_y)),
                this->trunk_appended->get_block_info(rel_box_right, rel_y, new_z)->block->get_top_edge(rel_box_right - int32_t(rel_box_right), rel_y - int32_t(rel_y)),
                this->trunk_appended->get_block_info(rel_x, rel_box_front, new_z)->block->get_top_edge(rel_x - int32_t(rel_x), rel_box_front - int32_t(rel_box_front)),
                this->trunk_appended->get_block_info(rel_x, rel_box_back, new_z)->block->get_top_edge(rel_x - int32_t(rel_x), rel_box_back - int32_t(rel_box_back))
            }) + int32_t(new_z)};
            double_t top_zmax{std::min({
                this->trunk_appended->get_block_info(rel_x, rel_y, new_z + this->box_height)->block->get_bottom_edge(rel_x - int32_t(rel_x), rel_y - int32_t(rel_y)),
                this->trunk_appended->get_block_info(rel_box_left, rel_y, new_z + this->box_height)->block->get_bottom_edge(rel_box_left - int32_t(rel_box_left), rel_y - int32_t(rel_y)),
                this->trunk_appended->get_block_info(rel_box_right, rel_y, new_z + this->box_height)->block->get_bottom_edge(rel_box_right - int32_t(rel_box_right), rel_y - int32_t(rel_y)),
                this->trunk_appended->get_block_info(rel_x, rel_box_front, new_z + this->box_height)->block->get_bottom_edge(rel_x - int32_t(rel_x), rel_box_front - int32_t(rel_box_front)),
                this->trunk_appended->get_block_info(rel_x, rel_box_back, new_z + this->box_height)->block->get_bottom_edge(rel_x - int32_t(rel_x), rel_box_back - int32_t(rel_box_back))
            }) + int32_t(new_z + box_height)};
// TCO<<new_z<<'\t'<<bottom_zmin<<'\t'<<top_zmax<<'\n';
            if (new_z < bottom_zmin) {
                this->dz = 0.0;
                new_z = bottom_zmin;
            }
            if (new_z + this->box_height > top_zmax) {
                this->dz = 0.0;
                new_z = top_zmax - this->box_height;
            }
            if (new_x - this->box_radius < left_xmin) {
                this->dx = 0.0;
                new_x = left_xmin + this->box_radius;
            }
            if (new_x + this->box_radius > right_xmax) {
                this->dx = 0.0;
                new_x = right_xmax - this->box_radius;
            }
            if (new_y - this->box_radius < front_ymin) {
                this->dy = 0.0;
                new_y = front_ymin + this->box_radius;
            }
            if (new_y + this->box_radius > back_ymax) {
                this->dy = 0.0;
                new_y = back_ymax - this->box_radius;
            }

//             if (this->trunk_appended->get_block_info(rel_x - this->box_radius, rel_y, this->z)->get_interaction_level() >= 0x02 || this->trunk_appended->get_block_info(rel_x - this->box_radius, rel_y, box_top)->get_interaction_level() >= 0x02) {
//                 this->dx = 0.0;
//                 new_x = this->x + 256e-6;
//             }
//             if (this->trunk_appended->get_block_info(rel_x + this->box_radius, rel_y, this->z)->get_interaction_level() >= 0x02 || this->trunk_appended->get_block_info(rel_x + this->box_radius, rel_y, box_top)->get_interaction_level() >= 0x02) {
//                 this->dx = 0.0;
//                 new_x = this->x - 256e-6;
//             }
//             if (this->trunk_appended->get_block_info(rel_x, rel_y - this->box_radius, this->z)->get_interaction_level() >= 0x02 || this->trunk_appended->get_block_info(rel_x, rel_y - this->box_radius, box_top)->get_interaction_level() >= 0x02) {
//                 this->dy = 0.0;
//                 new_y = this->y + 256e-6;
//             }
//             if (this->trunk_appended->get_block_info(rel_x, rel_y + this->box_radius, this->z)->get_interaction_level() >= 0x02 || this->trunk_appended->get_block_info(rel_x, rel_y + this->box_radius, box_top)->get_interaction_level() >= 0x02) {
//                 this->dy = 0.0;
//                 new_y = this->y - 256e-6;
//             }
// // TCO<<int32_t(Sseu::BlockSource::white_block->interaction_level)<<'\n';
// // TCO<<int32_t(this->trunk_appended->get_block_info(rel_x, rel_y, new_z)->get_interaction_level())<<'\n';
//             if (this->trunk_appended->get_block_info(rel_x, rel_y, new_z)->get_interaction_level() >= 0x02) {
// // TCO<<'\n';
//                 this->dz = 0.0;
//                 new_z = this->z + 256e-6;
//             }
//             if (this->trunk_appended->get_block_info(rel_x, rel_y, new_z + box_height)->get_interaction_level() >= 0x02) {
//                 this->dz = 0.0;
//                 new_z = this->z - 256e-6;
//             }

            this->dx *= 0.875;
            this->dy *= 0.875;
            this->x = new_x;
            this->y = new_y;
            this->z = new_z;
// TCO<<Sseu::BlockSource::block_enum[this->trunk_appended->block_map[int32_t(this->x)][int32_t(this->y)][int32_t(this->z)].block]<<'\n';
// TCO<<calculagraph.diff<<'\t'<<this->dz<<'\t'<<this->z<<'\n';
// TCO<<calculagraph.diff<<'\n';/
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    }) { }
    Entity(const Entity &) = delete;
    Entity(Entity &&) = delete;
    ~Entity() {
        this->update_thr.join();
    }
    void destroy() {
        this->is_existing = false;
    }
    void display() {
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
    void hold_camera(Camera *camera) {
        camera->x = this->x;
        camera->y = this->y;
        camera->z = this->z + this->box_height;
    }
};

#endif
