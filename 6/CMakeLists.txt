add_executable(6_1 6_1.cpp)
add_executable(6_2 6_2.cpp)
target_link_libraries(6_2 GTest::GTest GTest::Main)
add_test(NAME 6_2 COMMAND MyTests)

add_executable(6_3 6_3.cpp)