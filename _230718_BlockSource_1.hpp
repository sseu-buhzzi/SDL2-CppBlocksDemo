#ifndef BLOCK_SOURCE_HPP
#define BLOCK_SOURCE_HPP

#include "BlockBase.hpp"
#include "BlockSource\ActiveRedStonePowder.hpp"
#include "BlockSource\ActiveRedStonePowder_prune.hpp"
#include "BlockSource\ActiveRedStonePowderJoint_prune.hpp"
#include "BlockSource\ActiveRedStonePowderJointHalf_prune.hpp"
#include "BlockSource\BlackBlock.hpp"
#include "BlockSource\RedStoneBlock.hpp"
#include "BlockSource\RedStonePowder.hpp"
#include "BlockSource\RedStonePowder_prune.hpp"
#include "BlockSource\RedStonePowderJoint_prune.hpp"
#include "BlockSource\RedStonePowderJointHalf_prune.hpp"
#include "BlockSource\TestBlock.hpp"
#include "BlockSource\WhiteBlock.hpp"

namespace Sseu::BlockSource {
    ::BlockInstance null_block_inst;
    ::std::unordered_map<::BlockBaseType, ::BlockBase *const> block_enum;
    ::std::unordered_map<::BlockBase *, char const *const> block_name;

    void source_init() {
        ::Sseu::BlockSource::block_enum = {
            {BBT_AIR, new AirBlock},
            {BBT_TEST_BLOCK, new CubicBlock(&test_block_btx, BT_TEST_BLOCK, 0x04)},
            {BBT_BLACK_BLOCK, new CubicBlock(&black_block_btx, BT_BLACK_BLOCK, 0x04)},
            {BBT_WHITE_BLOCK, new CubicBlock(&white_block_btx, BT_WHITE_BLOCK, 0x04)},
            {BBT_RED_STONE_POWDER, new RedStonePowderBlock(&red_stone_powder_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_RED_STONE_POWDER_JOINT_FB, new RedStonePowderJointFbBlock(&red_stone_powder_joint_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_RED_STONE_POWDER_JOINT_LR, new RedStonePowderJointLrBlock(&red_stone_powder_joint_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_RED_STONE_POWDER_JOINT_FL, new RedStonePowderJointFlBlock(&red_stone_powder_btx_prune, &red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_RED_STONE_POWDER_JOINT_FR, new RedStonePowderJointFrBlock(&red_stone_powder_btx_prune, &red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_RED_STONE_POWDER_JOINT_BL, new RedStonePowderJointBlBlock(&red_stone_powder_btx_prune, &red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_RED_STONE_POWDER_JOINT_BR, new RedStonePowderJointBrBlock(&red_stone_powder_btx_prune, &red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_RED_STONE_POWDER_JOINT_FBR, new RedStonePowderJointFbrBlock(&red_stone_powder_btx_prune, &red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_RED_STONE_POWDER_JOINT_FLR, new RedStonePowderJointFlrBlock(&red_stone_powder_btx_prune, &red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_RED_STONE_POWDER_JOINT_BLR, new RedStonePowderJointBlrBlock(&red_stone_powder_btx_prune, &red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_RED_STONE_POWDER_JOINT_FBLR, new RedStonePowderJointFblrBlock(&red_stone_powder_btx_prune, &red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_ACTIVE_RED_STONE_POWDER, new RedStonePowderBlock(&active_red_stone_powder_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_ACTIVE_RED_STONE_POWDER_JOINT_FB, new RedStonePowderJointFbBlock(&active_red_stone_powder_joint_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_ACTIVE_RED_STONE_POWDER_JOINT_LR, new RedStonePowderJointLrBlock(&active_red_stone_powder_joint_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_ACTIVE_RED_STONE_POWDER_JOINT_FL, new RedStonePowderJointFlBlock(&active_red_stone_powder_btx_prune, &active_red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_ACTIVE_RED_STONE_POWDER_JOINT_FR, new RedStonePowderJointFrBlock(&active_red_stone_powder_btx_prune, &active_red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_ACTIVE_RED_STONE_POWDER_JOINT_BL, new RedStonePowderJointBlBlock(&active_red_stone_powder_btx_prune, &active_red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_ACTIVE_RED_STONE_POWDER_JOINT_BR, new RedStonePowderJointBrBlock(&active_red_stone_powder_btx_prune, &active_red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBL, new RedStonePowderJointFblBlock(&active_red_stone_powder_btx_prune, &active_red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBR, new RedStonePowderJointFbrBlock(&active_red_stone_powder_btx_prune, &active_red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_ACTIVE_RED_STONE_POWDER_JOINT_FLR, new RedStonePowderJointFlrBlock(&active_red_stone_powder_btx_prune, &active_red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_ACTIVE_RED_STONE_POWDER_JOINT_BLR, new RedStonePowderJointBlrBlock(&active_red_stone_powder_btx_prune, &active_red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBLR, new RedStonePowderJointFblrBlock(&active_red_stone_powder_btx_prune, &active_red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02)},
            {BBT_RED_STONE_BLOCK, new CubicBlock(&red_stone_block_btx, BT_RED_STONE_BLOCK, 0x04)}
        };
        ::Sseu::BlockSource::block_name = {
            {block_enum[BBT_AIR], "air"},
            {block_enum[BBT_TEST_BLOCK], "test_block"},
            {block_enum[BBT_BLACK_BLOCK], "black_block"},
            {block_enum[BBT_WHITE_BLOCK], "white_block"},
            {block_enum[BBT_RED_STONE_POWDER], "red_stone_powder"},
            {block_enum[BBT_RED_STONE_POWDER_JOINT_FB], "red_stone_powder_joint_fb"},
            {block_enum[BBT_RED_STONE_POWDER_JOINT_LR], "red_stone_powder_joint_lr"},
            {block_enum[BBT_RED_STONE_POWDER_JOINT_FL], "red_stone_powder_joint_fl"},
            {block_enum[BBT_RED_STONE_POWDER_JOINT_FR], "red_stone_powder_joint_fr"},
            {block_enum[BBT_RED_STONE_POWDER_JOINT_BL], "red_stone_powder_joint_bl"},
            {block_enum[BBT_RED_STONE_POWDER_JOINT_BR], "red_stone_powder_joint_br"},
            {block_enum[BBT_RED_STONE_POWDER_JOINT_FBL], "red_stone_powder_joint_fbl"},
            {block_enum[BBT_RED_STONE_POWDER_JOINT_FBR], "red_stone_powder_joint_fbr"},
            {block_enum[BBT_RED_STONE_POWDER_JOINT_FLR], "red_stone_powder_joint_flr"},
            {block_enum[BBT_RED_STONE_POWDER_JOINT_BLR], "red_stone_powder_joint_blr"},
            {block_enum[BBT_RED_STONE_POWDER_JOINT_FBLR], "red_stone_powder_joint_fblr"},
            {block_enum[BBT_ACTIVE_RED_STONE_POWDER], "active_red_stone_powder"},
            {block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_FB], "active_red_stone_powder_joint_fb"},
            {block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_LR], "active_red_stone_powder_joint_lr"},
            {block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_FL], "active_red_stone_powder_joint_fl"},
            {block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_FR], "active_red_stone_powder_joint_fr"},
            {block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_BL], "active_red_stone_powder_joint_bl"},
            {block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_BR], "active_red_stone_powder_joint_br"},
            {block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBL], "active_red_stone_powder_joint_fbl"},
            {block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBR], "active_red_stone_powder_joint_fbr"},
            {block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_FLR], "active_red_stone_powder_joint_flr"},
            {block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_BLR], "active_red_stone_powder_joint_blr"},
            {block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBLR], "active_red_stone_powder_joint_fblr"},
            {block_enum[BBT_RED_STONE_BLOCK], "red_stone_block"}
        };
        // block_enum.emplace(BBT_AIR, new AirBlock);
        // block_name.emplace(block_enum[BBT_AIR], "air");

        // block_enum.emplace(BBT_TEST_BLOCK, new CubicBlock(&test_block_btx, BT_TEST_BLOCK, 0x04));
        // block_name.emplace(block_enum[BBT_TEST_BLOCK], "test_block");

        // block_enum.emplace(BBT_BLACK_BLOCK, new CubicBlock(&black_block_btx, BT_BLACK_BLOCK, 0x04));
        // block_name.emplace(block_enum[BBT_BLACK_BLOCK], "black_block");

        // block_enum.emplace(BBT_WHITE_BLOCK, new CubicBlock(&white_block_btx, BT_WHITE_BLOCK, 0x04));
        // block_name.emplace(block_enum[BBT_WHITE_BLOCK], "white_block");

        // block_enum.emplace(BBT_RED_STONE_POWDER, new RedStonePowderBlock(&red_stone_powder_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_RED_STONE_POWDER], "red_stone_powder");

        // block_enum.emplace(BBT_RED_STONE_POWDER_JOINT_FB, new RedStonePowderJointFbBlock(&red_stone_powder_joint_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_RED_STONE_POWDER_JOINT_FB], "red_stone_powder_joint_fb");

        // block_enum.emplace(BBT_RED_STONE_POWDER_JOINT_LR, new RedStonePowderJointLrBlock(&red_stone_powder_joint_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_RED_STONE_POWDER_JOINT_LR], "red_stone_powder_joint_lr");

        // block_enum.emplace(BBT_RED_STONE_POWDER_JOINT_FL, new RedStonePowderJointFlBlock(&red_stone_powder_btx_prune, &red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_RED_STONE_POWDER_JOINT_FL], "red_stone_powder_joint_fl");

        // block_enum.emplace(BBT_RED_STONE_POWDER_JOINT_FR, new RedStonePowderJointFrBlock(&red_stone_powder_btx_prune, &red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_RED_STONE_POWDER_JOINT_FR], "red_stone_powder_joint_fr");

        // block_enum.emplace(BBT_RED_STONE_POWDER_JOINT_BL, new RedStonePowderJointBlBlock(&red_stone_powder_btx_prune, &red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_RED_STONE_POWDER_JOINT_BL], "red_stone_powder_joint_bl");

        // block_enum.emplace(BBT_RED_STONE_POWDER_JOINT_BR, new RedStonePowderJointBrBlock(&red_stone_powder_btx_prune, &red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_RED_STONE_POWDER_JOINT_BR], "red_stone_powder_joint_br");

        // block_enum.emplace(BBT_RED_STONE_POWDER_JOINT_FBL, new RedStonePowderJointFblBlock(&red_stone_powder_btx_prune, &red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_RED_STONE_POWDER_JOINT_FBL], "red_stone_powder_joint_fbl");

        // block_enum.emplace(BBT_RED_STONE_POWDER_JOINT_FBR, new RedStonePowderJointFbrBlock(&red_stone_powder_btx_prune, &red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_RED_STONE_POWDER_JOINT_FBR], "red_stone_powder_joint_fbr");

        // block_enum.emplace(BBT_RED_STONE_POWDER_JOINT_FLR, new RedStonePowderJointFlrBlock(&red_stone_powder_btx_prune, &red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_RED_STONE_POWDER_JOINT_FLR], "red_stone_powder_joint_flr");

        // block_enum.emplace(BBT_RED_STONE_POWDER_JOINT_BLR, new RedStonePowderJointBlrBlock(&red_stone_powder_btx_prune, &red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_RED_STONE_POWDER_JOINT_BLR], "red_stone_powder_joint_blr");

        // block_enum.emplace(BBT_RED_STONE_POWDER_JOINT_FBLR, new RedStonePowderJointFblrBlock(&red_stone_powder_btx_prune, &red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_RED_STONE_POWDER_JOINT_FBLR], "red_stone_powder_joint_fblr");

        // block_enum.emplace(BBT_ACTIVE_RED_STONE_POWDER, new RedStonePowderBlock(&active_red_stone_powder_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_ACTIVE_RED_STONE_POWDER], "active_red_stone_powder");

        // block_enum.emplace(BBT_ACTIVE_RED_STONE_POWDER_JOINT_FB, new RedStonePowderJointFbBlock(&active_red_stone_powder_joint_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_FB], "active_red_stone_powder_joint_fb");

        // block_enum.emplace(BBT_ACTIVE_RED_STONE_POWDER_JOINT_LR, new RedStonePowderJointLrBlock(&active_red_stone_powder_joint_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_LR], "active_red_stone_powder_joint_lr");

        // block_enum.emplace(BBT_ACTIVE_RED_STONE_POWDER_JOINT_FL, new RedStonePowderJointFlBlock(&active_red_stone_powder_btx_prune, &active_red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_FL], "active_red_stone_powder_joint_fl");

        // block_enum.emplace(BBT_ACTIVE_RED_STONE_POWDER_JOINT_FR, new RedStonePowderJointFrBlock(&active_red_stone_powder_btx_prune, &active_red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_FR], "active_red_stone_powder_joint_fr");

        // block_enum.emplace(BBT_ACTIVE_RED_STONE_POWDER_JOINT_BL, new RedStonePowderJointBlBlock(&active_red_stone_powder_btx_prune, &active_red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_BL], "active_red_stone_powder_joint_bl");

        // block_enum.emplace(BBT_ACTIVE_RED_STONE_POWDER_JOINT_BR, new RedStonePowderJointBrBlock(&active_red_stone_powder_btx_prune, &active_red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_BR], "active_red_stone_powder_joint_br");

        // block_enum.emplace(BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBL, new RedStonePowderJointFblBlock(&active_red_stone_powder_btx_prune, &active_red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBL], "active_red_stone_powder_joint_fbl");

        // block_enum.emplace(BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBR, new RedStonePowderJointFbrBlock(&active_red_stone_powder_btx_prune, &active_red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBR], "active_red_stone_powder_joint_fbr");

        // block_enum.emplace(BBT_ACTIVE_RED_STONE_POWDER_JOINT_FLR, new RedStonePowderJointFlrBlock(&active_red_stone_powder_btx_prune, &active_red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_FLR], "active_red_stone_powder_joint_flr");

        // block_enum.emplace(BBT_ACTIVE_RED_STONE_POWDER_JOINT_BLR, new RedStonePowderJointBlrBlock(&active_red_stone_powder_btx_prune, &active_red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_BLR], "active_red_stone_powder_joint_blr");

        // block_enum.emplace(BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBLR, new RedStonePowderJointFblrBlock(&active_red_stone_powder_btx_prune, &active_red_stone_powder_joint_half_btx_prune, BT_RED_STONE_POWDER, 0x02));
        // block_name.emplace(block_enum[BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBLR], "active_red_stone_powder_joint_fblr");

        // block_enum.emplace(BBT_RED_STONE_BLOCK, new CubicBlock(&red_stone_block_btx, BT_RED_STONE_BLOCK, 0x04));
        // block_name.emplace(block_enum[BBT_RED_STONE_BLOCK], "red_stone_block");

        null_block_inst.block = block_enum[BBT_AIR];
    }

    void source_quit() {
        for (std::pair<BlockBaseType const, BlockBase *> block_pair : block_enum) {
            delete block_pair.second;
        }
    }
}

#endif
