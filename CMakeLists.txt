cmake_minimum_required(VERSION 3.20)

project(arkanoid LANGUAGES CXX)

option(LORD_ARKANOID_WERROR "Treat warnings as errors" OFF)
option(LORD_ARKANOID_UNITY  "Unity build" OFF)
option(LORD_ARKANOID_LTO    "Enable LTO in Release" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJ_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# SFML
find_package(SFML 2.6 CONFIG REQUIRED COMPONENTS system window graphics audio)

# src
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJ_ROOT}/src/*.cpp")
if (NOT SOURCES)
  message(FATAL_ERROR "No .cpp files found in ${PROJ_ROOT}/src")
endif()

add_executable(arkanoid ${SOURCES})

# include
target_include_directories(arkanoid PRIVATE
  ${PROJ_ROOT}/include
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
source_group(TREE ${PROJ_ROOT} FILES ${SOURCES})

# warn
if (MSVC)
  target_compile_options(arkanoid PRIVATE /W4 /permissive- /Zc:preprocessor /EHsc /external:W0 /external:anglebrackets)
  if (LORD_ARKANOID_WERROR)
    target_compile_options(arkanoid PRIVATE /WX)
  endif()
else()
  target_compile_options(arkanoid PRIVATE -Wall -Wextra -Wpedantic)
  if (LORD_ARKANOID_WERROR)
    target_compile_options(arkanoid PRIVATE -Werror)
  endif()
endif()

set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/resources/assets")
target_compile_definitions(arkanoid PRIVATE
  NOMINMAX
  _CRT_SECURE_NO_WARNINGS
  ASSETS_DIR="${ASSETS_DIR}"
)

# SFML linking
target_link_libraries(arkanoid PRIVATE sfml-system sfml-window sfml-graphics sfml-audio)

add_custom_command(TARGET arkanoid POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${ASSETS_DIR}" "$<TARGET_FILE_DIR:arkanoid>/assets"
)

# SFML DLL 
if (WIN32 AND NOT SFML_USE_STATIC_LIBS AND DEFINED SFML_DIR)
  get_filename_component(_sfml_prefix "${SFML_DIR}/../../.." ABSOLUTE)
  add_custom_command(TARGET arkanoid POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${_sfml_prefix}/bin" "$<TARGET_FILE_DIR:arkanoid>"
  )
endif()

set_property(TARGET arkanoid PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:arkanoid>")
set_property(TARGET arkanoid PROPERTY VS_DEBUGGER_ENVIRONMENT "PATH=$<TARGET_FILE_DIR:arkanoid>;%PATH%")

if (LORD_ARKANOID_UNITY)
  set_target_properties(arkanoid PROPERTIES UNITY_BUILD ON)
endif()

if (LORD_ARKANOID_LTO)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT _ipo_supported OUTPUT _ipo_err)
  if(_ipo_supported)
    set_property(TARGET arkanoid PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
  else()
    message(WARNING "IPO/LTO not supported: ${_ipo_err}")
  endif()
endif()