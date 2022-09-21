#include "List.h"
#include <string>
#include <iostream>
#include <cstring>

int main () {

	std::string abc = "abc";
	char * str = new char [3];
	str[0] = 'a';
	str[1] = 'b';
	str[2] = 'c';
	// FAUTE : on se sert de i pour boucler sur les chaînons à l'envers, la condition
	// d'arrêt étant donc si i est inférieur à zéro dans la boucle for, ce qui n'arrivera
	// jamais avec un size_t qui est non signé, on le change donc en int
	//size_t i = 0;
	int i = 0;

	if (! strcmp (str, abc.c_str())) {
		std::cout << "Equal !";
	}

	pr::List list;
	list.push_front(abc);
	list.push_front(abc);

	std::cout << "Liste : " << list << std::endl;
	std::cout << "Taille : " << list.size() << std::endl;

	// Affiche à l'envers
	for (i= list.size() - 1 ; i >= 0 ; i--) {
		std::cout << "elt " << i << ": " << list[i] << std::endl;
	}

	// liberer les char de la chaine
	
	// FAUTE : str est un pointeur vers un tableau de 3 caractères, et absolument pas
	// un pointeur vers un tableau de 3 pointeurs vers un caractère, le code suivant n'a
	// donc pas lieu d'être.
	/*for (char *cp = str ; *cp ; cp++) {
		delete cp;
	}*/

	// et la chaine elle meme
	
	// FAUTE : on souhaite libérer la mémoire allouée avec un new[]
	// il faut donc utiliser un delete[].
	//delete str;
	delete[] str;
}
