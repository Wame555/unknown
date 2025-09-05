set_project("unknown")
set_languages("cxx20")
add_rules("mode.debug", "mode.release")

-- SYSTEM Qt (a gépeden lévő, nem letöltött)
add_requires("qt", {system = true, configs = {qt = "6", widgets = true}})
add_requires("nlohmann_json", {optional = true})

target("unknown")
    add_rules("qt.widgetapp")
    set_kind("binary")
    add_files("src/**.cpp")
    add_headerfiles("include/**.h")
    add_includedirs("include")
    if os.isdir("ui") then add_files("ui/**.ui") end
    if os.isdir("resources") then add_files("resources/**.qrc") end
    add_packages("qt")
    if has_package("nlohmann_json") then add_packages("nlohmann_json") end
