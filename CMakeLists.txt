cmake_minimum_required(VERSION 3.15)

project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "--std=c++1z")
add_executable(test test.cpp)

add_subdirectory(sources/abseil-cpp)
add_subdirectory(sources/re2-2023-09-01)



set(BOOST_INCLUDE_LIBRARIES asio)
set(BOOST_ENABLE_CMAKE ON)
include(FetchContent)
FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG boost-1.80.0
)
FetchContent_MakeAvailable(Boost)

target_link_libraries(test PRIVATE Boost::asio)
target_link_libraries(test PUBLIC re2::re2)


