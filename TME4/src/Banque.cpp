#include "Banque.h"

#include <iostream>

using namespace std;

namespace pr {

void Banque::transfert(size_t deb, size_t cred, unsigned int val) {
	//std::unique_lock<std::mutex> l(m); // question 9
	Compte & debiteur = comptes[deb];
	Compte & crediteur = comptes[cred];
	std::lock(debiteur.getMutex(), crediteur.getMutex()); // question 6 (ordre total des locks)
	//debiteur.lock(); question 4
	//crediteur.lock(); question 4
	if(debiteur.has_been_visited() || crediteur.has_been_visited()) {
		crediteur.unlock();
		debiteur.unlock();
		return;
	}
	if (debiteur.debiter(val)) {
		crediteur.crediter(val);
	}
	crediteur.unlock();
	debiteur.unlock();
}
size_t Banque::size() const {
	return comptes.size();
}
bool Banque::comptabiliser (int attendu) {
	//std::unique_lock<std::mutex> l(m); // question 9
	int bilan = 0;
	int id = 0;
	for (auto & compte : comptes) {
		compte.set_visite_comptable(true);
		if (compte.getSolde() < 0) {
			cout << "Compte " << id << " en négatif : " << compte.getSolde() << endl;
		}
		bilan += compte.getSolde();
		id++;
	}
	if (bilan != attendu) {
		cout << "Bilan comptable faux : attendu " << attendu << " obtenu : " << bilan << endl;
	}
	return bilan == attendu;
}
void Banque::reset_visite_compta() { // question 10
	for(auto & compte : comptes) {
		compte.set_visite_comptable(false);
	}
}
}
