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

There are 7 use cases : 
- `build release|debug` : build project out-of-source using CMake in /build directory in release or debug mode
- `test` : run all project tests using ctest
- `clear` : delete ./build folder
- `coverage` : check coverage of code using CMake
- `switch` : switch between V1 and V2 of recolor using git branch
- `memcheck` : check memory leak on executables
- `format` : format all *.h et *.c with clang-format

## Architecture

```bash
.
├── .clang-format
├── .gitignore
├── CMakeLists.txt
├── README.md
├── data
│   ├── default_game.rec
│   ├── default_game.sol
│   ├── horitzontal_game2N.sol
│   ├── horitzontal_game2S.sol
│   ├── horizontal_game2N.rec
│   └── horizontal_game2S.rec
├── helper.sh
├── include
│   ├── .DS_Store
│   ├── asde_slist.h
│   ├── asde_slist_utilitary_functions.h
│   ├── game.h
│   ├── game_io.h
│   └── solver.h
├── src
│   ├── asde_slist.c
│   ├── asde_slist_utilitary_functions.c
│   ├── game.c
│   ├── game_io.c
│   ├── recolor_solve.c
│   ├── recolor_text.c
│   └── solver.c
└── tests
    ├── test_game_akasparian.c
    ├── test_game_fakhoun.c
    ├── test_game_mpitassi.c
    └── test_game_mrousseau006.c
```

## Répartition du travail

### 1) Fonctions principales

Mario
- set_max_moves
- set_cell_init
- game_play_one_move
- game_height
- game_width

Mattias
- restart
- game_delete
- game_moves_max
- game_new_empty_ext

Anouche
- copy
- game_is_over
- nb_moves_max
- game_new_ext

Farouk
- game_new
- game_new_empty
- cell_current_color
- game_is_wrapping

### 2) Fonctions des tests

Mario
- test_game_new
- test_set_max_moves
- test_copy
- test_game_height
- test_game_width

Mattias
- test_game_new_empty
- test_game_nb_moves_cur
- test_game_delete
- test_game_new_empty_ext

Anouche
- test_game_play_one_move
- test_game_is_over
- test_game_restart
- test_game_new_ext

Farouk
- test_nb_moves_max
- test_set_cell_init
- test_cell_current_color
- test_is_wrapping