/*-------------------------------------------------------------------------------------------------------------/
/ Author: maups                                                                                                /
/ Date: 03/22/2020                                                                                             /
/------------------------------------------------------------------------------------------------------------ */
#ifndef __CONCEPTNET__
#define __CONCEPTNET__

#include <map>
#include <set>
#include <vector>
#include "dictionary.hpp"

using namespace std;

// Start concept, relation, end concept, weight
#define edge tuple<string,string,string,double>

// ConceptNet graph
class ConceptNet {
	private:
		// vector of weights for edges (edges may have duplicates with different weights on ConceptNet)
		map<tuple<int,int,int>, vector<double>> edge_weights;
		// maps pair<concept,relation> to conceps
		map<pair<int,int>,set<int>> inbonds, outbonds;
		// set of relation types
		Dictionary relations;
		// dictionary of concepts
		Dictionary *concepts;
	public:
		// Load a processed CSV file (see get_english_edges.cpp) and ignore concepts that are not in the dictionary
		ConceptNet(string, Dictionary &, set<string> &);

		// Look for other concepts with an edge to the query one, set outbonds_only to true if inbonds are not necessary
		vector<edge> query_concept(string, bool);
		// Look for edges between concept1 and concept2, set order_matters to true to discard edges from concept2 to concept1
		vector<edge> query_edge(string, string, bool);
};

#endif

