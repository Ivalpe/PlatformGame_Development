# Install script for directory: C:/Users/ivanap1/Downloads/PlatformGame-L9_Map_Collision_Solution/PlatformGame-L9_Map_Collision_Solution/vcpkg_installed/x64-windows/vcpkg/blds/box2d/src/64c16f51bc-7b27809cb5.clean

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/ivanap1/Downloads/PlatformGame-L9_Map_Collision_Solution/PlatformGame-L9_Map_Collision_Solution/vcpkg_installed/x64-windows/vcpkg/pkgs/box2d_x64-windows/debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "OFF")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/ivanap1/Downloads/PlatformGame-L9_Map_Collision_Solution/PlatformGame-L9_Map_Collision_Solution/vcpkg_installed/x64-windows/vcpkg/blds/box2d/src/64c16f51bc-7b27809cb5.clean/include/box2d")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/ivanap1/Downloads/PlatformGame-L9_Map_Collision_Solution/PlatformGame-L9_Map_Collision_Solution/vcpkg_installed/x64-windows/vcpkg/blds/box2d/x64-windows-dbg/src/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/ivanap1/Downloads/PlatformGame-L9_Map_Collision_Solution/PlatformGame-L9_Map_Collision_Solution/vcpkg_installed/x64-windows/vcpkg/blds/box2d/x64-windows-dbg/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
