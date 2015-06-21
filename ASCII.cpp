#include "ASCII.h"

void ASCII::load_image_format(char*& file_name)
{
	make_change = false;

	ifstream ifile;
	ifile.open(file_name);
	string comment;

	if (!ifile.good())
	{
		throw"problem with file.";
	}
	ifile.read(format, size_format);

	if (ifile.peek() == '\n')
		ifile.get();

	while (true){
		if (ifile.peek() == '#')
			getline(ifile, comment);
		else
			break;
	}
	//i know that in this binary file has this data
	ifile >> width;
	ifile >> height;
	ifile >> max_pix;

	while (ifile.peek() == '\n')
		ifile.get();
	// pgm or ppm

	if (format[1] == '2')
	{
		data_size = width*height;
	}
	else if (format[1] == '3')
	{
		data_size = width*height * 3;
	}
	else throw"stupid ascii format.";
	try{
		data_int = new int[data_size];
	}
	catch (std::bad_alloc)
	{
		free();
		throw "No memory";
	}
	if (ifile.good())
		for (size_t i = 0; i < data_size; i++)
		{
			ifile >> data_int[i];
		}
	else
	{
		free();
		ifile.close();
		throw"Problem with file";
	}
	ifile.close();
}


void ASCII::save_image_format(char*& path_to_file, char*& command)const
{
	if (make_change)
	{
		char* temp = new_file_name(path_to_file, command);
		ofstream ofile(temp);
		if (ofile.good())
		{

			ofile.write(format, size_format);
			ofile << " " << width << " " << height << " " << max_pix << '\n';

			for (size_t i = 0; i < data_size; i++)
			{
				ofile << data_int[i]<<" ";
			}
		}
		ofile.close();
		delete[]temp;
	}
	return;
}