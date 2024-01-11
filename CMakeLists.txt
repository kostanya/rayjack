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

set(RAYJACK_TEST_SOURCES
    tests/test_utils.cpp
    tests/test_camera.cpp
    tests/test_material.cpp
    tests/test_sphere.cpp
    tests/test_hittable_list.cpp
    tests/test_hit_record.cpp
    tests/test_interval.cpp
    tests/test_color.cpp
    tests/test_ray.cpp
    tests/test-main.cpp

    external/stb_image.h
    external/stb_image.cpp)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/Catch2)
add_executable(rayjack-test ${RAYJACK_TEST_SOURCES})

target_include_directories(rayjack-lib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/rayjack)
target_include_directories(rayjack-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/rayjack/detail)
target_include_directories(rayjack-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external)         # stb
target_include_directories(rayjack-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/glm)     # glm
target_include_directories(rayjack-app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})                 # rayjack 
              
# include directories for rayjack-test
target_include_directories(rayjack-test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)
target_include_directories(rayjack-test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/rayjack)
target_include_directories(rayjack-test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/rayjack/detail)
target_include_directories(rayjack-test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external)         
target_include_directories(rayjack-test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/glm)     
target_include_directories(rayjack-test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/Catch2)  # catch2
target_include_directories(rayjack-test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})                 


if(NOT MSVC)
  target_compile_options(rayjack-app PRIVATE -Wall -Wextra -Wconversion -Wsign-conversion -pedantic-errors)

  if(CMAKE_CXX_COMPILER_ID MATCHES "GCC")
    target_compile_options(rayjack-lib PRIVATE -fprofile-arcs -ftest-coverage)
    target_compile_options(rayjack-test PRIVATE -fprofile-arcs -ftest-coverage)
    target_link_options(rayjack-lib PRIVATE -fprofile-arcs -ftest-coverage)
    target_link_options(rayjack-test PRIVATE -fprofile-arcs -ftest-coverage)
  endif()
endif()


target_compile_options(rayjack-app PRIVATE
    $<$<CONFIG:Debug>:-O0>
    $<$<CONFIG:Release>:-O3>
)

target_link_libraries(rayjack-app rayjack-lib)
target_link_libraries(rayjack-test PRIVATE rayjack-lib Catch2::Catch2WithMain)

enable_testing()
add_test(NAME RayjackTests COMMAND rayjack-test)
