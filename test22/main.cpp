#include <iostream>
#include <vector>
#include <algorithm>




int main()
{
	unsigned char byte;
	std::vector<unsigned char> vec;
	std::cin >> byte;
	
	for (unsigned char b = 0; b < 255; b++)
		//std::cout << int(byte) << "^" << int(b) << "=\t" << int(byte ^ b) << std::endl;
		vec.push_back(byte ^ b);

	std::sort(vec.begin(), vec.end());

	for (size_t i = 0; i < vec.size(); i++)
		std::cout << int(vec[i]) << std::endl;


	system("pause");
	return 0;
}