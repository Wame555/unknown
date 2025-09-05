-- XMake build konfiguráció a Qt6 + MinGW projekthez

add_rules("mode.debug", "mode.release")  -- Debug/Release módok támogatása

-- Külső függőségek deklarálása és beállítása
add_requires("ixwebsocket 11.4.6", { configs = { ssl = "mbedtls" } })   -- IXWebSocket 11.4.6, mbedtls SSL-támogatással
add_requires("libcurl", { configs = { with_ssl = "schannel" } })       -- libcurl (WinSSL/Schannel használata)
add_requires("nlohmann_json")                                         -- nlohmann_json (JSON könyvtár, header-only)

-- Cél (target) beállítása
target("unknown")                                -- Alkalmazás neve (állítsd a projekt nevére, ha szükséges)
    set_kind("binary")                           -- Futatható bináris
    add_rules("qt.widgetapp")                    -- Qt Widgets alkalmazás szabályai (moc, UI fordítás stb.):contentReference[oaicite:0]{index=0}
    add_files("src/*.cpp")                       -- Forrásfájlok (állítsd be a megfelelő elérési utat)
    add_headerfiles("src/*.h")                   -- Fejlécfájlok becsomagolása (opcionális)
    add_packages("ixwebsocket", "libcurl", "nlohmann_json")  -- Külső csomagok hozzáadása a buildhez

    -- Qt specifikus fájlok (ha vannak ilyenek a projektben):
    add_files("src/ui/*.ui")            -- Qt Designer által generált .ui fájlok, ha használ a projekt
    add_files("resources/*.qrc")        -- Qt resource file-ok (.qrc), ha vannak
    -- Amennyiben vannak Q_OBJECT makrót tartalmazó osztály-headerek:
    add_files("src/*.h") { rule = "qt.moc" }  
    -- A fenti sor automatikusan lefuttatja a moc-t az összes headerre; 
    -- alternatív megoldásként egyenként is megadhatók azok a header fájlok, amelyek Q_OBJECT-et tartalmaznak.
