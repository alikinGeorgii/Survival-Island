cmake_minimum_required(VERSION 3.16)
project(SurvivalGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # чтобы clangd/редактор видел заголовки

find_package(SFML 3 REQUIRED COMPONENTS Graphics)

# все .cpp из папки files/, новые файлы подхватываются при пересборке
file(GLOB SOURCES CONFIGURE_DEPENDS files/*.cpp)

add_executable(game ${SOURCES})
target_include_directories(game PRIVATE files)
target_link_libraries(game PRIVATE SFML::Graphics)
