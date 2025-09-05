set_project("unknown")
set_languages("cxx20")
add_rules("mode.debug", "mode.release")

-- fontos: zlib elöl
add_requires("zlib")

add_requires("ixwebsocket", {configs = {ssl = "mbedtls"}})
add_requires("libcurl", {configs = {with_ssl = "schannel"}, optional = true})
add_requires("nlohmann_json", {optional = true})

target("unknown")
  add_rules("qt.widgetapp")
  set_kind("binary")
  add_files("src/**.cpp")
  add_headerfiles("include/**.h")
  add_includedirs("include")
  if os.isdir("ui") then add_files("ui/**.ui") end
  if os.isdir("resources") then add_files("resources/**.qrc") end
  add_packages("zlib", "ixwebsocket")
  if has_package("libcurl") then add_packages("libcurl") end
  if has_package("nlohmann_json") then add_packages("nlohmann_json") end
