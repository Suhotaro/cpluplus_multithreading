#! /bin/bash

DIRS=`ls`

echo ${DIRS}

rm -rf CMakeFiles CMakeCache.txt cmake_install.cmake  Makefile

for dir in ${DIRS}
do
    echo "entering: ${dir}"

    if [ -d ${dir} ]; then

        echo "  clear all in: ${dir}"
        cd ${dir}
        rm -rf CMakeFiles CMakeCache.txt cmake_install.cmake  Makefile main

        SUDIRS=`ls`
        echo "  sub: ${SUBDIRS}"

        for subdir in ${SUBDIRS}
        do

            echo "  clear : ${subdir}"
            if [ -x ${subdir} ]; then
                rm ${subdir}
            fi
        done

        cd ..
    fi
done
