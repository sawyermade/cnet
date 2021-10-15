/*-------------------------------------------------------------------------------------------------------------/
/ Author: maups                                                                                                /
/ Date: 03/22/2020                                                                                             /
/------------------------------------------------------------------------------------------------------------ */
#ifndef __DICTIONARY__
#define __DICTIONARY__

#include <map>
#include <vector>
#include <string>

using namespace std;

// Dictionary of concepts
class Dictionary {
	private:
		// hash table for concepts
		map<string,int> word_to_index;
		// inverted index
		vector<string> index_to_word;
	public:
		Dictionary();
		// check if concept is in the dictionary
		bool has(string);
		// add concept to the dictionary
		void add(string);
		// get index of concept in the dictionary
		int index(string);
		// get concept in the dictionary from index
		string word(int);
		// get dictionary size
		int size();
};

#endif

