cmake_minimum_required(VERSION 3.21)
project(CryptoBot LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (MSVC)
  add_compile_options(/permissive- /Zc:preprocessor /utf-8)
endif()

# ---- Könyvtárak (CONFIG mód) ----
find_package(Qt6 6.5 COMPONENTS Widgets Network REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# CURL: próbáljuk CONFIG-ot, ha nincs, fallback a FindCURL-re
find_package(CURL CONFIG QUIET)
if (NOT CURL_FOUND)
  find_package(CURL REQUIRED) # adja: CURL::libcurl
endif()

# ixwebsocket: először CONFIG (vcpkg), ha nincs, FetchContent fallback
include(FetchContent)
set(IXWS_TARGET "")

find_package(ixwebsocket CONFIG QUIET)
if (TARGET ixwebsocket::ixwebsocket)
  set(IXWS_TARGET ixwebsocket::ixwebsocket)
elseif (TARGET ixWebSocket::ixWebSocket)
  set(IXWS_TARGET ixWebSocket::ixWebSocket)
else()
  message(STATUS "ixwebsocket CMake config nem található – forrásból építjük (FetchContent).")
  # Stabil tagot tegyél ide, ha szeretnéd (pl. v11.5.x); default: master/main
  FetchContent_Declare(ixwebsocket
    GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
    GIT_TAG        master
  )
  set(IXWEBSOCKET_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(IXWEBSOCKET_USE_TLS      OFF CACHE BOOL "" FORCE) # állítsd ON-ra, ha kell OpenSSL
  FetchContent_MakeAvailable(ixwebsocket)

  if (TARGET ixwebsocket::ixwebsocket)
    set(IXWS_TARGET ixwebsocket::ixwebsocket)
  elseif (TARGET ixWebSocket::ixWebSocket)
    set(IXWS_TARGET ixWebSocket::ixWebSocket)
  else()
    message(FATAL_ERROR "ixwebsocket target nem jött létre.")
  endif()
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

target_link_libraries(CryptoBot PRIVATE
    Qt6::Widgets
    Qt6::Network
    nlohmann_json::nlohmann_json
    ${IXWS_TARGET}
    CURL::libcurl
)

target_compile_definitions(CryptoBot PRIVATE QT_DEPRECATED_WARNINGS)

if (WIN32 AND CMAKE_BUILD_TYPE MATCHES "Release")
  set_target_properties(CryptoBot PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
