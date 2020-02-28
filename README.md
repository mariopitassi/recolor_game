# Recolor game projet

*This project is maintained by Anouche KASPARIAN, Mattias ROUSSEAU, Farouk FAKHOUN and Mario PITASSI.* 

## Installation & usage 

Prerequisites : 
- Git
- GCC, CLang or another C compiler
- Valgrind (memory leaks)
- GCC (debug)
- Make, CMake & CTest (processes, build & test)
- Clang-format (linter of code)

To download the project you need to run the command : `git clone https://services.emi.u-bordeaux.fr/projet/git/recolor-tdm1-c`

You can use our helper to manipulate the project easily. Open your terminal and run `./helper.sh <command>`.

There are 6 use cases : 
- `build release|debug` : build project out-of-source using CMake in /build directory in release or debug mode
- `test` : run all project tests using ctest
- `coverage` : check coverage of code using CMake
- `switch` : switch between V1 and V2 of recolor using git branch
- `memcheck` : check memory leak on executables

## Architecture

```
.
├── .clang-format (config file linter)
├── .gitignore
├── CMakeLists.txt
├── README.md
├── data (all file load)                                                                 
│   ├── default_game.rec
│   ├── default_game.sol
│   ├── horitzontal_game2N.sol
│   ├── horitzontal_game2S.sol
│   ├── horizontal_game2N.rec
│   └── horizontal_game2S.rec
├── game.c
├── game.h
├── game_io.c
├── game_io.h
├── helper.sh
├── recolor_solve.c
├── recolor_text.c
├── test_game_akasparian.c
├── test_game_fakhoun.c
├── test_game_mpitassi.c
├── test_game_mrousseau006.c
└── todo.txt
```