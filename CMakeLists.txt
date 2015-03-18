cmake_minimum_required(VERSION 2.6)
project(libqtoc C)

# Create the binary directory for makefile output and copy over
# a test q script.
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
file(COPY "src/makeprint.q" DESTINATION "${CMAKE_SOURCE_DIR}/bin")

set(PROJ_SOURCES "src/makeq.c" "src/printq.c")

# Set some operating system dependent flags to be passed to the
# compilers.
if(UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -fpic -std=gnu99")
elseif(WIN32)
    set(LINK_LIBS "${CMAKE_SOURCE_DIR}/src/w32/q.lib")
    set(PROJ_SOURCES ${PROJ_SOURCES} "src/w32/libqtoc.def")
endif()

# Define how the library should be linked to the other objects.
add_library(libqtoc SHARED ${PROJ_SOURCES})
target_link_libraries(libqtoc ${LINK_LIBS})
set_target_properties(libqtoc PROPERTIES PREFIX "")

# Move the finished binaries into the binary folder if installing.
INSTALL(TARGETS libqtoc DESTINATION "${CMAKE_SOURCE_DIR}/bin")
