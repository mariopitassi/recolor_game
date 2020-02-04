#bin/sh

me=`basename "$0"`

if [ $# = 0 ]; then 
    echo "$(tput setaf 3)Usage : helper.sh <command>"
    echo "$(tput setaf 7)where command is one of build, test, clear, coverage\n"
    echo "build - Create a build dir and copy executables in run dir"
    echo "test - Run all the test of the program"
    echo "clear - Clean the projet (del: /build)"
    echo "coverage - Run coverage test"
else
    if [ $1 = "build" ]
    then
        if [ ! -d "./build" ]; then
            mkdir build
            (cd build && cmake .. && make)
        else
            (cd build && make)
        fi

    elif [ $1 = "test" ]
    then
        if [ ! -d "./build"]; then 
            me build
        fi

        (cd build && ctest .)

    elif [ $1 = "clear" ] || [ $1 = "clean" ]
    then
        if [ -d "./build" ]; then
            rm -r ./build
        fi

        echo "$(tput setaf 2)Clean done !"
        
    elif [ $1 = "coverage" ]
    then
        if [ ! -d "./build" ]; then 
            ./helper.sh build
        fi

        (cd build && ctest && make ExperimentalCoverage)
    else
        me
    fi
fi