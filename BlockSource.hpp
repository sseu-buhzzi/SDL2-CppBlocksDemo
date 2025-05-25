#ifndef BLOCK_SOURCE_HPP
#define BLOCK_SOURCE_HPP

#include "BlockInstance.hpp"
#include "./BlockSource/ActiveRedStoneLamp.hpp"
#include "./BlockSource/ActiveRedStonePowder.hpp"
#include "./BlockSource/ActiveRedStonePowderJointFb.hpp"
#include "./BlockSource/ActiveRedStonePowderJointLr.hpp"
#include "./BlockSource/ActiveRedStonePowderJointFl.hpp"
#include "./BlockSource/ActiveRedStonePowderJointFr.hpp"
#include "./BlockSource/ActiveRedStonePowderJointBl.hpp"
#include "./BlockSource/ActiveRedStonePowderJointBr.hpp"
#include "./BlockSource/ActiveRedStonePowderJointFbl.hpp"
#include "./BlockSource/ActiveRedStonePowderJointFbr.hpp"
#include "./BlockSource/ActiveRedStonePowderJointFlr.hpp"
#include "./BlockSource/ActiveRedStonePowderJointBlr.hpp"
#include "./BlockSource/ActiveRedStonePowderJointFblr.hpp"
#include "./BlockSource/ActiveRedStoneTorch.hpp"
#include "./BlockSource/BlackBlock.hpp"
#include "./BlockSource/RedStoneBlock.hpp"
#include "./BlockSource/RedStoneLamp.hpp"
#include "./BlockSource/RedStonePowder.hpp"
#include "./BlockSource/RedStonePowderJointFb.hpp"
#include "./BlockSource/RedStonePowderJointLr.hpp"
#include "./BlockSource/RedStonePowderJointFl.hpp"
#include "./BlockSource/RedStonePowderJointFr.hpp"
#include "./BlockSource/RedStonePowderJointBl.hpp"
#include "./BlockSource/RedStonePowderJointBr.hpp"
#include "./BlockSource/RedStonePowderJointFbl.hpp"
#include "./BlockSource/RedStonePowderJointFbr.hpp"
#include "./BlockSource/RedStonePowderJointFlr.hpp"
#include "./BlockSource/RedStonePowderJointBlr.hpp"
#include "./BlockSource/RedStonePowderJointFblr.hpp"
#include "./BlockSource/RedStoneTorch.hpp"
#include "./BlockSource/TestBlock.hpp"
#include "./BlockSource/WhiteBlock.hpp"

namespace Sseu::BlockSource {
	::BlockInstance null_block_inst;
	::std::unordered_map<::BlockBaseType, ::BlockBase *const> block_enum;
	::std::unordered_map<::BlockBase *, ::std::string const> block_name;

