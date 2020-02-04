#bin/sh

if [ $# = 0 ]; then 
    echo "$(tput setaf 3)Usage : helper.sh <command>"
    echo "$(tput setaf 7)where command is one of build, test, clear, coverage and switch\n"
    echo "$(tput bold) build$(tput sgr0) - Create a build dir and copy executables in run dir"
    echo "$(tput bold) test$(tput sgr0)  - Run all the test of the program"
    echo "$(tput bold) clear$(tput sgr0) - Clean the projet (del: /build)"
    echo "$(tput bold) coverage$(tput sgr0) - Run coverage test"
    echo "$(tput bold) switch$(tput sgr0) <V1|V2> - Change version of the project"
else
    if [ "$1" = "build" ]
    then
        if [ ! -d "./build" ]; then
            mkdir build
            (cd build && cmake .. && make)
        else
            (cd build && make)
        fi

    elif [ "$1" = "test" ]
    then
        if [ ! -d "./build" ]; then 
            ./helper.sh build
        fi

        (cd build && ctest .)

    elif [ "$1" = "clear" ] || [ "$1" = "clean" ]
    then
        if [ -d "./build" ]; then
            rm -r ./build
        fi

        echo "$(tput setaf 2)Clean done !"
        
    elif [ "$1" = "coverage" ]
    then
        if [ ! -d "./build" ]; then 
            ./helper.sh build
        fi

        (cd build && ctest && make ExperimentalCoverage)

    elif [ "$1" = "switch" ]
    then
        if [ "$2" = "v1" ] || [ "$2" = "V1" ]; then 
            git checkout v1
        elif [ "$2" = "v2" ] || [ "$2" = "V2" ]; then
            git checkout master
        else
            echo "Usage : ./helper.sh switch <v1|v2>"
        fi
    else
        ./helper.sh
    fi
fi