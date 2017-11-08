
# Windows with Direct3D9
add_executable(WindowWithDX9 win_dx9.cpp)
target_link_libraries(WindowWithDX9 d3d9.lib)

# Windows with OpenGL
add_executable(WindowWithGL win_gl.cpp)
