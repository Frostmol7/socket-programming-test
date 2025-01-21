# Указываем, что это исполняемый файл
add_executable(program2 main.cpp)

# Линкуем программу с библиотекой pthread (если необходимо)
target_link_libraries(program2 pthread)

