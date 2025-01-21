# Указываем, что это исполняемый файл
add_executable(program1 main.cpp socket_manager.cpp)

# Задаем флаги компилятора для использования pthreads (для потоков)
target_link_libraries(program1 pthread)

