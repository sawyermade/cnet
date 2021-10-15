/*-------------------------------------------------------------------------------------------------------------/
/ Author: maups                                                                                                /
/ Date: 03/22/2020                                                                                             /
/ TODO: * speed up queries by saving relations per concept                                                     /
/------------------------------------------------------------------------------------------------------------ */
#include <fstream>
#include "conceptnet.hpp"

// Load a processed CSV file (see get_english_edges.cpp)
ConceptNet::ConceptNet(string filename, Dictionary &concepts, set<string> &filter_relations) {
	this->concepts = &concepts;

	// Open file
	ifstream ifp(filename, ifstream::in);

	// Read all lines
	string s;
	while(getline(ifp, s)) {
		// Four columns separated by tab space
		// 1st item -> relation name
		// 2nd item -> start concept name
		// 3rd item -> end concept name
		// 4th item -> edge weight

		// Get relation name
		size_t pos1 = 0, pos2 = s.find('\t', pos1);
		string rel = s.substr(pos1, pos2-pos1);

		if(not filter_relations.empty() && filter_relations.find(rel) == filter_relations.end())
			continue;

		// Get start concept name
		pos1 = pos2+1;
		pos2 = s.find('\t', pos1);
		string start = s.substr(pos1, pos2-pos1);

		if(not concepts.has(start))
			continue;

		// Get start concept name
		pos1 = pos2+1;
		pos2 = s.find('\t', pos1);
		string end = s.substr(pos1, pos2-pos1);

		if(not concepts.has(end))
			continue;

		// Get edge weight
		pos1 = pos2+1;
		double weight = stod(s.substr(pos1));

		// Save relation type
		relations.add(rel);

		int irel = relations.index(rel), istart = concepts.index(start), iend = concepts.index(end);

		// Insert inbonds & outbonds
		outbonds[make_pair(istart,irel)].insert(iend);
		inbonds[make_pair(iend,irel)].insert(istart);

		// Save weight
		edge_weights[make_tuple(istart,irel,iend)].push_back(weight);

	}

	ifp.close();
}

// Look for other concepts with an edge to the query one, set outbonds_only to true if inbonds are not necessary
vector<edge> ConceptNet::query_concept(string concept, bool outbonds_only) {
	vector<edge> r;
	if(concepts->has(concept)) {
		int iconcept = concepts->index(concept);
		for(int irel=0; irel < relations.size(); irel++) {
			// Look for outbonds
			if(outbonds.count(make_pair(iconcept,irel)) > 0)
				for(int iend : outbonds[make_pair(iconcept,irel)])
					for(double w : edge_weights[make_tuple(iconcept,irel,iend)])
						r.push_back(make_tuple(concept,relations.word(irel),concepts->word(iend),w));
			if(!outbonds_only) {
				// Look for inbonds
				if(inbonds.count(make_pair(iconcept,irel)) > 0)
					for(int istart : inbonds[make_pair(iconcept,irel)]) {
						// Do not get 'self'-edges twice
						if(istart == iconcept)
							continue;
						for(double w : edge_weights[make_tuple(istart,irel,iconcept)])
							r.push_back(make_tuple(concepts->word(istart),relations.word(irel),concept,w));
					}
			}
		}
	}
	return r;
}

// Look for edges between concept1 and concept2
vector<edge> ConceptNet::query_edge(string concept1, string concept2, bool order_matters) {
	vector<edge> r;
	int iconcept1 = concepts->index(concept1), iconcept2 = concepts->index(concept2);
	for(int irel=0; irel < relations.size(); irel++) {
		// 1 -> 2
		if(edge_weights.count(make_tuple(iconcept1,irel,iconcept2)) > 0)
			for(double w : edge_weights[make_tuple(iconcept1,irel,iconcept2)])
				r.push_back(make_tuple(concept1,relations.word(irel),concept2,w));
		if(not order_matters)
			// 2 -> 1
			if(edge_weights.count(make_tuple(iconcept2,irel,iconcept1)) > 0)
				for(double w : edge_weights[make_tuple(iconcept2,irel,iconcept1)])
					r.push_back(make_tuple(concept2,relations.word(irel),concept1,w));
	}
	return r;
}

