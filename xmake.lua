set_project("unknown")
set_languages("cxx20")
add_rules("mode.debug", "mode.release")

add_requires("qt 6.9.2", {configs = {widgets = true}})
add_requires("nlohmann_json")

target("unknown")
    add_rules("qt.widgetapp")
    set_kind("binary")
    add_files("src/**.cpp", "ui/**.ui", "resources/**.qrc")
    add_headerfiles("include/**.h")
    add_includedirs("include")
    add_packages("qt", "nlohmann_json")
