#include "Banque.h"
#include <random>
#include <iostream>

class RandomIntGenerator
{
	std::random_device m_rand_device;
	std::mt19937 m_rand_engine;
	std::uniform_int_distribution<int> m_distribution;

	public:
	RandomIntGenerator(int from, int to) :
		m_rand_engine(m_rand_device()),
		m_distribution(from, to)
	{}

	int gen() { return m_distribution(m_rand_engine); }
};

using namespace std;

const int NB_THREAD = 10;
const size_t NB_COMPTES = 100;
const size_t SOLDEINITIAL = 100'000;

void worker(pr::Banque & bank)
{
	// générateurs de nombres aléatoires
	RandomIntGenerator rng_montant(1, 100);
	RandomIntGenerator rng_compte(0, bank.size()-1);

	for(int i = 0; i < 1000; ++i)
	{
		// choisis deux indices de compte de la banque
		int c1 = rng_compte.gen();
		int c2 = rng_compte.gen();
		// choix d'un montant aléatoire
		int m = rng_montant.gen();
		bank.transfert(c1, c2, m);
		RandomIntGenerator rng_sleep(0, 20);
		std::this_thread::sleep_for(std::chrono::milliseconds(rng_sleep.gen()));
	}
}

int main () {
	pr::Banque bank(NB_COMPTES, SOLDEINITIAL);
	vector<thread> threads;

	// TODO : creer des threads qui font ce qui est demandé
	for(int i = 0; i < NB_THREAD; ++i) {
		threads.emplace_back(worker, std::ref(bank));
	}

	for (auto & t : threads) {
		t.join();
	}

	// TODO : tester solde = NB_THREAD * JP
	if(bank.comptabiliser(SOLDEINITIAL * NB_COMPTES)) {
		std::cout << "ALL GOOD !" << std::endl;
	} else {
		std::cout << "NOT ALL GOOD !" << std::endl;
	}

	return 0;
}
