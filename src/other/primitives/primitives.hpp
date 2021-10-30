#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

using i8  = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using f32 = float;
using f64 = double;

using ptr     = std::uintptr_t;
using ptrdiff = std::ptrdiff_t;

using str   = char*;
using cstr  = const char*;
using wstr  = wchar_t*;
using cwstr = const wchar_t*;

using unk = void*;

// are these healthy?
namespace fs  = std::filesystem;
namespace chr = std::chrono;

#endif // PRIMITIVES_HPP