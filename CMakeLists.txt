cmake_minimum_required(VERSION 2.8)


# Define project name
project(blackmagic-test)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules")

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost REQUIRED COMPONENTS thread)
set(OpenCV_SHARED ON)
set(OpenCV_STATIC OFF)
find_package(OpenCV REQUIRED)
find_package(DeckLink REQUIRED)

link_directories(${Boost_LIBRARY_DIR_RELEASE} ${Boost_LIBRARY_DIR_DEBUG})

add_executable(blackmagic_test
    src/DeckLinkCapture.cpp
    src/DeckLinkInputCallback.cpp
    src/DeckLinkOpenCv.cpp
    src/DeckLinkCreateInstance.cpp
    src/CCoInitializer.cpp
    src/main.cpp
)

set_target_properties(${OpenCV_LIBS} PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE)
set_target_properties(${OpenCV_LIBS} PROPERTIES MAP_IMPORTED_CONFIG_MINSIZEREL RELEASE)

#set_target_properties(${OpenCV_LIBS} IlmImf zlib libtiff libjpeg libwebp libjasper libpng PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE)
#set_target_properties(${OpenCV_LIBS} IlmImf zlib libtiff libjpeg libwebp libjasper libpng PROPERTIES MAP_IMPORTED_CONFIG_MINSIZEREL RELEASE)

target_link_libraries(blackmagic_test ${OpenCV_LIBS})

include (CMakeModules/UseDeckLink.cmake)

target_link_libraries(blackmagic_test 
    ${DeckLink_LIBRARIES} 
    -ldl 
    -pthread
)

include_directories(${Boost_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS} ${DeckLink_INCLUDE_DIR})
target_link_libraries(blackmagic_test 
    ${BOOST_Libraries} 
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
)




if (WIN32)
 target_link_libraries(blackmagic_test comsupp.lib ole32.lib)
endif(WIN32)

#cmake_policy(PUSH)
#cmake_policy(SET CMP0026 OLD)
#cmake_policy(SET CMP0053 OLD)
#include(CreateLaunchers)
#guess_runtime_library_dirs(runtime_lib_dirs OpenCV Tesseract)
#create_default_target_launcher(blackmagic_test
#	RUNTIME_LIBRARY_DIRS "${runtime_lib_dirs};${OpenCV_LIB_DIR}/../bin;"
#)
#cmake_policy(POP)
#
#

if(MSVC)
#We statically link to reduce dependancies
#foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
#    if(${flag_var} MATCHES "/MD")
#        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
#    endif(${flag_var} MATCHES "/MD")
#    if(${flag_var} MATCHES "/MDd")
#        string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
#    endif(${flag_var} MATCHES "/MDd")
#endforeach(flag_var)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(MSVC)
