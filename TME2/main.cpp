#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include <algorithm>
#include "hashmap.hpp"

typedef std::chrono::time_point<std::chrono::steady_clock> instant_t;

void add_word_if_not_inside(std::vector<std::pair<std::string, int>>& v, std::string& word)
{
	size_t index = 0;
	bool found = false;
	for(;index < v.size(); ++index){
		std::string& str = v[index].first;
		if(str == word){found = true; break;}
	}
	if(!found){
		v.emplace_back(word, 1);
	}
	else {
		v[index].second += 1;
	}
}

int get_entry_count(std::vector<std::pair<std::string, int>>& v, const std::string & key)
{
	for(auto& e : v)
	{
		if(e.first == key) {
			return e.second;
		}
	}
	return 0;
}

int main () {
	using namespace std;
	using namespace std::chrono;

	ifstream input = ifstream("WarAndPeace.txt");

	instant_t start = steady_clock::now();
	instant_t end;

	size_t nombre_lu = 0;
	// prochain mot lu
	string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");
	
	// ###########################################################################
	cout << "###################################################" << endl;
	cout << "Parsing War and Peace (version without HashMap)" << endl;
	// vecteur stockant les mots du fichier texte en leur associant leur occurence
	vector<pair<string, int>> all_words;
	
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);

		// word est maintenant "tout propre"
		//if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			//cout << nombre_lu << ": "<< word << endl;
		nombre_lu++;

		// ajout du mot dans le vecteur de mots
		// si et seulement si ce mot n'est pas dans le vecteur
		add_word_if_not_inside(all_words, word);
	}
	input.close();
	input.open("WarAndPeace.txt");
	cout << "Finished Parsing War and Peace" << endl;

	end = steady_clock::now();
    cout << "Parsing with vector of pair method took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "\tFound a total of " << nombre_lu << " words." << endl;
	cout << "\tFound a total of " << all_words.size() << " different words." << endl;
	
	int war_count = get_entry_count(all_words, "war");
	int peace_count = get_entry_count(all_words, "peace");
	int toto_count = get_entry_count(all_words, "toto");
	cout << "\tWord \"war\" appears " << war_count << " times." << endl;
	cout << "\tWord \"peace\" appears " << peace_count << " times." << endl;
	cout << "\tWord \"toto\" appears " << toto_count << " times." << endl;
	
	// ###########################################################################
	// variante avec une hashmap
	start = steady_clock::now();
	cout << "###################################################" << endl;
	cout << "Parsing War and Peace (version with HashMap)" << endl;
	pr::HashMap<string, int> all_words_bis(600000);
	nombre_lu = 0;
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);
		
		nombre_lu++;

		// ajout du mot dans la hashmap
		// si et seulement si ce mot n'est pas déjà stocké
		// sinon, on incrémente le compteur associé
		int * value = all_words_bis.get(word);
		if (value != nullptr) {
			all_words_bis.put(word, (*value)+1);
		} else {
			all_words_bis.put(word, 1);
		}
	}
	input.close();
	cout << "Finished Parsing War and Peace" << endl;

	end = steady_clock::now();
    cout << "Parsing with HashMap method took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "\tFound a total of " << nombre_lu << " words." << endl;
	cout << "\tFound a total of " << all_words_bis.size() << " different words." << endl;

	int* war_ptr = all_words_bis.get("war");
	int* peace_ptr = all_words_bis.get("peace");
	int* toto_ptr = all_words_bis.get("toto");
	if(war_ptr) {
		cout << "\tWord \"war\" appears " << *war_ptr << " times." << endl;
	} else {
		cout << "\tWord \"war\" appears 0 times." << endl;
	}
	if(peace_ptr) {
		cout << "\tWord \"peace\" appears " << *peace_ptr << " times." << endl;
	} else {
		cout << "\tWord \"peace\" appears 0 times." << endl;
	}
	if(toto_ptr) {
		cout << "\tWord \"toto\" appears " << *toto_ptr << " times." << endl;
	} else {
		cout << "\tWord \"toto\" appears 0 times." << endl;
	}
	
	// ###########################################################################
	// initialisation d'un std::vecor<std::pair<std::string, int>>
	// par copie des entrées dans la table de hachage

	auto buckets = all_words_bis.get_buckets();
	vector<pair<string, int>> text_words(600000);
	for(auto l : buckets){
		for(auto e : l) {
			text_words.emplace_back(e.key, e.val);
		}
	}

	// affichage des 10 mots les plus fréquents
	std::sort(text_words.begin(), text_words.end(),
			[] (const pair<string, int> & a, const pair<string, int> & b) {
				return a.second > b.second;
			});
	cout << "###################################################" << endl;
	cout << "Printing the 10 most frequently used words" << endl;
	for(int i = 0; i < 10; ++i) {
		cout << "\tThe word \"" << text_words[i].first << "\" appears " << text_words[i].second;
		cout << " times." << endl;
	}

	// ###########################################################################
	// TME3 : test des fonctions count et count_if_equal
	size_t c = all_words_bis.count(all_words_bis.begin(), all_words_bis.end());
	std::cout << "There are " << c << " elements." << std::endl;
	return 0;
}


