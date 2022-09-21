// FAUTE : il manquait l'include du fichier d'en-tête "List.h"
#include "List.h"

namespace pr {

// ******************* Chainon
Chainon::Chainon (const std::string & data, Chainon * next):data(data),next(next) {};

size_t Chainon::length() {
	size_t len = 1;
	if (next != nullptr) {
		len += next->length();
	}
	// FAUTE : boucle infinie, on veut retourner la valeur de la variable "len" içi.
	return len;
}

// FAUTE : cette fonction print est déclarée comme const dans le fichier d'en-tête
// correspondant. Il manquait donc le mot clé const dans la signature içi.
void Chainon::print (std::ostream & os) const {
	os << data ;
	if (next != nullptr) {
		os << ", ";
		next->print(os);
	}
	// FAUTE : on déréférence next sans vérifier si le pointeur pointe sur un
	// emplacement mémoire valide. Il faut donc le placer dans les accolades du if au dessus
	//next->print(os);
}

// ******************  List
const std::string & List::operator[] (size_t index) const  {
	Chainon * it = tete;
	for (size_t i=0; i < index ; i++) {
		it = it->next;
	}
	return it->data;
}

void List::push_back (const std::string& val) {
	if (tete == nullptr) {
		tete = new Chainon(val);
	} else {
		Chainon * fin = tete;
		while (fin->next) {
			fin = fin->next;
		}
		fin->next = new Chainon(val);
	}
}

// FAUTE : le code de cette fonction est déjà défini dans le fichier d'en-tête
/*void List::push_front (const std::string& val) {
	tete = new Chainon(val,tete);
}*/

// FAUTE : on souhaite définir une fonction membre de la classe List,
// par conséquent il faut utiliser l'opérateur de résolution de portée
// pour indiquer qu'on définit la fonction empty, membre de la classe List
bool List::empty() {
	return tete == nullptr;
}

size_t List::size() const {
	if (tete == nullptr) {
		return 0;
	} else {
		return tete->length();
	}
}

} // namespace pr

std::ostream & operator<< (std::ostream & os, const pr::List & vec)
{
	os << "[";
	if (vec.tete != nullptr) {
		vec.tete->print (os) ;
	}
	os << "]";
	return os;
}

