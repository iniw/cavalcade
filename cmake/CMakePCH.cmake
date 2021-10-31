set(PCH

    # --- c stdlib ---
    <cstdint>
    <cstddef>
    <cstdlib>
    <cstdio>
    <cmath>
    <cstring>

    # --- cpp stdlib ---
    <limits>
    <memory>
    <utility>
    <algorithm>
    <chrono>
    <functional>
    <filesystem>
    <fstream>
    <compare>
    <bit>
    <numbers>
    <thread>
    # string
    <string>
    <string_view>
    # concepts
    <concepts>
    <type_traits>
    # containers
    <unordered_map>
    <vector>
    <array>

    # --- winapi ---
    <Windows.h>
    <WinUser.h>
    <Winternl.h>
    <intrin.h>
    <shlobj_core.h>

    # --- libraries ---
    src/other/lib/include/lazyimporter/lazyimporter.hpp
    src/other/lib/include/xorstr/xorstr.hpp
    src/other/lib/include/stb/stb.hpp
    
    <fmt/core.h>
    <fmt/format.h> 
    <fmt/chrono.h>
    <fmt/ranges.h>
    <fmt/color.h>

    <polyhook2/Detour/x86Detour.hpp>
    <polyhook2/CapstoneDisassembler.hpp>

    # --- other ---
    src/other/primitives/primitives.hpp

    src/cavalcade/cavalcade.hpp
    
    src/other/macros/macros.hpp
    src/other/hash/hash.hpp # ensures that hash::SEED is the same across translation units    
    
    # these two aren't necessary, i just want every file to have quick access to them;
    # mostly for debug purposes
    src/other/io/io.hpp
    src/other/utils/utils.hpp

    
)