cmake_minimum_required(VERSION 3.16.3)
project(AcceptorFix)
add_executable(
	app 
	StartServidor.cpp 
	Application.cpp Market.cpp
	Application.h  
	IDGenerator.h  
	Market.h  
	OrderCommand.h  
	Order.h
)