#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "Camera.hpp"

struct Chunk{
    static uint32_t const SIZE;
    BlockInstance ***block_map;
    inline Chunk();
    ~Chunk();
    inline void display_flush() const;
    inline bool contains_point_at(int32_t const, int32_t const, int32_t const) const;
};

#endif
