cmake_minimum_required(VERSION 3.15)

# 1. Define the project name and metadata
project(message-broker 
    VERSION 1.0.0 
    LANGUAGES CXX
)

# 2. Set the C++ standard globally
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 3. Create an executable target and specify source files
add_executable(message-broker 
    src/server_main.cpp
    src/message_queue.cpp
    src/queue_manager.cpp
    src/message.cpp
    src/message_broker.cpp
    src/server.cpp
)

# Client executable
add_executable(client
    src/client_main.cpp
    src/client.cpp
)

# 4. Include directories (where your .h header files live)
target_include_directories(message-broker PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/nlohmann
)

target_include_directories(client PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/nlohmann
)

# 5. Link external libraries if needed (e.g., threads)
find_package(Threads REQUIRED)
target_link_libraries(message-broker PRIVATE 
    Threads::Threads
)
