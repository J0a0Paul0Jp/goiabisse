cmake_minimum_required(VERSION 3.16.3)
project(acceptorFix)
add_executable(
	${PROJECT_NAME}
	client.cpp
	Application.cpp  
	Application.h  
)

target_link_directories("${PROJECT_NAME}" PRIVATE "lib")
target_link_libraries("${PROJECT_NAME}" PRIVATE "quickfix")
target_include_directories("${PROJECT_NAME}" PRIVATE "src")
