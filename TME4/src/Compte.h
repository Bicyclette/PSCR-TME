#pragma once
#include <thread>
#include <mutex>

namespace pr {

class Compte {
	mutable std::recursive_mutex m;
	int solde;
	bool visite_comptable; // question 10
public :
	Compte(int solde=0):solde(solde),visite_comptable(false) {}
	Compte(const Compte & other);
	void crediter (unsigned int val) ;
	bool debiter (unsigned int val) ;
	int getSolde() const  ;
	std::recursive_mutex& getMutex();
	bool has_been_visited() const;
	void set_visite_comptable(bool v);
	// lockable
	void lock() const;
	void unlock() const;
	bool try_lock() const;
};

}
