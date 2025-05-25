#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Calculagraph.hpp"
#include "ChunkMap.hpp"

struct GameEvent;

struct Entity{
    ::ChunkMap *chunk_map;
    ::double_t x, y, z;
    ::double_t dx, dy, dz;
    ::double_t box_radius, box_height;
    bool is_existing;
    ::std::vector<::std::thread> update_thrs;
    inline Entity(::ChunkMap *, ::double_t const, ::double_t const, ::double_t const, ::double_t const, ::double_t const);
    Entity(Entity const &) = delete;
    Entity(Entity &&) = delete;
    ~Entity();
    inline virtual void start_thrs();
    inline void display() const;
    inline void update_vertical_motion(::double_t const);
    inline void update_horizon_motion(::double_t const, ::double_t const);
};

struct CameraEntity : Entity{
    ::Camera *camera;
    ::std::deque<GameEvent> *event_deque;
    ::double_t phi, theta;
    ::int8_t is_walking_lati, is_walking_longi;
    bool is_jumping, is_crouching;
    ::BlockBase *holding_block;
    bool is_updating;
    using double_t = void;
    inline CameraEntity(::ChunkMap *, ::Camera *, ::std::deque<GameEvent> *, ::double_t const, ::double_t const, ::double_t const, ::double_t const, ::double_t const, ::double_t const, ::double_t const);
    inline void start_thrs() override;
    inline void hold_camera();
    inline void control(::MouseControl *);
    inline void place_block();
    inline void break_block();
};

#endif