	void source_init() {
		::Sseu::BlockSource::block_enum = {
			{::BlockBaseType::BBT_AIR, new ::AirBlock},
			{::BlockBaseType::BBT_TEST_BLOCK, new ::CubicBlock(&test_block_btx, ::BlockType::BT_TEST_BLOCK, 0x04)},
			{::BlockBaseType::BBT_BLACK_BLOCK, new ::CubicBlock(&black_block_btx, ::BlockType::BT_BLACK_BLOCK, 0x04)},
			{::BlockBaseType::BBT_WHITE_BLOCK, new ::CubicBlock(&white_block_btx, ::BlockType::BT_WHITE_BLOCK, 0x04)},
			// {::BlockBaseType::BBT_RED_STONE_POWDER, new RedStonePowderBlock(&red_stone_powder_btx_prune, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_RED_STONE_POWDER, new ::PowderBlock(&red_stone_powder_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FB, new ::PowderBlock(&red_stone_powder_joint_fb_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_LR, new ::PowderBlock(&red_stone_powder_joint_lr_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FL, new ::PowderBlock(&red_stone_powder_joint_fl_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FR, new ::PowderBlock(&red_stone_powder_joint_fr_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_BL, new ::PowderBlock(&red_stone_powder_joint_bl_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_BR, new ::PowderBlock(&red_stone_powder_joint_br_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FBL, new ::PowderBlock(&red_stone_powder_joint_fbl_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FBR, new ::PowderBlock(&red_stone_powder_joint_fbr_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FLR, new ::PowderBlock(&red_stone_powder_joint_flr_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_BLR, new ::PowderBlock(&red_stone_powder_joint_blr_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FBLR, new ::PowderBlock(&red_stone_powder_joint_fblr_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			// {::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER, new RedStonePowderBlock(&active_red_stone_powder_btx_prune, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER, new ::PowderBlock(&active_red_stone_powder_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_FB, new ::PowderBlock(&active_red_stone_powder_joint_fb_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_LR, new ::PowderBlock(&active_red_stone_powder_joint_lr_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_FL, new ::PowderBlock(&active_red_stone_powder_joint_fl_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_FR, new ::PowderBlock(&active_red_stone_powder_joint_fr_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_BL, new ::PowderBlock(&active_red_stone_powder_joint_bl_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_BR, new ::PowderBlock(&active_red_stone_powder_joint_br_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBL, new ::PowderBlock(&active_red_stone_powder_joint_fbl_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBR, new ::PowderBlock(&active_red_stone_powder_joint_fbr_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_FLR, new ::PowderBlock(&active_red_stone_powder_joint_flr_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_BLR, new ::PowderBlock(&active_red_stone_powder_joint_blr_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBLR, new ::PowderBlock(&active_red_stone_powder_joint_fblr_btx, ::BlockType::BT_RED_STONE_POWDER, 0x02)},
			{::BlockBaseType::BBT_RED_STONE_BLOCK, new ::CubicBlock(&red_stone_block_btx, ::BlockType::BT_RED_STONE_BLOCK, 0x04)},
			{::BlockBaseType::BBT_RED_STONE_LAMP, new ::CubicBlock(&red_stone_lamp_btx, ::BlockType::BT_RED_STONE_LAMP, 0x04)},
			{::BlockBaseType::BBT_ACTIVE_RED_STONE_LAMP, new ::CubicBlock(&active_red_stone_lamp_btx, ::BlockType::BT_RED_STONE_LAMP, 0x04)},
			{::BlockBaseType::BBT_RED_STONE_TORCH, new ::TorchBlock(&red_stone_torch_btx, ::BlockType::BT_RED_STONE_TORCH, 0x02)},
			{::BlockBaseType::BBT_ACTIVE_RED_STONE_TORCH, new ::TorchBlock(&active_red_stone_torch_btx, ::BlockType::BT_RED_STONE_TORCH, 0x02)}
		};

		::Sseu::BlockSource::block_name = {
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_AIR], ::std::string("air")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_TEST_BLOCK], ::std::string("test_block")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_BLACK_BLOCK], ::std::string("black_block")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK], ::std::string("white_block")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER], ::std::string("red_stone_powder")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FB], ::std::string("red_stone_powder_joint_fb")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_LR], ::std::string("red_stone_powder_joint_lr")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FL], ::std::string("red_stone_powder_joint_fl")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FR], ::std::string("red_stone_powder_joint_fr")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_BL], ::std::string("red_stone_powder_joint_bl")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_BR], ::std::string("red_stone_powder_joint_br")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FBL], ::std::string("red_stone_powder_joint_fbl")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FBR], ::std::string("red_stone_powder_joint_fbr")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FLR], ::std::string("red_stone_powder_joint_flr")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_BLR], ::std::string("red_stone_powder_joint_blr")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FBLR], ::std::string("red_stone_powder_joint_fblr")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER], ::std::string("active_red_stone_powder")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_FB], ::std::string("active_red_stone_powder_joint_fb")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_LR], ::std::string("active_red_stone_powder_joint_lr")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_FL], ::std::string("active_red_stone_powder_joint_fl")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_FR], ::std::string("active_red_stone_powder_joint_fr")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_BL], ::std::string("active_red_stone_powder_joint_bl")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_BR], ::std::string("active_red_stone_powder_joint_br")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBL], ::std::string("active_red_stone_powder_joint_fbl")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBR], ::std::string("active_red_stone_powder_joint_fbr")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_FLR], ::std::string("active_red_stone_powder_joint_flr")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_BLR], ::std::string("active_red_stone_powder_joint_blr")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBLR], ::std::string("active_red_stone_powder_joint_fblr")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_BLOCK], ::std::string("red_stone_block")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_LAMP], ::std::string("red_stone_lamp")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_ACTIVE_RED_STONE_LAMP], ::std::string("active_red_stone_lamp")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_TORCH], ::std::string("red_stone_torch")},
			{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_ACTIVE_RED_STONE_TORCH], ::std::string("active_red_stone_torch")}
		};

		::Sseu::BlockSource::null_block_inst.block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_AIR];
	}

	void source_quit() {
		for (::std::pair<::BlockBaseType, ::BlockBase *const> block_pair : ::Sseu::BlockSource::block_enum) {
			// TCO<<block_pair.second<<'\n';
			delete block_pair.second;
		}
	}
}

#endif
