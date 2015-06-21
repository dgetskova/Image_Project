#include "Manage.h"
using namespace std;

const int SIZE_FORMAT = 2;

void Manage::allocate(int size_came, char **&argv)
{
	input_size = size_came;
	int size_elem = 0;
	try{
		input = new char*[input_size];
	} catch (std::bad_alloc)
	{
		free();
		throw"No obj input.";
	}
	for (size_t i = 0; i < input_size; i++)
	{
		input[i] = NULL;
	}

	int zero_command = 1; // = 0 kogato iskam da si go pusna ot cmd
	for (size_t i = 0; i < input_size; i++)
	{
		size_elem = strlen(argv[zero_command]) + 1;
		try{
		input[i] = new char[size_elem];
		}
		catch (std::bad_alloc)
		{
			free();
			throw"No obj input.";
		}
		strncpy_s(input[i], size_elem, argv[zero_command], size_elem);
		zero_command++;
	}

}
Manage::~Manage()
{
	free();
}

void Manage::free()
{
	for (size_t i = 0; i < input_size; i++)
	{
		delete input[i];
	}
	delete[]input;
	input = NULL;
	input_size = 0;
}

void Manage::create(int size, char **&argv)
{
	allocate(size-1, argv);//change for load from cmd (size)
	if (!correct_input())
	{
		free();
		throw"Wrong input!!!";
	}
}

bool Manage::correct_input()
{
	for (size_t i = 0; i < input_size; i++)
	{
		if (!(is_command(input[i]) || is_file(i)))
			return false;
	}
}

bool Manage::is_file(int index)
{
	//kogato si e samo faila ili e poddaden s '\\' pravilniq pyt
	fstream file_try(input[index]);
	if (!file_try)
		;
	else
	{
		if (file_try.good())
			;
		file_try.close();
		return true;
	}

	insert_backslash(index);
	fstream file_try2(input[index]);
	if (!file_try2)
		return false;
	if (file_try.good())
	{
		cout << "yesss";
	}
	file_try2.close();
	return true;
}

void Manage::insert_backslash(int index)
{
	size_t elem_size = strlen(input[index]) + 1;

	size_t backslash = 0;
	char symbol = '\\';
	char *temp = NULL;
	try{
		temp = new char[elem_size];
	}
	catch (std::bad_alloc)
	{
		free();
		throw "No obj.";
	}strncpy_s (temp, elem_size, input[index], elem_size);

	for (size_t i = 0; i < elem_size; i++)
	{
		if (input[index][i] == symbol)
			backslash++;
	}
	if (backslash == 0)
		return;
	int new_elem_size = elem_size + backslash;
	delete input[index];
	input[index] = NULL;

	try{
		input[index] = new char[new_elem_size];
	}
	catch (std::bad_alloc)
	{
		free();
		throw "NO obj.";
	}
	int index_backslash = 0;
	for (size_t i = 0; i < elem_size; i++)
	{
		input[index][index_backslash] = temp[i];
		index_backslash++;
		if (temp[i] == '\\')
		{
			input[index][index_backslash] = '\\';
			index_backslash++;
			
		}
	}
	delete []temp;
}

bool Manage::is_command(char *&came)const
{
	if ((strcmp(came, "--grayscale") == 0) || (strcmp(came, "--monochrome") == 0))
		return true;
	if ((strcmp(came, "--histogram=red") == 0) ||
		(strcmp(came, "--histogram=green") == 0) ||
		(strcmp(came, "--histogram=blue") == 0))
		{
			return true;
		}
	return false;
}

void Manage::execute_file()
{
	for (size_t i = 0; i < input_size; i++)
	{
		if (is_file(i))
		{
			commands_for_file(i);
		}
	}
}

void Manage::commands_for_file(int index)
{
	for (size_t i = 0; i < input_size; i++)
	{
		if (is_command(input[i]))
		{
			read(input[index], input[i]);
		}
	}
}

void Manage::read( char*&file_name, char*& command)
{
	ifstream file(file_name);
	if (!file)
	{
		;
	}
	if (file.good())
	{
		char format[SIZE_FORMAT];
		file.read(format, SIZE_FORMAT);
		file.close();
		
		// format pbm 1 i 4
		if (format[SIZE_FORMAT - 1] == '1' || format[SIZE_FORMAT - 1] == '4')
		{
			return;
		}
		Image* base_pointer = NULL;
		//ASCII format 2 i 3
		if (format[SIZE_FORMAT - 1] == '2' || format[SIZE_FORMAT-1] == '3')
			base_pointer = new ASCII;
		//binary format 5 I 6
		else if (format[SIZE_FORMAT - 1] == '5' || format[SIZE_FORMAT-1] == '6')
			base_pointer = new Binary;
		else{
			cout << "wrong format image";
			return;
		}

		base_pointer->load_image_format(file_name);
		base_pointer->execute_command(command, file_name);
		base_pointer->save_image_format(file_name,command);
	}

}


Manage::Manage()
{
	input = NULL;
	input_size = NULL;
}