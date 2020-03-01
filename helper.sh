#bin/sh

if [ $# = 0 ]; then 
    echo "$(tput setaf 3)Usage : helper.sh <command>"
    echo "$(tput setaf 7)where command is one of build, test, clear, coverage, memcheck and switch\n$(tput sgr0) "
    echo "$(tput bold)build$(tput sgr0) <release|debug> - Create a build dir and copy executables in run dir"
    echo "$(tput bold)test$(tput sgr0)  - Run all the test of the program"
    echo "$(tput bold)clear$(tput sgr0) - Clean the projet (del: /build)"
    echo "$(tput bold)coverage$(tput sgr0) - Run coverage test"
    echo "$(tput bold)switch$(tput sgr0) <V1|V2> - Change version of the project"
    echo "$(tput bold)memcheck$(tput sgr0) - Do a complete memory check"
    echo "$(tput bold)format$(tput sgr0) - Format all *.h and *.c file with clang-format"
else
    if [ "$1" = "build" ]
    then
        if [ ! -d "./build" ]; then

            if [ "$2" = "release" ]; 
            then
                mkdir build
                (cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make)
            elif [ "$2" = "debug" ]; 
            then 
                mkdir build
                (cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make)
            else
                echo "Usage: ./helper.sh build release|debug"
            fi

        else
            echo "$(tput setaf 3)\nAttention ! Ceci est un simple make..."
            echo "Si vous souhaiter changer de mode de compilation (Debug ou Release), faites un clean d'abord...\n$(tput sgr0)"
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

    elif [ "$1" = "format" ]
    then
        if [ ! -d "./include" ] || [ ! -d "./src" ] || [ ! -d "./tests" ]; then
            echo "$(tput setaf 3)Ce n'est pas la dernière version du projet !"
            clang-format -i *.c *.h
            sleep 0.5
        else 
            find . -name '*.h' -o -name '*.c' | xargs clang-format -i
        fi

        echo "$(tput setaf 2)Les fichiers *.c et *.h ont été formatés !"

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

        echo "$(tput bold)$(tput setaf 3)Press ENTER to run an ExperimentalMemCheck$(tput sgr0)"
        read io 

        (cd build && make ExperimentalMemCheck)

        echo "$(tput bold)$(tput setaf 3)Press ENTER to make a memcheck of 'recolor_text' (arg by default)$(tput sgr0)"
        read io

        (cd build && valgrind --leak-check=full echo $(cat "data/default_game.sol") | ./recolor_text)

        echo "$(tput bold)$(tput setaf 3)Press ENTER to make a memcheck of 'recolor_text horizontal2S_game.rec'$(tput sgr0)"
        read io

        (cd build && valgrind --leak-check=full echo $(cat "data/horitzontal_game2S.sol") | ./recolor_text data/horizontal_game2S.rec)

    else
        ./helper.sh
    fi
fi