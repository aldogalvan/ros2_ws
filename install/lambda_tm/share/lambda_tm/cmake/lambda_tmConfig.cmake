# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_lambda_tm_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED lambda_tm_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(lambda_tm_FOUND FALSE)
  elseif(NOT lambda_tm_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(lambda_tm_FOUND FALSE)
  endif()
  return()
endif()
set(_lambda_tm_CONFIG_INCLUDED TRUE)

# output package information
if(NOT lambda_tm_FIND_QUIETLY)
  message(STATUS "Found lambda_tm: 0.0.0 (${lambda_tm_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'lambda_tm' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${lambda_tm_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(lambda_tm_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${lambda_tm_DIR}/${_extra}")
endforeach()
