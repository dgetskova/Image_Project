#pragma once
#include <iostream>
#include <fstream>
#include "Binary.h"
#include "ASCII.h"
#include <string>

class Manage
{
	char ** input;
	size_t input_size;

	void Manage::allocate(int size, char **&argv);
	void Manage::free();

	bool Manage::correct_input();
	bool is_command(char*& came)const; 
	bool is_file(int index);
	void insert_backslash(int index);

	void commands_for_file(int index);
	void read(char*&file, char*& command);
	
public:
	Manage();
	void create(int argc, char**&argv);
	~Manage();
	
	void execute_file();
	
};