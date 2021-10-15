/*-------------------------------------------------------------------------------------------------------------/
/ Author: maups                                                                                                /
/ Date: 03/22/2020                                                                                             /
/------------------------------------------------------------------------------------------------------------ */
#include "dictionary.hpp"

Dictionary::Dictionary() {
	word_to_index.clear();
	index_to_word.clear();
}

bool Dictionary::has(string word) {
	return word_to_index.count(word) > 0;
}

void Dictionary::add(string word) {
	if(not this->has(word)) {
		word_to_index[word] = index_to_word.size();
		index_to_word.push_back(word);
	}
}

int Dictionary::index(string word) {
	if(this->has(word))
		return word_to_index[word];
	else
		return -1;
}

string Dictionary::word(int index) {
	return index_to_word[index];
}

int Dictionary::size() {
	return index_to_word.size();
}

