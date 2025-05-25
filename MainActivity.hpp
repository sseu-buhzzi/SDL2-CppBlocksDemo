#ifndef MAIN_ACTIVITY_HPP
#define MAIN_ACTIVITY_HPP

#include "FrameRateCounter.hpp"
#include "GameEvent.hpp"
#include "Random.hpp"

enum ScreenInterfaceType{
	SIT_GAME,
	SIT_PACKAGE
};

struct MainActivity : Sseu::ActivityBase{
	bool is_existing;
	::Sseu::Random random;
	::Sseu::FrameRateCounter frame_rate_counter;
	::Camera camera;
	::ChunkMap chunk_map;
	::std::vector<::BlockInstance *> visible_block_list;
	// ::std::deque<::std::unordered_set<::BlockInstance *>> visible_block_list;
	// ::std::vector<::TexturedFaceInstance *> visible_face_list;
	::std::vector<::Entity *> entity_list;
	::CameraEntity camera_entity;
	::std::deque<GameEvent> event_deque;
	::ScreenInterfaceType screen_interface;
	::PackageScreenInterface package_screen_interface;
	::std::thread filter_thr, event_thr;
	::std::mutex visible_block_list_mutex;

	inline MainActivity();
	~MainActivity() override;
	void init() override;
	void update() override;

	inline void start_thrs();
	inline void clear_background() const;
	inline void start_world_painting() const;
	inline void start_screen_painting() const;
	inline ::double_t get_squared_distance_from_block_to_camera(::BlockInstance *);
	inline void filter_imvisible_blocks();
	inline void world_display();
	inline void screen_display();
	inline void event_func();
	inline void refresh_block(::BlockInstance *);
	inline void activate_rs_circuit(::BlockInstance *);
	inline void clear_rs_circuit(::BlockInstance *, ::std::vector<::BlockInstance *> *);
	inline void deactive_rs_circuit(::BlockInstance *);

	void __TEST_TERRAIN__() {
		for (::int64_t x{-this->chunk_map.left_offset}; x < this->chunk_map.right_offset; ++x) {
			for (::int64_t y{-this->chunk_map.front_offset}; y < this->chunk_map.right_offset; ++y) {
				this->chunk_map.generate_chunk_at(x, y);
				// if (x == 0LL && y == 1LL) {
				//	   for (uint32_t i{0U}; i < ::Chunk::SIZE; ++i) {
				//		   for (uint32_t j{0U}; j < ::Chunk::SIZE; ++j) {
				//			   for (uint32_t k{0U}; k < ::Chunk::SIZE; ++k) {
				//				   if (this->random.rand_bool(0.01)) {
				//					   this->chunk_map.origin[x][y].block_map[i][j][k].block = ::std::next(::Sseu::BlockSource::block_enum.begin(), this->random.rand_range(::Sseu::BlockSource::block_enum.size()))->second;
				//				   }
				//			   }
				//		   }
				//	   }
				// }
			}
		}
		TCO << "Chunks generated successfully.\n";
		for (::int64_t x{-this->chunk_map.left_offset}; x < this->chunk_map.right_offset; ++x) {
			for (::int64_t y{-this->chunk_map.front_offset}; y < this->chunk_map.right_offset; ++y) {
				this->chunk_map.link_chunk_at(x, y);
			}
		}
		TCO << "Chunks linked successfully.\n";
																// return;
		// this->chunk_map.origin[0][0].block_map[2][2][0].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
		// this->chunk_map.origin[0][0].block_map[2][2][1].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FB];
		// this->chunk_map.origin[0][0].block_map[2][3][1].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FB];
		// this->chunk_map.origin[0][0].block_map[2][4][1].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FB];
		// this->chunk_map.origin[0][0].block_map[2][5][1].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FB];
		// this->chunk_map.origin[0][0].block_map[2][6][1].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FB];
		// this->chunk_map.origin[0][0].block_map[2][7][1].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FB];
		// this->chunk_map.origin[0][0].block_map[3][2][1].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FB];
		// this->chunk_map.origin[0][0].block_map[3][3][1].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FB];
		// this->chunk_map.origin[0][0].block_map[3][4][1].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FB];
		// this->chunk_map.origin[0][0].block_map[3][5][1].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FB];
		// this->chunk_map.origin[0][0].block_map[3][6][1].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER_JOINT_FB];
// return;
		::BlockBase *__TEST_BLOCK_LIST__[3]{::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK], ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_BLACK_BLOCK], ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_TEST_BLOCK]};
		this->chunk_map.origin[0][0].block_map[2][2][0].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
		this->chunk_map.origin[0][0].block_map[2][2][1].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_RED_STONE_POWDER];
