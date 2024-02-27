# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_force_dimension_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED force_dimension_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(force_dimension_FOUND FALSE)
  elseif(NOT force_dimension_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(force_dimension_FOUND FALSE)
  endif()
  return()
endif()
set(_force_dimension_CONFIG_INCLUDED TRUE)

# output package information
if(NOT force_dimension_FIND_QUIETLY)
  message(STATUS "Found force_dimension: 0.2.0 (${force_dimension_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'force_dimension' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${force_dimension_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(force_dimension_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${force_dimension_DIR}/${_extra}")
endforeach()
