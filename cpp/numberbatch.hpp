/*-------------------------------------------------------------------------------------------------------------/
/ Author: maups                                                                                                /
/ Date: 03/22/2020                                                                                             /
/------------------------------------------------------------------------------------------------------------ */
#ifndef __NUMBERBATCH__
#define __NUMBERBATCH__

#include <vector>
#include "dictionary.hpp"

using namespace std;

#define embedding vector<float>

// ConceptNet NumberBatch embeddings
class NumberBatch {
	private:
		// vector of concept embeddings
		vector<embedding> embeddings;
	public:
		Dictionary concepts;

		// groups of concepts with the same embedding
		vector<int> group_index;
		vector<vector<string>> grouped_concepts;

		// Load a TXT file
		NumberBatch(string);

		// Return concept embedding
		embedding& get(string);
		embedding& get(int);

		// Return group of equivalent concepts
		vector<string>& get_equivalent(string);
};

#endif

