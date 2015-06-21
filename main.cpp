#include"Manage.h"

int main(int argc, char* argv[])
{
	Manage input;
	try{
		input.create(argc, argv);
	} catch (char*)
	{
		cout << "Wrong input!"<<endl;
		return -1;
	}
	input.execute_file();


}