project(CQTTEST)

cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(SOURCE_FILES)
include(gl.cmake)
include(font.cmake)

find_package(GLFW3 REQUIRED)
find_package(FFTW3 REQUIRED COMPONENTS double)
find_package(PortAudio REQUIRED)
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLM REQUIRED)

file(COPY res DESTINATION ${CQTTEST_BINARY_DIR})
list(APPEND SOURCE_FILES Kern.cpp CQT.cpp BlockMatrix.cpp)
add_executable(CQTTest main.cpp ${SOURCE_FILES})
set_target_properties(CQTTest PROPERTIES COMPILE_DEFINITIONS "GLEW_STATIC=1 GLM_FORCE_RADIANS=1")
target_include_directories(CQTTest PUBLIC ${FREETYPE_INCLUDE_DIR_freetype2} ${FREETYPE_INCLUDE_DIR_ft2build} ${GLEW_INCLUDE_DIR} .)
add_custom_command(TARGET CQTTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/res"
        ${CQTTEST_BINARY_DIR})


set(LIBS ${GLFW3_LIBRARY} ${GLEW_LIBRARY} ${FREETYPE_LIBRARY} ${FFTW3_LIBRARY} ${PORTAUDIO_LIBRARY} ${GLFW3_STATIC_LIBRARIES})
if(WIN32)
	list(APPEND LIBS opengl32 gdi32 winmm uuid setupapi)
endif(WIN32)

target_link_libraries(CQTTest ${LIBS})