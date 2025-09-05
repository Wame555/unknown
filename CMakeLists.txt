cmake_minimum_required(VERSION 3.16)
project(CryptoBot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets Network REQUIRED)
find_package(ixWebSocket REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(CURL REQUIRED libcurl)

add_executable(CryptoBot
    src/main.cpp
    src/mainwindow.cpp
    src/demoaccount.cpp
    src/binancewebsocket.cpp
    src/telegramnotifier.cpp
    src/configmanager.cpp
)

target_include_directories(CryptoBot PRIVATE include)
target_link_libraries(CryptoBot PRIVATE
    Qt6::Widgets
    Qt6::Network
    ixWebSocket::ixWebSocket
    nlohmann_json::nlohmann_json
    ${CURL_LIBRARIES}
)

target_compile_definitions(CryptoBot PRIVATE QT_DEPRECATED_WARNINGS)
