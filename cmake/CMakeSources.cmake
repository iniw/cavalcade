set(SOURCES

    # --- cavalcade ---
    # gui - managers
    src/cavalcade/gui/managers/children/children.cpp
    src/cavalcade/gui/managers/events/events.cpp
    # gui - objects
    src/cavalcade/gui/objects/base/base_parent.cpp
    src/cavalcade/gui/objects/base/base_object.cpp
    src/cavalcade/gui/objects/checkbox/checkbox.cpp
    src/cavalcade/gui/objects/column/column.cpp
    src/cavalcade/gui/objects/groupbox/groupbox.cpp
    src/cavalcade/gui/objects/tab/tab.cpp    
    src/cavalcade/gui/objects/window/window.cpp
    # gui
    src/cavalcade/gui/gui.cpp
    # ctx
    src/cavalcade/ctx/ctx.cpp
    # cavalcade
    src/cavalcade/cavalcade.cpp
    # hack - other
    src/cavalcade/hack/other/prediction.cpp
    # hooks - functions
    src/cavalcade/hooks/functions/base_player.cpp
    src/cavalcade/hooks/functions/chlc_client.cpp
    src/cavalcade/hooks/functions/d3d9_device.cpp
    # hooks
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
    # render - imgui
    src/other/render/imgui/misc/freetype/imgui_freetype.cpp
    src/other/render/imgui/imgui_draw.cpp
    src/other/render/imgui/imgui_impl_dx9.cpp
    src/other/render/imgui/imgui_tables.cpp
    src/other/render/imgui/imgui_widgets.cpp
    src/other/render/imgui/imgui.cpp
    # render - geometry
    src/other/render/geometry/rect/rect.cpp
    src/other/render/geometry/rect/rect_filled.cpp
    src/other/render/geometry/font/font.cpp
    # render
    src/other/render/render.cpp

    # --- sdk ---
    src/sdk/csgo/csgo.cpp
    src/sdk/netvars/netvars.cpp
    src/sdk/entities/cs_player.cpp

    # --- dllmain ---
    src/dllmain.cpp   
)