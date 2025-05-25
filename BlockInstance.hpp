#ifndef BLOCK_INSTANCE_HPP
#define BLOCK_INSTANCE_HPP

#include "BlockBase.hpp"

struct BlockInstance{
	static ::int32_t const RS_SIGNAL_STRENGTH_MAX;
	bool not_refreshed;
	::int32_t rs_signal_strength;
	::BlockInstance *linked_block_list[6];
	::std::unordered_set<::uint8_t> link_in, link_out;
	::int64_t coord[3];
	::BlockBase *block;
	inline BlockInstance();
};

#endif
