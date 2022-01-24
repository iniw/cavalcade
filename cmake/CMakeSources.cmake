set(SOURCES

    # --- cavalcade ---
    # gui - other
    src/cavalcade/gui/managers/children/children.cpp
    # gui - objects
    src/cavalcade/gui/objects/base/base_parent.cpp
    src/cavalcade/gui/objects/base/base_object.cpp
    src/cavalcade/gui/objects/checkbox/checkbox.cpp
    src/cavalcade/gui/objects/column/column.cpp
    src/cavalcade/gui/objects/groupbox/groupbox.cpp
    src/cavalcade/gui/objects/scrollbar/scrollbar.cpp
    src/cavalcade/gui/objects/tab/tab.cpp
    src/cavalcade/gui/objects/window/window.cpp
    # gui
    src/cavalcade/gui/gui.cpp
    # ctx
    src/cavalcade/ctx/ctx.cpp
    src/cavalcade/entity_cacher/entity_cacher.cpp
    # cavalcade
    src/cavalcade/cavalcade.cpp
    # hack - other
    src/cavalcade/hack/other/prediction.cpp
    src/cavalcade/hack/translator/translator.cpp
    src/cavalcade/hack/trainer/trainer.cpp
    src/cavalcade/hack/aimbot/aimbot.cpp
    src/cavalcade/hack/aimbot/autowall/autowall.cpp
    src/cavalcade/hack/esp/esp.cpp
    src/cavalcade/hack/graph/graph.cpp
    src/cavalcade/hack/indicators/screen.cpp
    src/cavalcade/hack/nightmode/nightmode.cpp
    # hooks - functions
    src/cavalcade/hooks/functions/push_notice.cpp
    src/cavalcade/hooks/functions/client_cmd.cpp
    src/cavalcade/hooks/functions/base_player.cpp
    src/cavalcade/hooks/functions/prediction.cpp
    src/cavalcade/hooks/functions/chlc_client.cpp
    src/cavalcade/hooks/functions/d3d9_device.cpp
    src/cavalcade/hooks/functions/steam.cpp
    src/cavalcade/hooks/functions/sfhud.cpp
    src/cavalcade/hooks/functions/crosshair.cpp
    src/cavalcade/hooks/functions/entity_listener.cpp
    src/cavalcade/hooks/functions/static_prop_mgr.cpp
    src/cavalcade/hooks/functions/material_system.cpp
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
    src/other/lib/include/crc32/crc32.cpp
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
    src/other/render/geometry/rect/rect_grad.cpp
    src/other/render/geometry/line/line.cpp
    src/other/render/geometry/font/font.cpp
    src/other/render/geometry/text/text.cpp
    # render
    src/other/render/render.cpp

    # --- sdk ---
    src/sdk/csgo/csgo.cpp
    src/sdk/netvars/netvars.cpp
    src/sdk/entities/cs_player.cpp
    src/sdk/entities/player.cpp

    # --- dllmain ---
    src/dllmain.cpp   
)