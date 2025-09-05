set_project("unknown")
set_languages("cxx20")
add_rules("mode.debug", "mode.release")

-- Qt: rendszerre telepített (QTDIR-ből jön, NEM csomag)
-- ezt már beállítottuk: xmake g --qt="C:/Qt/6.9.2/mingw_64"

-- WebSocket: Xmake kezeli/letölti
add_requires("ixwebsocket", {configs = {ssl = "openssl"}})

-- (opcionális) JSON, ha használod a kódban
add_requires("nlohmann_json", {optional = true})

target("unknown")
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

    -- Qt csomagot NEM kérünk (system-ből jön)
    add_packages("ixwebsocket")
    if has_package("nlohmann_json") then
        add_packages("nlohmann_json")
    end
