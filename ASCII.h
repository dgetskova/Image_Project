#include "Image.h"

class ASCII :public Image
{
	virtual void load_image_format(char*& file_name);
	virtual void save_image_format(char*&file_name, char*&command)const;
};