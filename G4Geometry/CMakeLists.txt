cmake_minimum_required(VERSION 3.13)

project(IAXO-DETECTOR-GEOMETRY LANGUAGES CXX HOMEPAGE_URL https://github.com/iaxo/detector-geometry)

message("===============  ${PROJECT_NAME}  ==============")

SET(CMAKE_CXX_STANDARD 17)

# Find ROOT
find_package(ROOT REQUIRED COMPONENTS RIO Geom Eve)
execute_process(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CFLAGS)
string(STRIP ${ROOT_CFLAGS} ROOT_CFLAGS)
message("-- Found ROOT version: ${ROOT_VERSION} with compilation flags: ${ROOT_CFLAGS} and libraries: ${ROOT_LIBRARIES}")

# Find Geant4
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})
message("-- Found Geant4 version: ${Geant4_VERSION}")


file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)

add_executable(${PROJECT_NAME} main.cpp ${sources} ${headers})

target_link_libraries(
        ${PROJECT_NAME}
        ${ROOT_LIBRARIES} ROOT::Geom ROOT::RIO ROOT::Eve
        ${Geant4_LIBRARIES}
        ${REST_LIBRARIES}
)

target_include_directories(
        ${PROJECT_NAME} SYSTEM PUBLIC ${PROJECT_SOURCE_DIR}/include
        ${ROOT_INCLUDE_DIRS}
        ${Geant4_INCLUDE_DIR}
        ${REST_INCLUDE_DIRS}
)

target_compile_options(${PROJECT_NAME} PRIVATE)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)