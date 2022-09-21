#include <iostream>

int main()
{
	std::cout << "Hello world" << std::endl;

	int tab[10] = {0,1,2,3,4,5,6,7,8,9};
	for(int i = 0; i < 10; ++i){
		std::cout << "tab[" << i << "] = " << tab[i] << std::endl;
	}

	for(int i = 9; i >= 0; i--){
		if(i > 0 && tab[i] - tab[i-1] != 1){
			std::cout << "problème !" << std::endl;
		}
	}

	return 0;
}
