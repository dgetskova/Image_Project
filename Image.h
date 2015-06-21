#pragma once

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

typedef unsigned int uint;

class Image
{
protected:
	static const size_t size_format = 2; 

	bool make_change;
	char* data;
	int* data_int;
	size_t data_size;

	char format[size_format];
	uint width;
	uint height;
	uint max_pix;

	void free();

protected:
	bool is_black_and_white()const;
	bool is_grayscale()const;

	char* new_file_name(char *& path_to_file, char*& command)const;//to delete return data

	void monochrome();
	void grayscale();
	void histogram(char*& came, char *&file_name);

public:
	Image()
	{
		data = NULL;
		data_int = NULL;
		make_change = false;
	}
	virtual~Image()
	{
		free();
	};

	virtual void load_image_format(char*& filename)=0;
	virtual void save_image_format(char*&file_name,char*& command)const=0;

	void execute_command(char*&came, char*& file_name);
	int get_data_size()
	{
		return width*height * 3;
	}
};