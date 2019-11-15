# CMake generated Testfile for 
# Source directory: /net/cremi/fakhoun/Bureau/espaces/travail/projet/projetGroupe/recolor-tdm1-c
# Build directory: /net/cremi/fakhoun/Bureau/espaces/travail/projet/projetGroupe/recolor-tdm1-c/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(recolor-nb_moves_max "./test_game_fakhoun" "nb_moves_max")
set_tests_properties(recolor-nb_moves_max PROPERTIES  TIMEOUT "3")
add_test(recolor-set_cell_init "./test_game_fakhoun" "set_cell_init")
set_tests_properties(recolor-set_cell_init PROPERTIES  TIMEOUT "3")
add_test(recolor-cell_current_color "./test_game_fakhoun" "cell_current_color")
set_tests_properties(recolor-cell_current_color PROPERTIES  TIMEOUT "3")
add_test(recolor-game_new "./test_game_mpitassi" "game_new")
set_tests_properties(recolor-game_new PROPERTIES  TIMEOUT "3")
add_test(recolor-set_max_moves "./test_game_mpitassi" "set_max_moves")
set_tests_properties(recolor-set_max_moves PROPERTIES  TIMEOUT "3")
add_test(recolor-copy "./test_game_mpitassi" "copy")
set_tests_properties(recolor-copy PROPERTIES  TIMEOUT "3")
add_test(recolor-game_play_one_move "./test_game_akasparian" "game_play_one_move")
set_tests_properties(recolor-game_play_one_move PROPERTIES  TIMEOUT "3")
add_test(recolor-game_is_over "./test_game_akasparian" "game_is_over")
set_tests_properties(recolor-game_is_over PROPERTIES  TIMEOUT "3")
add_test(recolor-game_restart "./test_game_akasparian" "game_restart")
set_tests_properties(recolor-game_restart PROPERTIES  TIMEOUT "3")
add_test(recolor-game_new_empty "./test_game_mrousseau006" "game_new_empty")
set_tests_properties(recolor-game_new_empty PROPERTIES  TIMEOUT "3")
add_test(recolor-game_delete "./test_game_mrousseau006" "game_delete")
set_tests_properties(recolor-game_delete PROPERTIES  TIMEOUT "3")
add_test(recolor-game_nb_moves_cur "./test_game_mrousseau006" "game_nb_moves_cur")
set_tests_properties(recolor-game_nb_moves_cur PROPERTIES  TIMEOUT "3")
