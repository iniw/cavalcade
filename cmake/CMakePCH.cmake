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
    <bitset>
    <numbers>
    <thread>
    <optional>
    <variant>
    <ranges>
    <any>
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
    # data
    <source_location>

    # --- winapi ---
    <Windows.h>
    <Windowsx.h>
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

    src/other/mocking_bird/mocking_bird.hpp

    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamapplist.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamapps.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamappticket.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamclient.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamcontroller.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamfriends.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamgamecoordinator.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamgameserver.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamgameserverstats.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamhtmlsurface.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamhttp.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteaminput.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteaminventory.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteammatchmaking.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteammusic.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteammusicremote.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamnetworking.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamnetworkingmessages.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamnetworkingsockets.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamnetworkingutils.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamparentalsettings.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamremoteplay.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamremotestorage.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamscreenshots.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamugc.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamuser.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamuserstats.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamutils.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/isteamvideo.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/matchmakingtypes.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/steamclientpublic.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/steamencryptedappticket.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/steamhttpenums.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/steamnetworkingfakeip.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/steamnetworkingtypes.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/steamps3params.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/steamtypes.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/steamuniverse.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/steam_api.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/steam_api_common.h"
    "${PROJECT_SOURCE_DIR}/src/other/lib/include/steam/steam_gameserver.h"

    
)