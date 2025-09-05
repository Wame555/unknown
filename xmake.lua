-- Projekt + nyelv
set_project("unknown")
set_languages("cxx20")
add_rules("mode.debug", "mode.release")

-- --- Opcionális: Qt6 támogatás (ha van GUI, moc/uic/rcc kell) ---
-- Vedd ki a kommentet, ha Qt6-ot használsz.
add_requires("qt", {configs = {qt = "6", widgets = true}})

-- --- Közkedvelt, fejfájásmentes JSON lib (header-only) ---
-- Ha a kód nlohmann_json-t használja:
add_requires("nlohmann_json")

target("unknown")
    -- Ha Qt-ot használsz, cseréld erre:
    add_rules("qt.widgetapp")

    set_kind("binary")
    add_files("src/**.cpp")
    add_headerfiles("include/**.h")
    add_includedirs("include")

    -- Ha Qt kell:
     add_packages("qt")

    -- Ha nlohmann_json kell:
     add_packages("nlohmann_json")

    -- Windows GUI app esetén (ne nyisson konzolt):
     if is_plat("windows") then
        add_ldflags("/SUBSYSTEM:WINDOWS")
     end

    -- Ha vannak .ui vagy .qrc fájlok:
    add_files("ui/**.ui", "resources/**.qrc")
