#ifndef BLOCK_BASE_HPP
#define BLOCK_BASE_HPP

#include "headers.hpp"

enum BlockType{
	BT_AIR,
	BT_TEST_BLOCK,
	BT_BLACK_BLOCK,
	BT_WHITE_BLOCK,
	BT_RED_STONE_POWDER,
	BT_RED_STONE_BLOCK,
	BT_RED_STONE_LAMP,
	BT_RED_STONE_TORCH
};

enum BlockBaseType{
	BBT_AIR,
	BBT_TEST_BLOCK,
	BBT_BLACK_BLOCK,
	BBT_WHITE_BLOCK,
	BBT_RED_STONE_POWDER,
	BBT_RED_STONE_POWDER_JOINT_FB,
	BBT_RED_STONE_POWDER_JOINT_LR,
	BBT_RED_STONE_POWDER_JOINT_FL,
	BBT_RED_STONE_POWDER_JOINT_FR,
	BBT_RED_STONE_POWDER_JOINT_BL,
	BBT_RED_STONE_POWDER_JOINT_BR,
	BBT_RED_STONE_POWDER_JOINT_FBL,
	BBT_RED_STONE_POWDER_JOINT_FBR,
	BBT_RED_STONE_POWDER_JOINT_FLR,
	BBT_RED_STONE_POWDER_JOINT_BLR,
	BBT_RED_STONE_POWDER_JOINT_FBLR,
	BBT_ACTIVE_RED_STONE_POWDER,
	BBT_ACTIVE_RED_STONE_POWDER_JOINT_FB,
	BBT_ACTIVE_RED_STONE_POWDER_JOINT_LR,
	BBT_ACTIVE_RED_STONE_POWDER_JOINT_FL,
	BBT_ACTIVE_RED_STONE_POWDER_JOINT_FR,
	BBT_ACTIVE_RED_STONE_POWDER_JOINT_BL,
	BBT_ACTIVE_RED_STONE_POWDER_JOINT_BR,
	BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBL,
	BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBR,
	BBT_ACTIVE_RED_STONE_POWDER_JOINT_FLR,
	BBT_ACTIVE_RED_STONE_POWDER_JOINT_BLR,
	BBT_ACTIVE_RED_STONE_POWDER_JOINT_FBLR,
	BBT_RED_STONE_BLOCK,
	BBT_RED_STONE_LAMP,
	BBT_ACTIVE_RED_STONE_LAMP,
	BBT_RED_STONE_TORCH,
	BBT_ACTIVE_RED_STONE_TORCH,
};

namespace Sseu::BlockSource {
	::std::unordered_map<::BlockType, ::BlockBaseType const> block_base_default{
		{::BlockType::BT_AIR, ::BlockBaseType::BBT_AIR},
		{::BlockType::BT_TEST_BLOCK, ::BlockBaseType::BBT_TEST_BLOCK},
		{::BlockType::BT_BLACK_BLOCK, ::BlockBaseType::BBT_BLACK_BLOCK},
		{::BlockType::BT_WHITE_BLOCK, ::BlockBaseType::BBT_WHITE_BLOCK},
		{::BlockType::BT_RED_STONE_POWDER, ::BlockBaseType::BBT_RED_STONE_POWDER},
		{::BlockType::BT_RED_STONE_BLOCK, ::BlockBaseType::BBT_RED_STONE_BLOCK},
		{::BlockType::BT_RED_STONE_LAMP, ::BlockBaseType::BBT_RED_STONE_LAMP},
		{::BlockType::BT_RED_STONE_TORCH, ::BlockBaseType::BBT_ACTIVE_RED_STONE_TORCH}
	};
}

struct BlockBase{
	::BlockType block_type;
	::uint8_t interaction_level;
	inline BlockBase(::BlockType block_type, ::uint8_t interaction_level): block_type{block_type}, interaction_level{interaction_level} { }
	inline virtual void display(::float_t const, ::float_t const, ::float_t const) const { };
	inline virtual void select(::float_t const, ::float_t const, ::float_t const) const { }
	inline virtual ::double_t get_left_border(::double_t const) const {
		return 1.125;
	}
	inline virtual ::double_t get_right_border(::double_t const) const {
		return -0.125;
	}
	inline virtual ::double_t get_front_border(::double_t const) const {
		return 1.125;
	}
	inline virtual ::double_t get_back_border(::double_t const) const {
		return -0.125;
	}
	inline virtual ::double_t get_bottom_border(::double_t const, ::double_t const) const {
		return 1.125;
	}
	inline virtual ::double_t get_top_border(::double_t const, ::double_t const) const {
		return -0.125;
	}
};

