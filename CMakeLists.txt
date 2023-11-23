cmake_minimum_required(VERSION 3.18)

project(rayjack LANGUAGES C CXX VERSION 0.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# put all runtime binaries in the root build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set (RAYJACK_LIB_SOURCES 
  rayjack/detail/color.h
  rayjack/detail/color.cpp
  rayjack/detail/hit_record.h
  rayjack/detail/hittable.h 
  rayjack/detail/interval.h 
  rayjack/detail/ray.h  
  rayjack/detail/utils.h
  rayjack/detail/render_thread.h
  rayjack/detail/render_thread.cpp
  rayjack/detail/render_thread_pool.h
  rayjack/detail/render_thread_pool.cpp

  rayjack/camera.h
  rayjack/camera.cpp
  rayjack/hittable_list.h  
  rayjack/hittable_list.cpp
  rayjack/material.h
  rayjack/material.cpp
  rayjack/sphere.h
  rayjack/sphere.cpp  

  external/stb_image_write.h
  external/stb_image_write.cpp)

add_library(rayjack-lib STATIC ${RAYJACK_LIB_SOURCES})

set (RAYJACK_APP_SOURCES
  app/application.cpp
)

add_executable(rayjack-app ${RAYJACK_APP_SOURCES})

target_include_directories(rayjack-lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/rayjack)
target_include_directories(rayjack-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/rayjack/detail)
target_include_directories(rayjack-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external)         # stb
target_include_directories(rayjack-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/glm)     # glm
target_include_directories(rayjack-app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})                 # rayjack 
              
target_compile_options(rayjack-app PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion -pedantic-errors)

target_compile_options(rayjack-app PRIVATE
    $<$<CONFIG:Debug>:-O0>
    $<$<CONFIG:Release>:-O3>
)

target_link_libraries(rayjack-app rayjack-lib)
