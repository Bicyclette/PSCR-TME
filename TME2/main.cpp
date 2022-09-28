#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include <algorithm>
#include "hashmap.hpp"

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

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	size_t nombre_lu = 0;
	// prochain mot lu
	string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	regex re( R"([^a-zA-Z])");
	// vecteur stockant les mots du fichier texte
	vector<pair<string, int>> all_words;
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word = regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);

		// word est maintenant "tout propre"
		/*
		if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": "<< word << endl;
		*/
		nombre_lu++;

		// ajout du mot dans le vecteur de mots
		// si et seulement si ce mot n'est pas dans le vecteur
		add_word_if_not_inside(all_words, word);
	}
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    cout << "Found a total of " << nombre_lu << " words." << endl;
    
	cout << "Found a total of " << all_words.size() << " different words." << endl;
	int war_count = get_entry_count(all_words, "war");
	int peace_count = get_entry_count(all_words, "peace");
	int toto_count = get_entry_count(all_words, "toto");
	cout << "Word \"war\" appears " << war_count << " times." << endl;
	cout << "Word \"peace\" appears " << peace_count << " times." << endl;
	cout << "Word \"toto\" appears " << toto_count << " times." << endl;

    return 0;
}


