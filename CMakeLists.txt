CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

project(Lockfree_Reader_Writer)
set(CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=thread")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=thread")
find_package(Threads REQUIRED)
add_executable(Reader_Writer_pointer_globalObj Reader_Writer_pointer_globalObj.cpp)
add_executable(Reader_Writer_pointers_only Reader_Writer_pointers_only.cpp)
target_link_libraries(Reader_Writer_pointer_globalObj Threads::Threads)
target_link_libraries(Reader_Writer_pointers_only Threads::Threads)

