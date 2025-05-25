#ifndef CHUNK_MAP_HPP
#define CHUNK_MAP_HPP

#include "Chunk.hpp"

struct ChunkMap{
	int64_t front_offset, back_offset, left_offset, right_offset;
	Chunk **origin;
	inline ChunkMap(int64_t const, int64_t const, int64_t const, int64_t const);
	~ChunkMap();
	inline BlockInstance *get_block_inst(int64_t const, int64_t const, uint32_t const) const;
	inline void generate_chunk_at(int64_t const, int64_t const);
	inline void link_chunk_at(int64_t const, int64_t const);
	inline void front_expand();
	inline void back_expand();
	inline void left_expand();
	inline void right_expand();
};

#endif
