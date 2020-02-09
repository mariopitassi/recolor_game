cmake_minimum_required(VERSION 2.6)

project (recolor C)

set(CMAKE_C_FLAGS "-std=c99 -g -Wall --coverage")

add_executable(recolor_text recolor_text.c)
add_executable(recolor_solve recolor_solve.c)
add_executable(test_game_fakhoun test_game_fakhoun.c)
add_executable(test_game_mpitassi test_game_mpitassi.c)
add_executable(test_game_akasparian test_game_akasparian.c)
add_executable(test_game_mrousseau006 test_game_mrousseau006.c)

add_library(game game_io.c game.c)

target_link_libraries(recolor_text game)
target_link_libraries(recolor_solve game)
target_link_libraries(test_game_fakhoun game)
target_link_libraries(test_game_mpitassi game)
target_link_libraries(test_game_akasparian game)
target_link_libraries(test_game_mrousseau006 game)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include(CTest)
enable_testing()
set(cmds "test_game_fakhoun" "test_game_fakhoun" "test_game_fakhoun" "test_game_fakhoun" "test_game_mpitassi" "test_game_mpitassi" "test_game_mpitassi" "test_game_mpitassi" "test_game_mpitassi" "test_game_akasparian" "test_game_akasparian" "test_game_akasparian" "test_game_akasparian" "test_game_akasparian" "test_game_akasparian" "test_game_mrousseau006" "test_game_mrousseau006" "test_game_mrousseau006" "test_game_mrousseau006")
set(args "game_nb_moves_max" "game_set_cell_init" "game_cell_current_color" "game_is_wrapping" "game_new" "set_max_moves" "copy" "height" "width" "game_play_one_move" "game_is_over" "game_restart" "game_new_ext" "game_load" "game_save" "game_new_empty" "game_delete" "game_nb_moves_cur" "game_new_empty_ext")
foreach(i RANGE 16)
    list(GET cmds ${i} cmd)
    list(GET args ${i} arg)
    add_test(test_${arg} ./${cmd} ${arg})
    set_tests_properties(test_${arg} PROPERTIES PASS_REGULAR_EXPRESSION "SUCCESS" TIMEOUT 3)
endforeach()