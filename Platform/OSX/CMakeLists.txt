add_executable(osx_gl MACOSX_BUNDLE osx_gl.mm AppDelegate.mm)

FIND_LIBRARY(COCOA_LIBRARY Cocoa)
target_link_libraries(osx_gl ${COCOA_LIBRARY})