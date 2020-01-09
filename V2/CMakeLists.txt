cmake_minimum_required(VERSION 2.6)

project (recolor C)

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

add_executable(recolor_text recolor_text.c)
add_executable(test_game_fakhoun test_game_fakhoun.c)
add_executable(test_game_mpitassi test_game_mpitassi.c)
add_executable(test_game_akasparian test_game_akasparian.c)
add_executable(test_game_mrousseau006 test_game_mrousseau006.c)

add_library(game game_io.c game.c)

target_link_libraries(recolor_text game)
target_link_libraries(test_game_fakhoun game)
target_link_libraries(test_game_mpitassi game)
target_link_libraries(test_game_akasparian game)
target_link_libraries(test_game_mrousseau006 game)

include(CTest)
enable_testing()
set(cmds "test_game_fakhoun" "test_game_fakhoun" "test_game_fakhoun" "test_game_fakhoun" "test_game_mpitassi" "test_game_mpitassi" "test_game_mpitassi" "test_game_mpitassi" "test_game_mpitassi" "test_game_akasparian" "test_game_akasparian" "test_game_akasparian" "test_game_akasparian" "test_game_mrousseau006" "test_game_mrousseau006" "test_game_mrousseau006" "test_game_mrousseau006")
set(args "test_game_nb_moves_max" "test_game_set_cell_init" "test_game_cell_current_color" "test_game_is_wrapping" "game_new" "set_max_moves" "copy" "height" "width" "game_play_one_move" "game_is_over" "game_restart" "game_new_ext" "game_new_empty" "game_delete" "game_nb_moves_cur" "game_new_empty_ext")
foreach(i RANGE 16)
    list(GET cmds ${i} cmd)
    list(GET args ${i} arg)
    add_test(recolor-${arg} ./${cmd} ${arg})
    set_tests_properties(recolor-${arg} PROPERTIES PASS_REGULAR_EXPRESSION "SUCCESS" TIMEOUT 3)
endforeach()
