#ifndef GAME_EVENT_HPP
#define GAME_EVENT_HPP

#include "Entity.hpp"

enum GameEventType{
    ET_PLACE,
    ET_BREAK
};

struct GameEvent{
    GameEventType type;
    CameraEntity *entity;
    BlockInstance *block_inst;
};

#endif
