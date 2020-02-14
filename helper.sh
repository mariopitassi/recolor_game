#bin/sh

if [ $# = 0 ]; then 
    echo "$(tput setaf 3)Usage : helper.sh <command>"
    echo "$(tput setaf 7)where command is one of build, test, clear, coverage, memcheck and switch\n$(tput sgr0) "
    echo "$(tput bold)build$(tput sgr0) - Create a build dir and copy executables in run dir"
    echo "$(tput bold)test$(tput sgr0)  - Run all the test of the program"
    echo "$(tput bold)clear$(tput sgr0) - Clean the projet (del: /build)"
    echo "$(tput bold)coverage$(tput sgr0) - Run coverage test"
    echo "$(tput bold)switch$(tput sgr0) <V1|V2> - Change version of the project"
    echo "$(tput bold)memcheck$(tput sgr0) - Do a complete memory check"
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
    elif [ "$1" = "memcheck" ]
    then
        if [ ! -d "./build" ]; then 
            ./helper.sh build
        fi

        echo "$(tput bold)$(tput setaf 3)\nPress ENTER to run an ExperimentalMemCheck$(tput sgr0)"
        read io 

        (cd build && make ExperimentalMemCheck)

        echo "$(tput bold)$(tput setaf 3)Press ENTER to make a memcheck of 'recolor_text' (arg by default)$(tput sgr0)"
        read io

        (cd build && valgrind --leak-check=full -v ./recolor_text 313103101320)

        echo "$(tput bold)$(tput setaf 3)Press ENTER to make a memcheck of 'recolor_text horizontal2S_game.rec'$(tput sgr0)"
        read io

        (cd build && valgrind --leak-check=full -v ./recolor_text data/horizontal_game2S.rec 192983651012735182946720)

    else
        ./helper.sh
    fi
fi