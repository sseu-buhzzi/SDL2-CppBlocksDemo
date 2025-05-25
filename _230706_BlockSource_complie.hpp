#ifndef BLOCK_SOURCE_HPP
#define BLOCK_SOURCE_HPP

namespace Sseu::BlockSource {
    char *get_dir_path(char *dir_path, char *const separator) {
        char *exe_path{::GetCommandLineA()};
        char *end_slash{exe_path};
        while (*end_slash != '\0') {
            ++end_slash;
        }
        while (*end_slash != '/' && *end_slash != '\\') {
            --end_slash;
        }
        *separator = *end_slash;
        for (char *ch{exe_path + 1}; ch != end_slash; ++ch) {
            *dir_path++ = *ch;
        }
        *dir_path = '\0';
        ::LocalFree(exe_path);
        return dir_path;
    }

    struct BlockTex{
        static const uint32_t SIZE;
        static const char FILE_SUFFIX[4];
        uint8_t *data[6];
        BlockTex(const char *rel_path): data{} {
            char abs_path[_MAX_DIR], separator;
            char *ch{get_dir_path(abs_path, &separator)};
            *ch = separator;
            while ((*++ch = *rel_path++) != '\0') { }
            std::ifstream uba(abs_path, std::ios::binary);
            std::cout << abs_path << '\n' << "D:\\SceuhBuhqi_SheuBuhi\\Trwingsciuh\\Uyndcienn\\Kuahjdcje\\_230515_STGs\\SDL_OpenGL\\Minecraft\\BlockSource\\TestBlock.uba\n";
            if (uba.is_open()) {
                char prefix[4];
                uba.read(prefix, 4);
                for (const char *ch_0{BlockTex::FILE_SUFFIX}, *ch_1{prefix}; ch_0 - BlockTex::FILE_SUFFIX < 4; ++ch_0, ++ch_1) {
                    if (*ch_0 != *ch_1) {
                        return;
                    }
                }
                for (uint8_t face{0U}; face < 6U; ++face) {
                    this->data[face] = new uint8_t[BlockTex::SIZE];
                    uba.read(reinterpret_cast<char *>(this->data[face]), BlockTex::SIZE);
                }
            }
        }
        ~BlockTex() {
            for (uint8_t face{0U}; face < 6U; ++face) {
                delete[] this->data[face];
            }
        }
    };
    const uint32_t RESOLUTION{64};
    const uint32_t BlockTex::SIZE{RESOLUTION * RESOLUTION * 4};
    const char BlockTex::FILE_SUFFIX[4]{"UBA"};

    BlockTex test_block_btx("BlockSource\\TestBlock.uba");
}

#endif
