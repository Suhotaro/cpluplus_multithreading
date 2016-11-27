CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

SET(SKIP_DIR CMakeFiles )
SET( MY_CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR} )

#macro declaration
MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    foreach(child ${children})
        if(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        endif()
    endforeach()
    SET(${result} ${dirlist})
ENDMACRO()

#macro invokation
SUBDIRLIST(SUBDIRS ${MY_CURRENT_DIR} )

MESSAGE( STATUS "SUBDIRS are:         " ${SUBDIRS} )

#iterate over found directories
foreach(subdir ${SUBDIRS})
    if ( NOT "${subdir}" STREQUAL "${SKIP_DIR}" )
        MESSAGE( STATUS "DIR is:         " ${subdir} )
        ADD_SUBDIRECTORY(${subdir})
    else()
        MESSAGE( STATUS "continue" )
    endif()
endforeach()
