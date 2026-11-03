cmake_minimum_required(VERSION 3.16)

project(WISArD LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# The Geant4 makefile setup stores G4INSTALL at the geant4make directory.
# Add the Geant4 installation prefix so find_package can locate its CMake files.
if(DEFINED ENV{G4INSTALL})
  get_filename_component(_g4_make_dir "$ENV{G4INSTALL}" ABSOLUTE)
  get_filename_component(_g4_prefix "${_g4_make_dir}/../../.." ABSOLUTE)
  list(APPEND CMAKE_PREFIX_PATH "${_g4_prefix}")
endif()

if(DEFINED ENV{ROOTSYS})
  list(APPEND CMAKE_PREFIX_PATH "$ENV{ROOTSYS}")
endif()

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

find_package(ROOT CONFIG REQUIRED COMPONENTS Core RIO Tree TreePlayer Hist Graf Graf3d)
include(${ROOT_USE_FILE})

add_subdirectory(FastModel)

file(GLOB WISARD_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
)
file(GLOB WISARD_HEADERS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hh"
)

add_executable(wisard
  wisard.cc
  ${WISARD_SOURCES}
  ${WISARD_HEADERS}
)

# Build ROOT collection dictionaries once at compile time, rather than asking
# Cling to generate and compile them each time the simulation starts.
get_target_property(WISARD_CLHEP_INCLUDE_DIRS CLHEP::CLHEP
  INTERFACE_INCLUDE_DIRECTORIES
)
add_library(WisardRootDictionaries SHARED)
target_include_directories(WisardRootDictionaries PRIVATE
  ${Geant4_INCLUDE_DIRS}
  ${WISARD_CLHEP_INCLUDE_DIRS}
)
target_link_libraries(WisardRootDictionaries PRIVATE
  ROOT::Core
  ROOT::RIO
  ROOT::Tree
  CLHEP::CLHEP
)
ROOT_GENERATE_DICTIONARY(G__WisardRootDictionaries
  "${CMAKE_CURRENT_SOURCE_DIR}/include/WisardRootDictionaryTypes.hh"
  LINKDEF "${CMAKE_CURRENT_SOURCE_DIR}/include/WisardRootLinkDef.h"
  MODULE WisardRootDictionaries
)

target_include_directories(wisard PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(wisard PRIVATE
  FastModel::UniformField
  WisardRootDictionaries
  ${Geant4_LIBRARIES}
  ROOT::Core
  ROOT::RIO
  ROOT::Tree
  ROOT::TreePlayer
  ROOT::Hist
  ROOT::Graf
  ROOT::Graf3d
)

set_target_properties(wisard PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
  INSTALL_RPATH "$ORIGIN/../lib"
  INSTALL_RPATH_USE_LINK_PATH TRUE
)

# Keep the previous PATH-based launch location in sync for existing scripts.
add_custom_command(TARGET wisard POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/bin/Linux-g++"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:wisard>"
    "${CMAKE_CURRENT_SOURCE_DIR}/bin/Linux-g++/wisard"
  VERBATIM
)

install(TARGETS wisard RUNTIME DESTINATION bin)
install(TARGETS WisardRootDictionaries LIBRARY DESTINATION lib)
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/libWisardRootDictionaries_rdict.pcm"
  "${CMAKE_CURRENT_BINARY_DIR}/libWisardRootDictionaries.rootmap"
  DESTINATION lib
)
