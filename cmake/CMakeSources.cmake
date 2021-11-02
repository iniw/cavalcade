set(SOURCES

    # --- cavalcade ---
    # gui
    src/cavalcade/gui/objects/window/window.cpp
    src/cavalcade/gui/gui.cpp
    
    src/cavalcade/ctx/ctx.cpp
    
    
    src/cavalcade/cavalcade.cpp
    
    src/cavalcade/hack/other/prediction.cpp
    
    src/cavalcade/hooks/functions/base_player.cpp
    src/cavalcade/hooks/functions/chlc_client.cpp
    src/cavalcade/hooks/functions/d3d9_device.cpp
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
    # render
    src/other/render/imgui/misc/freetype/imgui_freetype.cpp
    src/other/render/imgui/imgui_draw.cpp
    src/other/render/imgui/imgui_impl_dx9.cpp
    src/other/render/imgui/imgui_tables.cpp
    src/other/render/imgui/imgui_widgets.cpp
    src/other/render/imgui/imgui.cpp

    src/other/render/geometry/rect/rect.cpp
    src/other/render/geometry/rect/rect_filled.cpp
    src/other/render/geometry/font/font.cpp
    src/other/render/render.cpp

    # --- sdk ---
    src/sdk/csgo/csgo.cpp
    src/sdk/netvars/netvars.cpp

    src/sdk/entities/cs_player.cpp

    # --- dllmain ---
    src/dllmain.cpp   
)