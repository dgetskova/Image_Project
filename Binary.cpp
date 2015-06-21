#include"Binary.h"

void Binary::load_image_format(char*& file_name)
{
	make_change = false;
	ifstream ifile;
	ifile.open(file_name, ios::binary);
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
	//i know that it has this data in file
	ifile >> width;
	ifile >> height;
	ifile >> max_pix;

	while (ifile.peek() == '\n')
		ifile.get();
	// pgm or ppm
	if (format[1] == '5')
	{
		data_size = width*height;
	}
	else if (format[1] == '6')
	{
		data_size = width*height * 3;
	}
	try{
		data = new char[data_size];
	}
	catch (std::bad_alloc)
	{
		free();
		throw "No memory";
	}
	if (ifile.good())
		ifile.read(data, data_size);
	else
	{
		free();
		ifile.close();
		throw"Problem with file";
	}
	ifile.close();
}


void Binary::save_image_format(char*& path_to_file, char*& command)const
{

	if (make_change)
	{
		char * temp = new_file_name(path_to_file, command);
		ofstream ofile(temp, ofstream::binary);

		if (ofile.good())
		{
			ofile.write(format, size_format);
			ofile << " " << width << " " << height << " " << max_pix << '\n';

			ofile.write(data, data_size);
		}
		ofile.close();
		delete[]temp;
	}
		return;
}
