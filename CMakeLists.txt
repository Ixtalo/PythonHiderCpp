cmake_minimum_required (VERSION 3.12)
project (PythonHiderCpp)



## https://stackoverflow.com/questions/18968979/how-to-get-colorized-output-with-cmake
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(ColourBold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldGreen   "${Esc}[1;32m")
  set(BoldYellow  "${Esc}[1;33m")
  set(BoldBlue    "${Esc}[1;34m")
  set(BoldMagenta "${Esc}[1;35m")
  set(BoldCyan    "${Esc}[1;36m")
  set(BoldWhite   "${Esc}[1;37m")
endif()

message("--------------------------------------------------------------------")
message("${Red}If you changed your Python code and/or generated a new data.h \nthen do a 'make clean all'!${ColourReset}")
message("--------------------------------------------------------------------")


## https://cmake.org/cmake/help/latest/module/FindPython3.html
find_package (Python3 COMPONENTS Interpreter Development)
#execute_process(COMMAND python3-config --cflags OUTPUT_VARIABLE Python3_CFLAGS)
#execute_process(COMMAND python3-config --ldflags OUTPUT_VARIABLE Python3_LDFLAGS)
message("Python3_LIBRARY:            ${Python3_LIBRARY}")
message("Python3_INCLUDE_DIRS:       ${Python3_INCLUDE_DIRS}")
#message("Python3 CFLAGS:             ${Python3_CFLAGS}")
#message("Python3 LDFLAGS:             ${Python3_LDFLAGS}")

add_library (AES ./AES/AES.cpp)


add_executable (gendatah gendatah.cpp)
target_link_libraries (gendatah AES)


if(NOT(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/data.h"))
    file(COPY "${PROJECT_SOURCE_DIR}/data.h" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif()




add_executable (runner runner.cpp)
target_include_directories(runner BEFORE PRIVATE ${CMAKE_CURRENT_BINARY_DIR} PUBLIC ${Python3_INCLUDE_DIRS})
target_link_libraries (runner AES ${Python3_LIBRARIES} )
