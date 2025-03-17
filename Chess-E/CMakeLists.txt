cmake_minimum_required(VERSION 3.10)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files (exclude main.cpp)
set(ENGINE_SOURCES
    src/board.cpp
    src/movegen.cpp
    src/evaluation.cpp
    src/search.cpp
    src/endgame_tablebase.cpp
    src/opening_book.cpp
    src/iterative_deepening.cpp
    src/alphabeta.cpp
    src/transposition_table.cpp
    src/uci.cpp
    src/post_analysis.cpp
    src/search_extensions.cpp
    src/time_management.cpp
    src/aspiration_windows.cpp
    src/late_move_reduction.cpp
    src/null_move_pruning.cpp
    src/principal_variation_search.cpp
    src/move_ordering.cpp
    src/quiescence_search.cpp
    src/thread_pool.cpp
)

# Add test program
set(TEST_SOURCES
    src/test_engine.cpp
)

# Additional specialized test executables
set(TEST_EVAL_SOURCES
    test/test_evaluation.cpp
)

set(TEST_SEARCH_SOURCES
    test/test_search.cpp
)

# Create library for common code
add_library(chess_engine_lib SHARED ${ENGINE_SOURCES})
target_include_directories(chess_engine_lib PUBLIC src)

# Create main executable
add_executable(chess_engine src/main.cpp)
target_link_libraries(chess_engine PRIVATE chess_engine_lib)

# Create test executable
add_executable(test_engine ${TEST_SOURCES})
target_link_libraries(test_engine PRIVATE chess_engine_lib)

# Create specialized test executables
add_executable(test_evaluation ${TEST_EVAL_SOURCES})
target_link_libraries(test_evaluation PRIVATE chess_engine_lib)

add_executable(test_search ${TEST_SEARCH_SOURCES})
target_link_libraries(test_search PRIVATE chess_engine_lib)

# Add include directories
target_include_directories(chess_engine PRIVATE src)
target_include_directories(test_engine PRIVATE src)
target_include_directories(test_evaluation PRIVATE src)
target_include_directories(test_search PRIVATE src)

# Set ENABLE_EXPORTS property to allow linking
set_target_properties(chess_engine PROPERTIES ENABLE_EXPORTS ON)