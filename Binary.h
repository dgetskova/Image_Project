#include "image.h"

class Binary :public Image
{
public:
	virtual void load_image_format(char*& file_name);
	virtual void save_image_format(char*&file_name,char*& command)const;

};