struct AirBlock : BlockBase{
	inline AirBlock(): ::BlockBase(::BlockType::BT_AIR, 0) { };
};

struct CubicBlock : BlockBase{
	static ::int32_t faces[6][12];
	::uint32_t tex_ids[6];
	inline CubicBlock(::Sseu::BlockSource::BlockTexDataWrapper<6U> *tex, ::BlockType block_type, ::uint8_t interaction_level): ::BlockBase(block_type, interaction_level) {
		::glGenTextures(6, this->tex_ids);
		for (::uint8_t face{0U}; face < 6U; ++face) {
			::glBindTexture(GL_TEXTURE_2D, this->tex_ids[face]);
			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->data[face]);
		}
	}
	~CubicBlock() {
		::glDeleteTextures(6, this->tex_ids);
	}
	inline void display(::float_t const x, ::float_t const y, ::float_t const z) const override {
		::glPushMatrix();
		::glTranslated(x, y, z);
		::glColor3ub(0xff, 0xff, 0xff);
		for (::int32_t (*face)[12]{::CubicBlock::faces}; face - ::CubicBlock::faces < 6; ++face) {
			::glBindTexture(GL_TEXTURE_2D, this->tex_ids[face - ::CubicBlock::faces]);
			::glBegin(GL_QUADS);
			::glTexCoord2i(0, 1);
			::glVertex3iv(*face);
			::glTexCoord2i(0, 0);
			::glVertex3iv(*face + 3);
			::glTexCoord2i(1, 0);
			::glVertex3iv(*face + 6);
			::glTexCoord2i(1, 1);
			::glVertex3iv(*face + 9);
			::glEnd();
		}
		::glPopMatrix();
	}
	inline void select(::float_t const x, ::float_t const y, ::float_t const z) const override {
		::glPushMatrix();
		::glTranslatef(x, y, z);
		::glBegin(GL_LINE_LOOP);
		::glVertex3iv(::CubicBlock::faces[0]);
		::glVertex3iv(::CubicBlock::faces[0] + 3);
		::glVertex3iv(::CubicBlock::faces[0] + 6);
		::glVertex3iv(::CubicBlock::faces[0] + 9);
		::glEnd();
		::glBegin(GL_LINE_LOOP);
		::glVertex3iv(::CubicBlock::faces[1]);
		::glVertex3iv(::CubicBlock::faces[1] + 3);
		::glVertex3iv(::CubicBlock::faces[1] + 6);
		::glVertex3iv(::CubicBlock::faces[1] + 9);
		::glEnd();
		::glBegin(GL_LINES);
		::glVertex3iv(::CubicBlock::faces[0]);
		::glVertex3iv(::CubicBlock::faces[1] + 9);
		::glVertex3iv(::CubicBlock::faces[0] + 3);
		::glVertex3iv(::CubicBlock::faces[1] + 6);
		::glVertex3iv(::CubicBlock::faces[0] + 6);
		::glVertex3iv(::CubicBlock::faces[1] + 3);
		::glVertex3iv(::CubicBlock::faces[0] + 9);
		::glVertex3iv(::CubicBlock::faces[1]);
		::glEnd();
		::glPopMatrix();
	}
	inline ::double_t get_left_border(::double_t const) const override {
		return 0.0;
	}
	inline ::double_t get_right_border(::double_t const) const override {
		return 1.0;
	}
	inline ::double_t get_front_border(::double_t const) const override {
		return 0.0;
	}
	inline ::double_t get_back_border(::double_t const) const override {
		return 1.0;
	}
	inline ::double_t get_bottom_border(::double_t const, ::double_t const) const override {
		return 0.0;
	}
	inline ::double_t get_top_border(::double_t const, ::double_t const) const override {
		return 1.0;
	}
};
::int32_t CubicBlock::faces[6][12]{{ // Bottom
	1, 0, 0,
	1, 1, 0,
	0, 1, 0,
	0, 0, 0
}, { // Top
	0, 0, 1,
	0, 1, 1,
	1, 1, 1,
	1, 0, 1
}, { // Front
	0, 0, 0,
	0, 0, 1,
	1, 0, 1,
	1, 0, 0
}, { // Back
	1, 1, 0,
	1, 1, 1,
	0, 1, 1,
	0, 1, 0
}, { //Left
	0, 1, 0,
	0, 1, 1,
	0, 0, 1,
	0, 0, 0
}, { // Right
	1, 0, 0,
	1, 0, 1,
	1, 1, 1,
	1, 1, 0
}};

