#include "Image.h"


void Image::grayscale()
{
	if (is_black_and_white() || is_grayscale())
	{
		make_change = false;
		return;
	}
	const float r = 0.299F;
	const float g = 0.587F;
	const float b = 0.114F;

	float grayscale_sum = 0;
	for (size_t i = 0; i < data_size - 3; i += 3)
	{
		//data from type char
		if (data != NULL){
			grayscale_sum = (data[i] * r + data[i + 1] * g + data[i + 2] * b);
			data[i] = char(grayscale_sum);
			data[i + 1] = char(grayscale_sum);
			data[i + 2] = char(grayscale_sum);
		}
		else if (data_int!=NULL)
		{
			grayscale_sum = (data_int[i] * r + data_int[i + 1] * g + data_int[i + 2] * b);
			data_int[i] = grayscale_sum;
			data_int[i + 1] = grayscale_sum;
			data_int[i + 2] = grayscale_sum;
		}
	}
	make_change = true;
}


bool Image::is_black_and_white()const
{
	if (data != NULL)
	{
		for (size_t i = 0; i < data_size; i++)
		{
			if (((int)data[i] < 250 && (int)data[i] > 5) &&
				((int)data[i + 1] <= 250 && (int)data[i + 1] > 5) &&
				((int)data[i + 2] <= 250 && (int)data[i + 2] > 5))
				return false;
		}
	}
	else if (data_int != NULL)
	{
		for (size_t i = 0; i < data_size; i++)
		{
			if ((data_int[i] < 250 && data_int[i] > 5) &&
				(data_int[i + 1] <= 250 && data_int[i + 1] > 5) &&
				(data_int[i + 2] <= 250 && data_int[i + 2] > 5))
				return false;
		}
	}
	return true;
}


bool Image::is_grayscale()const
{
	if (data != NULL){
		for (size_t i = 0; i < data_size-3; i+=3)
		{
			if ((data[i]) != (data[i + 1]) || (data[i + 1]) != (data[i + 2]))
				return false;
		}
	}
	else if (data_int != NULL)
	{
		for (size_t i = 0; i < data_size-3; i+=3)
		{
			if ((data_int[i]) != (data_int[i + 1]) || (data_int[i + 1]) != (data_int[i + 2]))
				return false;
		}
	}
	return true;
}



char * Image::new_file_name(char *& path_to_file, char*& command)const
{
	char *temp = NULL;

	int size_path_to_file = strlen(path_to_file) + 1;
	int size_path_command = strlen(command);
	int size_temp = size_path_command + size_path_to_file;
	size_t  i_backslash = 0;

	try{
		temp = new char[size_temp];
	}
	catch (bad_alloc)
	{
		delete[]temp;
		return "new_image";
	}

	for (i_backslash = size_path_to_file - 1; i_backslash > 0; i_backslash--)
	{
		if (path_to_file[i_backslash] == '\\')
			break;
	}
	int index_path = 0;
	int index_command = 0;
	for (size_t i = 0; i < size_temp; i++)
	{
		temp[i] = path_to_file[index_path];
		if (i > i_backslash && index_command < size_path_command)
		{
			temp[i] = command[index_command];
			index_command++;
		}
		else
		{
			index_path++;

		}
	}
	return temp;
}

void Image::execute_command(char*&came, char*& file_name) 
{

	if (strcmp(came, "--grayscale") == 0)
	{
		if (format[1] == '3' || format[1] == '6') //ppm 
			grayscale();
	}
	if (strcmp(came, "--monochrome") == 0)
		monochrome();

	if ((strcmp(came, "--histogram=red") == 0) ||
		(strcmp(came, "--histogram=green") == 0) ||
		(strcmp(came, "--histogram=blue") == 0))
		histogram(came, file_name);
}

void Image::monochrome()
{
	int old_pixel = 0;
	int new_pixel = 0;
	int error = 0;
	int shift_index = 0;
	
	if (format[1] == '2')//ASCII -pgm
	{
		for (size_t i = 0; i < data_size - width - 1; i++)
		{
			old_pixel = data_int[i];
			new_pixel = (data_int[i]>128) ? 255 : 0;
			data_int[i] = new_pixel;
			error = old_pixel - new_pixel;

			//1).. x+1, y
			data_int[i + 1] += (7.0 / 16 * error);
			//3).. x, y+1
			shift_index = i + width;
			data_int[shift_index] += (error * 5.0 / 16);
			//4 .. x+1, y+1 =>shift +1
			data_int[shift_index + 1] += (error * 1.0 / 16);
			//2 .. x-1, y+1 = > shift -1
			data_int[shift_index - 1] += (error* 3.0 / 16);
		}
	}

	if (format[1] == '3')//ASCII -ppm
	{
	
		int width_ppm = width * 3;
		for (size_t i = 0; i < data_size - width_ppm - 5; i += 3)
		{
			old_pixel = data_int[i];
			new_pixel = (data_int[i]>128) ? 255 : 0;
			data_int[i] = new_pixel;
			data_int[i + 1] = new_pixel;
			data_int[i + 2] = new_pixel;

			error = old_pixel - new_pixel;

			//1).. x+1, y
			data_int[i + 3] += (7.0 / 16 * error);
			data_int[i + 3 + 1] += (7.0 / 16 * error);
			data_int[i + 3 + 2] += (7.0 / 16 * error);
			//3).. x, y+1
			shift_index = i + width_ppm;
			data_int[shift_index] += (error * 5.0 / 16);
			data_int[shift_index + 1] += (error * 5.0 / 16);
			data_int[shift_index + 2] += (error * 5.0 / 16);
			//4 .. x+1, y+1 =>shift +1
			data_int[shift_index + 1] += (error * 1.0 / 16);
			data_int[shift_index + 1 + 1] += (error * 1.0 / 16);
			data_int[shift_index + 1 + 2] += (error * 1.0 / 16);
			//2 .. x-1, y+1 = > shift -1
			data_int[shift_index - 1] += (error* 3.0 / 16);
			data_int[shift_index - 1 - 1] += (error* 3.0 / 16);
			data_int[shift_index - 1 - 2] += (error* 3.0 / 16);
		}
	}

	if ((format[1] == '5'))//Binary -pgm
	{

		for (size_t i = 0; i < data_size - width - 1; i++)
		{
			data[i] = (char)((int)data[i] * (-1));
			new_pixel = (data[i]>96) ? 255 : 0;
			data[i] = (char)new_pixel;
		}
	}

	if ((format[1] == '6'))//Binary -pgm
	{
		for (size_t i = 0; i < data_size - 3; i+=3)
		{
			old_pixel = data[i];
			new_pixel = (data[i]>96) ? 255 : 0;
			data[i] = (char)new_pixel;
			data[i + 1] = (char)new_pixel;
			data[i + 2] = (char)new_pixel;
		}
	}
	make_change = true;
}

