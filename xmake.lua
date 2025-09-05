set_project("unknown")
set_languages("cxx20")
add_rules("mode.debug", "mode.release")

target("unknown")
    -- Qt integráció (moc/uic/rcc) a beállított Qt SDK-val
    add_rules("qt.widgetapp")
    set_kind("binary")

    add_files("src/**.cpp")
    add_headerfiles("include/**.h")
    add_includedirs("include")

    if os.isdir("ui") then
        add_files("ui/**.ui")
    end
    if os.isdir("resources") then
        add_files("resources/**.qrc")
    end