struct PowderBlock : BlockBase{
	static ::float_t faces[2][12];
	::uint32_t tex_ids[1];
	inline PowderBlock(::Sseu::BlockSource::BlockTexDataWrapper<1U> *tex, ::BlockType block_type, ::uint8_t interaction_level): ::BlockBase(block_type, interaction_level) {
		::glGenTextures(1, this->tex_ids);
		::glBindTexture(GL_TEXTURE_2D, this->tex_ids[0]);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->data[0]);
	}
	~PowderBlock() {
		::glDeleteTextures(1, this->tex_ids);
	}
	inline virtual void display(::float_t const x, ::float_t const y, ::float_t const z) const override {
		::glPushMatrix();
		::glTranslatef(x, y, z);
		::glColor3ub(0xff, 0xff, 0xff);
		::glBindTexture(GL_TEXTURE_2D, this->tex_ids[0]);
		::glBegin(GL_QUADS);
		::glTexCoord2i(0, 1);
		::glVertex3fv(PowderBlock::faces[1]);
		::glTexCoord2i(0, 0);
		::glVertex3fv(PowderBlock::faces[1] + 3);
		::glTexCoord2i(1, 0);
		::glVertex3fv(PowderBlock::faces[1] + 6);
		::glTexCoord2i(1, 1);
		::glVertex3fv(PowderBlock::faces[1] + 9);
		::glEnd();
		::glPopMatrix();
	}
	inline void select(::float_t const x, ::float_t const y, ::float_t const z) const override {
		::glPushMatrix();
		::glTranslatef(x, y, z);
		::glBegin(GL_LINE_LOOP);
		::glVertex3fv(::PowderBlock::faces[0]);
		::glVertex3fv(::PowderBlock::faces[0] + 3);
		::glVertex3fv(::PowderBlock::faces[0] + 6);
		::glVertex3fv(::PowderBlock::faces[0] + 9);
		::glEnd();
		::glBegin(GL_LINE_LOOP);
		::glVertex3fv(::PowderBlock::faces[1]);
		::glVertex3fv(::PowderBlock::faces[1] + 3);
		::glVertex3fv(::PowderBlock::faces[1] + 6);
		::glVertex3fv(::PowderBlock::faces[1] + 9);
		::glEnd();
		::glBegin(GL_LINES);
		::glVertex3fv(::PowderBlock::faces[0]);
		::glVertex3fv(::PowderBlock::faces[1] + 9);
		::glVertex3fv(::PowderBlock::faces[0] + 3);
		::glVertex3fv(::PowderBlock::faces[1] + 6);
		::glVertex3fv(::PowderBlock::faces[0] + 6);
		::glVertex3fv(::PowderBlock::faces[1] + 3);
		::glVertex3fv(::PowderBlock::faces[0] + 9);
		::glVertex3fv(::PowderBlock::faces[1]);
		::glEnd();
		::glPopMatrix();
	}
};
::float_t PowderBlock::faces[2][12]{{ // Bottom
	1.0F, 0.0F, 0.0F,
	1.0F, 1.0F, 0.0F,
	0.0F, 1.0F, 0.0F,
	0.0F, 0.0F, 0.0F
}, { // Top
	0.0F, 0.0F, 0.125F,
	0.0F, 1.0F, 0.125F,
	1.00F, 1.00F, 0.125F,
	1.0F, 0.0F, 0.125F
}};
//取件號碼8610門口洗衣池上箱子裏
struct TorchBlock : BlockBase{
	static ::float_t faces[6][12];
	::uint32_t tex_ids[6];
	inline TorchBlock(::Sseu::BlockSource::BlockTexDataWrapper<6U> *tex, ::BlockType block_type, ::uint8_t interaction_level): ::BlockBase(block_type, interaction_level) {
		::glGenTextures(6, this->tex_ids);
		for (::uint8_t face{0U}; face < 6U; ++face) {
			::glBindTexture(GL_TEXTURE_2D, this->tex_ids[face]);
			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->data[face]);
		}
	}
	~TorchBlock() {
		::glDeleteTextures(6, this->tex_ids);
	}
	inline void display(::float_t const x, ::float_t const y, ::float_t const z) const override {
		::glPushMatrix();
		::glTranslated(x, y, z);
		::glColor3ub(0xff, 0xff, 0xff);
		// for (::float_t (*face)[12]{::TorchBlock::faces}; face - ::TorchBlock::faces < 6; ++face) {
		//	   ::glBindTexture(GL_TEXTURE_2D, this->tex_ids[face - ::TorchBlock::faces]);
		//	   ::glBegin(GL_QUADS);
		//	   ::glTexCoord2i(0, 1);
		//	   ::glVertex3fv(*face);
		//	   ::glTexCoord2i(0, 0);
		//	   ::glVertex3fv(*face + 3);
		//	   ::glTexCoord2i(1, 0);
		//	   ::glVertex3fv(*face + 6);
		//	   ::glTexCoord2i(1, 1);
		//	   ::glVertex3fv(*face + 9);
		//	   ::glEnd();
		// }
		for (::float_t (*face)[12] : {
			::TorchBlock::faces + 2,
			::TorchBlock::faces + 4,
			::TorchBlock::faces + 3,
			::TorchBlock::faces + 5,
			::TorchBlock::faces + 1,
			::TorchBlock::faces
		}) {
			::glBindTexture(GL_TEXTURE_2D, this->tex_ids[face - ::TorchBlock::faces]);
			::glBegin(GL_QUADS);
			::glTexCoord2i(0, 1);
			::glVertex3fv(*face);
			::glTexCoord2i(0, 0);
			::glVertex3fv(*face + 3);
			::glTexCoord2i(1, 0);
			::glVertex3fv(*face + 6);
			::glTexCoord2i(1, 1);
			::glVertex3fv(*face + 9);
			::glEnd();
		}
		::glPopMatrix();
	}
	inline void select(::float_t const x, ::float_t const y, ::float_t const z) const override {
		::glPushMatrix();
		::glTranslatef(x, y, z);
		::glBegin(GL_LINE_LOOP);
		::glVertex3fv(::TorchBlock::faces[0]);
		::glVertex3fv(::TorchBlock::faces[0] + 3);
		::glVertex3fv(::TorchBlock::faces[0] + 6);
		::glVertex3fv(::TorchBlock::faces[0] + 9);
		::glEnd();
		::glBegin(GL_LINE_LOOP);
		::glVertex3fv(::TorchBlock::faces[1]);
		::glVertex3fv(::TorchBlock::faces[1] + 3);
		::glVertex3fv(::TorchBlock::faces[1] + 6);
		::glVertex3fv(::TorchBlock::faces[1] + 9);
		::glEnd();
		::glBegin(GL_LINES);
		::glVertex3fv(::TorchBlock::faces[0]);
		::glVertex3fv(::TorchBlock::faces[1] + 9);
		::glVertex3fv(::TorchBlock::faces[0] + 3);
		::glVertex3fv(::TorchBlock::faces[1] + 6);
		::glVertex3fv(::TorchBlock::faces[0] + 6);
		::glVertex3fv(::TorchBlock::faces[1] + 3);
		::glVertex3fv(::TorchBlock::faces[0] + 9);
		::glVertex3fv(::TorchBlock::faces[1]);
		::glEnd();
		::glPopMatrix();
	}
};
::float_t TorchBlock::faces[6][12]{{ // Bottom
	1.0F, 0.0F, 0.0F,
	1.0F, 1.0F, 0.0F,
	0.0F, 1.0F, 0.0F,
	0.0F, 0.0F, 0.0F
}, { // Top
	0.0F, 0.0F, 0.75F,
	0.0F, 1.0F, 0.75F,
	1.0F, 1.0F, 0.75F,
	1.0F, 0.0F, 0.75F
}, { // Front
	0.0F, 0.4375F, 0.0F,
	0.0F, 0.4375F, 1.0F,
	1.0F, 0.4375F, 1.0F,
	1.0F, 0.4375F, 0.0F
}, { // Back
	1.0F, 0.5625F, 0.0F,
	1.0F, 0.5625F, 1.0F,
	0.0F, 0.5625F, 1.0F,
	0.0F, 0.5625F, 0.0F
}, { //Left
	0.4375F, 1.0F, 0.0F,
	0.4375F, 1.0F, 1.0F,
	0.4375F, 0.0F, 1.0F,
	0.4375F, 0.0F, 0.0F
}, { // Right
	0.5625F, 0.0F, 0.0F,
	0.5625F, 0.0F, 1.0F,
	0.5625F, 1.0F, 1.0F,
	0.5625F, 1.0F, 0.0F
}};

#endif
