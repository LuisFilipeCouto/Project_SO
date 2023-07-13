set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/lib/grp)

add_library(grp_freeinodes STATIC
    grp_alloc_inode.cpp
    grp_free_inode.cpp
)

