set_project("unknown")
set_languages("cxx20")
add_rules("mode.debug", "mode.release")

-- Függőségek
add_requires("ixwebsocket 11.4.6", { configs = { ssl = "mbedtls" } })
add_requires("libcurl", { configs = { with_ssl = "schannel" }, optional = true })
add_requires("nlohmann_json", { optional = true })

target("unknown")
    set_kind("binary")
    add_rules("qt.widgetapp")           -- kezeli uic\rcc-t is

    add_includedirs("include")
    add_headerfiles("include/**.h")
    add_files("src/**.cpp")

    if os.isdir("ui") then
        add_files("ui\**.ui")
    end
    if os.isdir("resources") then
        add_files("resources/**.qrc")
    end

    -- FONTOS: minden Q_OBJECT-es headerre fusson MOC
    -- konkrétan felsorolva:
    if os.isfile("include\binancewebsocket.h") then
        add_files("include\binancewebsocket.h", { rule = "qt.moc" })
    end
    if os.isfile("include\demoaccount.h") then
        add_files("include\demoaccount.h", { rule = "qt.moc" })
    end
    if os.isfile("include\configmanager.h") then
        add_files("include\configmanager.h", { rule = "qt.moc" })
    end
    if os.isfile("include\mainwindow.h") then
        add_files("include\mainwindow.h", { rule = "qt.moc" })
    end
    if os.isfile("include\telegramnotifier.h") then
        add_files("include\telegramnotifier.h", { rule = "qt.moc" })
    end
    -- és még egy biztosíték: ha máshol is van Q_OBJECT a projektben
    add_files("include\**.h", { rule = "qt.moc" })
    add_files("src\**.h",     { rule = "qt.moc" })

    -- Csomagok
    add_packages("ixwebsocket")
    if has_package("libcurl") then add_packages("libcurl") end
    if has_package("nlohmann_json") then add_packages("nlohmann_json") end
