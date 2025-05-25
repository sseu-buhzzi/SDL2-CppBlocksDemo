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
        bool is_colliding;
        double_t box_top, new_x, new_y, new_z, rel_x, rel_y;
        while (this->is_existing) {
            calculagraph.get(4e-9)->set();
            this->dz -= calculagraph.diff;

            is_colliding = this->trunk_appended->get_block_info(this->x - this->trunk_appended->x, this->y - this->trunk_appended->y, this->z);
            box_top = this->z + this->box_height;
            new_x = this->x + std::min(this->dx * calculagraph.diff, 0.125);
            new_y = this->y + std::min(this->dy * calculagraph.diff, 0.125);
            new_z = this->z + std::min(this->dz * calculagraph.diff, 0.125);
            rel_x = this->x - this->trunk_appended->x;
            rel_y = this->y - this->trunk_appended->y;

            if (this->trunk_appended->get_block_info(rel_x - this->box_radius, rel_y, this->z)->is_solid_block() || this->trunk_appended->get_block_info(rel_x - this->box_radius, rel_y, box_top)->is_solid_block()) {
                this->dx = 0.0;
                new_x = this->x;
                if (is_colliding) {
                    new_x += 0.001;
                }
            }
            if (this->trunk_appended->get_block_info(rel_x + this->box_radius, rel_y, this->z)->is_solid_block() || this->trunk_appended->get_block_info(rel_x + this->box_radius, rel_y, box_top)->is_solid_block()) {
                this->dx = 0.0;
                new_x = this->x;
                if (is_colliding) {
                    new_x -= 0.001;
                }
            }
            if (this->trunk_appended->get_block_info(rel_x, rel_y - this->box_radius, this->z)->is_solid_block() || this->trunk_appended->get_block_info(rel_x, rel_y - this->box_radius, box_top)->is_solid_block()) {
                this->dy = 0.0;
                new_y = this->y;
                if (is_colliding) {
                    new_y += 0.001;
                }
            }
            if (this->trunk_appended->get_block_info(rel_x, rel_y + this->box_radius, this->z)->is_solid_block() || this->trunk_appended->get_block_info(rel_x, rel_y + this->box_radius, box_top)->is_solid_block()) {
                this->dy = 0.0;
                new_y = this->y;
                if (is_colliding) {
                    new_y -= 0.001;
                }
            }
            if (this->trunk_appended->get_block_info(rel_x, rel_y, new_z)->is_solid_block()) {
                this->dz = 0.0;
                new_z = this->z;
                if (is_colliding) {
                    new_z += 0.001;
                }
            }
            if (this->trunk_appended->get_block_info(rel_x, rel_y, new_z + box_height)->is_solid_block()) {
                this->dz = 0.0;
                new_z = this->z;
                if (is_colliding) {
                    new_z -= 0.001;
                }
            }

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
        camera->z = this->z + this->box_height + 0.125;
    }
};

#endif
