#ifndef __FUNC_H__
#define __FUNC_H__

struct func
{
	int & i;
	func(int &i_) : i(i_) {}

	void operator() ()
	{
		std::cout << "operator start\n";

		for (unsigned int j = 0; j < 100; j++)
			std::cout << i << std::endl;

		std::cout << "operator end\n";
	}
};

#endif //__FUNC_H__