// return;
		for (::uint32_t i{0U}; i < ::Chunk::SIZE; ++i) {
			for (::uint32_t j{0U}; j < ::Chunk::SIZE; ++j) {
				this->chunk_map.origin[0][0].block_map[i][j][0].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
				this->chunk_map.origin[0][1].block_map[i][j][0].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_BLACK_BLOCK];
				this->chunk_map.origin[1][0].block_map[i][j][0].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_BLACK_BLOCK];
				this->chunk_map.origin[1][1].block_map[i][j][0].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_TEST_BLOCK];
				this->chunk_map.origin[-1][1].block_map[i][j][0].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_TEST_BLOCK];
				this->chunk_map.origin[-1][-1].block_map[i][j][3].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_BLACK_BLOCK];
				this->chunk_map.origin[-1][-1].block_map[i][j][0].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_TEST_BLOCK];
			}
		}
		this->chunk_map.origin[0][0].block_map[4][4][3].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_TEST_BLOCK];
		this->chunk_map.origin[0][0].block_map[3][4][3].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_TEST_BLOCK];
		this->chunk_map.origin[0][0].block_map[5][4][3].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_TEST_BLOCK];
		this->chunk_map.origin[0][0].block_map[4][3][3].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_TEST_BLOCK];
		this->chunk_map.origin[0][0].block_map[4][5][3].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_TEST_BLOCK];
		for (::uint8_t k{1U}; k < 8; ++k) {
			this->chunk_map.origin[0][0].block_map[0][0][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
			this->chunk_map.origin[0][0].block_map[0][1][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
			this->chunk_map.origin[0][0].block_map[0][2][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
			this->chunk_map.origin[0][0].block_map[0][3][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
			this->chunk_map.origin[0][0].block_map[0][4][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
			this->chunk_map.origin[0][0].block_map[0][5][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
			this->chunk_map.origin[0][0].block_map[0][6][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
			this->chunk_map.origin[0][0].block_map[0][7][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
			this->chunk_map.origin[0][0].block_map[0][8][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
			this->chunk_map.origin[0][0].block_map[1][0][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
			this->chunk_map.origin[0][0].block_map[2][0][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
			this->chunk_map.origin[0][0].block_map[3][0][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
			this->chunk_map.origin[0][0].block_map[4][0][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
			this->chunk_map.origin[0][0].block_map[5][0][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
			this->chunk_map.origin[0][0].block_map[6][0][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
			this->chunk_map.origin[0][0].block_map[7][0][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
			this->chunk_map.origin[0][0].block_map[8][0][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
			this->chunk_map.origin[0][0].block_map[8][8][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
			this->chunk_map.origin[0][0].block_map[8][7][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
			this->chunk_map.origin[0][0].block_map[8][6][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
			this->chunk_map.origin[0][0].block_map[7][8][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
			this->chunk_map.origin[0][0].block_map[6][8][k].block = ::Sseu::BlockSource::block_enum[::BlockBaseType::BBT_WHITE_BLOCK];
		}
		for (::uint32_t i{0U}; i < ::Chunk::SIZE; ++i) {
			for (::uint32_t j{0U}; j < ::Chunk::SIZE; ++j) {
				for (::uint32_t k{0U}; k < ::Chunk::SIZE; ++k) {
					if (this->random.rand_bool(0.01)) {
						this->chunk_map.origin[1][0].block_map[i][j][k].block = __TEST_BLOCK_LIST__[this->random.rand_range(3U)];
					}
				}
			}
		}
	}
};

#endif
