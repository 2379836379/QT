# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\oj_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\oj_autogen.dir\\ParseCache.txt"
  "oj_autogen"
  )
endif()
