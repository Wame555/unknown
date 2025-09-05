-- xmake.lua (javított)
set_project("unknown")
set_languages("cxx20")
add_rules("mode.debug", "mode.release")

-- IXWebSocket TLS: mbedTLS (nem OpenSSL, így nincs perl/windres szívás)
add_requires("ixwebsocket", {configs = {ssl = "mbedtls"}})

-- Opcionális: ha tényleg használod
add_requires("nlohmann_json", {optional = true})

target("unknown")
    -- Qt a rendszerből jön (QTDIR), nem csomag
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

    add_packages("ixwebsocket")
    if has_package("nlohmann_json") then
        add_packages("nlohmann_json")
    end
