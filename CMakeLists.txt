cmake_minimum_required(VERSION 3.21)
project(CryptoBot LANGUAGES CXX)

# C++ és Qt automatikák
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# MSVC barát flag-ek (Windows)
if (MSVC)
  add_compile_options(/permissive- /Zc:preprocessor /utf-8)
endif()

# ---- Csomagok (CONFIG mód, vcpkg-hez igazítva) ----
find_package(Qt6 6.5 COMPONENTS Widgets Network REQUIRED)

# nlohmann_json (vcpkg: nlohmann-json)
find_package(nlohmann_json CONFIG REQUIRED)

# ixwebsocket (vcpkg: ixwebsocket) - különböző target nevek kezelése
# vcpkg exportálhatja így: ixwebsocket::ixwebsocket  (tipikus)
# vagy régebbi/nevesített módon: ixWebSocket::ixWebSocket
find_package(ixwebsocket CONFIG REQUIRED)

# CURL: próbáljuk CONFIG módban, ha nincs, esünk vissza a beépített FindCURL-re
find_package(CURL CONFIG QUIET)
if (NOT CURL_FOUND)
  find_package(CURL REQUIRED) # ekkor a cél általában CURL::libcurl
endif()

# ---- Források ----
add_executable(CryptoBot
    src/main.cpp
    src/mainwindow.cpp
    src/demoaccount.cpp
    src/binancewebsocket.cpp
    src/telegramnotifier.cpp
    src/configmanager.cpp
)

target_include_directories(CryptoBot PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# ixwebsocket cél kiválasztása névütközés esetére
set(IXWS_TARGET "")
if (TARGET ixwebsocket::ixwebsocket)
  set(IXWS_TARGET ixwebsocket::ixwebsocket)
elseif (TARGET ixWebSocket::ixWebSocket)
  set(IXWS_TARGET ixWebSocket::ixWebSocket)
else()
  message(FATAL_ERROR "ixwebsocket target nem található. Ellenőrizd, hogy a vcpkg-ben az 'ixwebsocket' csomag telepítve van.")
endif()

target_link_libraries(CryptoBot PRIVATE
    Qt6::Widgets
    Qt6::Network
    nlohmann_json::nlohmann_json
    ${IXWS_TARGET}
    CURL::libcurl
)

target_compile_definitions(CryptoBot PRIVATE QT_DEPRECATED_WARNINGS)

# Opcionális: ne nyisson külön konzol ablakot Release-ben Windows alatt
if (WIN32 AND CMAKE_BUILD_TYPE MATCHES "Release")
  set_target_properties(CryptoBot PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
