CMAKE_MINIMUM_REQUIRED (VERSION 2.8)


# set temporaries names
SET(SRC_PATH src)
SET(EXECUTABLE threadsafe_lookup_table)

# store sources in SOURCES varable
SET (SOURCES main.cpp)

# check if a compiller supports c++11 standart

# as I understand it exports all supported compillers's flags into CheckCXXCompilerFlag variable
include(CheckCXXCompilerFlag)
# check if that variable contains flag we are intrested in
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(NOT COMPILER_SUPPORTS_CXX11)
	set_property(TARGET ${OPTIONS} APPEND_STRING PROPERTY COMPILE_FLAGS "-std=c++11 ")
endif()

# find pthread library on system we are building on
find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

# add additional compiller options
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11 -pthread -lboost_thread")

# store compillers flags we are going to use into OPTIONS varable
SET(OPTIONS -Wall -Werror -g2 -O0)

# compillers flags we are going to use
add_definitions(${OPTIONS})

# compile
add_executable(${EXECUTABLE} ${SOURCES} )

# add linkage flags -lpthread
target_link_libraries(${EXECUTABLE} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})

# specify directory the program will be installed to
INSTALL(PROGRAMS ${EXECUTABLE} 
        DESTINATION ${CMAKE_SOURCE_DIR}
)
