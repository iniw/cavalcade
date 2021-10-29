set(SOURCES

    # --- cavalcade ---
    src/cavalcade/ctx/ctx.cpp
    src/cavalcade/cavalcade.cpp
    
    src/cavalcade/hack/other/prediction.cpp
    
    src/cavalcade/hooks/functions/base_player.cpp
    src/cavalcade/hooks/hooks.cpp

    # --- other ---
    # hash
    src/other/hash/md5/md5.cpp
    # io
    src/other/io/console/console.cpp
    src/other/io/files/files.cpp
    src/other/io/input/input.cpp
    src/other/io/io.cpp
    # mem
    src/other/mem/module_info/module_info.cpp
    src/other/mem/mem.cpp
    # utils
    src/other/utils/str/str.cpp
    src/other/utils/utils.cpp
    # math
    src/other/math/math.cpp

    # --- sdk ---
    src/sdk/csgo/csgo.cpp
    src/sdk/netvars/netvars.cpp

    src/sdk/entities/cs_player.cpp

    # --- dllmain ---
    src/dllmain.cpp
    
)