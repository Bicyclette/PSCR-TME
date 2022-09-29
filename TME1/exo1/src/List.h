#ifndef SRC_LIST_H_
#define SRC_LIST_H_

#include <cstddef>
#include <string>
#include <ostream>

namespace pr {

class Chainon {
public :
	std::string data;
	Chainon * next;
	Chainon (const std::string & data, Chainon * next=nullptr);
	size_t length() ;
	void print (std::ostream & os) const;
};

class List {
public:

	Chainon * tete;

	List(): tete(nullptr)  {}

	~List() {
		for (Chainon * c = tete ; c ; ) {
			Chainon * tmp = c->next;
			delete c;
			c = tmp;
		}
	}

	const std::string & operator[] (size_t index) const ;

	void push_back (const std::string& val) ;

	void push_front (const std::string& val) {
		tete = new Chainon(val,tete);
	}

	bool empty() ;

	size_t size() const ;
};

// FAUTE : dans le fichier List.cpp cette fonction a sa définition
// à l'extérieur du namespace pr, par conséquent on la place aussi
// à l'extérieur du namespace pr dans le header et on préfixe List avec pr => pr::List
//std::ostream & operator<< (std::ostream & os, const List & vec) ;

} /* namespace pr */

std::ostream & operator<< (std::ostream & os, const pr::List & vec) ;

#endif /* SRC_LIST_H_ */
