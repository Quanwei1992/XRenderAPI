add_executable(osx_gl MACOSX_BUNDLE 
main.mm 
AppDelegate.mm
AppDelegate.h
GLView.mm
GLView.h
)

FIND_LIBRARY(COCOA_LIBRARY Cocoa)
FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
target_link_libraries(osx_gl ${COCOA_LIBRARY} ${OPENGL_LIBRARY})