void Image::free()
{
	delete[]data;
	delete[]data_int;
	data = NULL;
	data_int = NULL;
	make_change = false;
	data_size = 0;
	width = 0;
	height = 0;
	max_pix = 0;
}


void Image::histogram(char*& came, char*& file_name)
{
	int jumb = 0;
	if (strcmp(came, "--histogram=red") == 0) 
		jumb = 0;
	if (strcmp(came, "--histogram=green") == 0)
		jumb = 1;
	if (strcmp(came, "--histogram=blue") == 0)
		jumb = 2;

	int min = 0, middle = 0, max = 0; // 1 ili 0
	const int row = 100, col = 255*3;
	int hist[row][col];
	for (size_t i = 0; i < row; i++)
	{
		for (size_t k = 0; k < col; k++)
		{
			hist[i][k] = 255;
		}
	}
	for (size_t i = 0; i < data_size - 3; i+=3) 
	{
		if (format[1] == '3')
		{
			//ascii --ppm
			if (data_int[i] <= 100)
				min++;
			if (data_int[i] > 100 && data_int[i] <200)
				middle++;
			if (data_int[i] >= 200)
				max++;
		}
		if (format[1] == '6')
		{
			//binary --ppm
			if (data[i] <= 100)
				min++;
			if (data[i] > 100 && data[i] <200)
				middle++;
			if (data[i] >= 200)
				max++;
		}

	}
	int min_percent = (min * 100) / (data_size / 3);
	int middle_percent = (middle*100) / (data_size / 3);
	int max_percent = (max*100) / (data_size / 3);

	//to check
	//min_percent = 25;
	//middle_percent = 50;
	//max_percent = 25;

	int min_column = row - min_percent;
	int middle_column = row - middle_percent;
	int max_column = row - max_percent;

	int index = 0;
	for (size_t i = min_column; i <row ; i++) 
	{
		index = 0;
		//red
		if (jumb == 0)
		{
			hist[i][index] = 255;
			hist[i][index + 1] = 0;
			hist[i][index + 2] = 0;
		}
		//green
		if (jumb == 1)
		{
			hist[i][index + 1] = 255;
			hist[i][index + 1 - 1] = 0;
			hist[i][index + 1 + 1] = 0;
		}
		//blue
		if (jumb == 2)
		{
			hist[i][index + 2] = 255;
			hist[i][index + 2 - 1] = 0;
			hist[i][index + 2 - 2] = 0;
		}
	}


	for (size_t i = middle_column; i < row; i++)
	{
		index = 120*3;
		//red
		if (jumb == 0)
		{
			hist[i][index] = 255;
			hist[i][index + 1] = 0;
			hist[i][index + 2] = 0;
		}
		//green
		if (jumb == 1)
		{
			hist[i][index + 1] = 255;
			hist[i][index + 1 - 1] = 0;
			hist[i][index + 1 + 1] = 0;
		}
		//blue
		if (jumb == 2)
		{
			hist[i][index + 2] = 255;
			hist[i][index + 2 - 1] = 0;
			hist[i][index + 2 - 2] = 0;
		}
	}

	for (size_t i = max_column; i < row; i++)
	{
		index = 252*3;
		//red
		if (jumb == 0)
		{
			hist[i][index] = 255;
			hist[i][index + 1] = 0;
			hist[i][index + 2] = 0;
		}
		//green
		if (jumb == 1)
		{
			hist[i][index + 1] = 255;
			hist[i][index + 1 - 1] = 0;
			hist[i][index + 1 + 1] = 0;
		}
		//blue
		if (jumb == 2)
		{
			hist[i][index + 2] = 255;
			hist[i][index + 2 - 1] = 0;
			hist[i][index + 2 - 2] = 0;
		}
	}

	char* temp = new_file_name(file_name, came);// came is command

	//ascii --ppm -- data_int
	if (format[1] == '3' || format[1] == '6')
	{
		ofstream ofile(temp);
		if (ofile.good())
		{
			//format is ppm ASCII
			ofile.write("P3", size_format);
			ofile << " " << (col/3) << " " << row << " " << max_pix << '\n';

			for (size_t i = 0; i < row; i++)
				for (size_t j = 0; j < col; j++)
					ofile << hist[i][j] << " ";
		}
		ofile.close();
	}
	delete[]temp;

}