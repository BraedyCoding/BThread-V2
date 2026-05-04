#pragma once
#include <cstdint>

//******************* ABI types *************************//

#ifdef __x86_64__
using reg_t = uint64_t;
inline constexpr std::size_t offset = 8;
#else
using reg_t = uint32_t;
inline constexpr std::size_t offset = 4;
#endif