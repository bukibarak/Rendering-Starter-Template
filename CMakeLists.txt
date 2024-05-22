cmake_minimum_required(VERSION 3.20)
project(TPs-Rendering)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*)
#add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_executable(Main2D src/main2D.cpp)
target_include_directories(Main2D PRIVATE src)
target_compile_features(Main2D PRIVATE cxx_std_20)

# Include lib
add_subdirectory(opengl-framework)
target_link_libraries(Main2D PRIVATE opengl_framework::opengl_framework)
gl_target_copy_folder(Main2D res)


add_executable(Main3D src/main3D.cpp)
target_include_directories(Main3D PRIVATE src)
target_compile_features(Main3D PRIVATE cxx_std_20)

# Include lib
target_link_libraries(Main3D PRIVATE opengl_framework::opengl_framework)
gl_target_copy_folder(Main3D res)
