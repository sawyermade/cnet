#include <iostream>
#include <algorithm>
#include "conceptnet.hpp"
#include "numberbatch.hpp"

#include <cmath>
#include <queue>
#include <assert.h>

#include <iomanip>

double cosine_distance(embedding &a, embedding &b) {
	double A = 0.0, B = 0.0, AB = 0.0;
	assert(a.size() == b.size());
	for(int i=0; i < a.size(); i++) {
		A += a[i]*a[i];
		B += b[i]*b[i];
		AB += a[i]*b[i];
	}
	return 1.0 - AB/(sqrt(A)*sqrt(B));
}

double angular_distance(embedding &a, embedding &b) {
	double A = 0.0, B = 0.0, AB = 0.0;
	assert(a.size() == b.size());
	for(int i=0; i < a.size(); i++) {
		A += a[i]*a[i];
		B += b[i]*b[i];
		AB += a[i]*b[i];
	}
	return acos(AB/(sqrt(A)*sqrt(B)))/M_PI;
}

class Path {
	public:
		set<string> nodes;
		string last, end;
		vector<edge> edges;
		double cost, heuristic;
		Path(string s, string t = "", double wh = 0.0) {
			nodes.clear();
			edges.clear();
			nodes.insert(s);
			last = s;
			cost = 0.0;
			heuristic = wh;
			end = t;
		}
		bool add(edge e, double w, double wh = 0.0) {
			//if(edges.size() >= 3)
			//	return false;
			string s;
			if(get<0>(e) == last && get<2>(e) != last && nodes.find(get<2>(e)) == nodes.end())
				s = get<2>(e);
			else if(get<2>(e) == last && get<0>(e) != last && nodes.find(get<0>(e)) == nodes.end())
				s = get<0>(e);
			else
				return false;
			last = s;
			nodes.insert(s);
			edges.push_back(e);
			cost += w;
			heuristic = wh;
			return true;
		}
};

class ComparePaths{
	public:
		bool operator() (Path p, Path q) {
			return p.cost+p.heuristic > q.cost+q.heuristic;
		}
};

void k_shortest_paths(string start, string end, ConceptNet &cnet, NumberBatch &numb, int k = 10) {
	priority_queue<Path, vector<Path>, ComparePaths> pq;

	Path s(start);
	pq.push(s);

	int kc = 0;
	while(kc < k && not pq.empty()) {
		Path p = pq.top();
		pq.pop();

		if(p.last == end) {
			kc++;
			cout << "PATH #" << kc << endl;
			cout << "--------------------------------------" << endl;
			for(edge e : p.edges)
				cout << get<0>(e) << " >>>" << get<1>(e) << ">>> " << get<2>(e) << " [" << get<3>(e) << "]" << endl;
			cout << "--------------------------------------" << endl;
			cout << "TOTAL COST: " << p.cost << endl;
			cout << "--------------------------------------" << endl << endl;
		}
		else {
			vector<edge> edges = cnet.query_concept(p.last, false);
			std::sort(edges.rbegin(), edges.rend(), [](edge const& a, edge const& b){ return get<3>(a) < get<3>(b); });
			set<string> concepts;
			for(edge e : edges) {
				Path q = p;
				double w = angular_distance(numb.get(get<0>(e)), numb.get(get<2>(e)));
				if(q.add(e, w))
					if(concepts.find(q.last) == concepts.end()) {
						vector<string> qlast_eq = numb.get_equivalent(q.last);
						for(string eq : qlast_eq)
							concepts.insert(eq);
						concepts.insert(q.last);
						pq.push(q);
					}
			}
		}
	}
	cout << pq.size() << endl;
}

void k_shortest_paths_astar(string start, string end, ConceptNet &cnet, NumberBatch &numb, int k = 10) {
	priority_queue<Path, vector<Path>, ComparePaths> pq;

	cout << "--------------------------------------" << endl;
	cout << "Top-" << k << " paths between " << start << " and " << end << endl;
	cout << "--------------------------------------" << endl << endl;

	Path s(start,end,angular_distance(numb.get(start), numb.get(end)));
	pq.push(s);

	int kc = 0;
	while(kc < k && not pq.empty()) {
		Path p = pq.top();
		pq.pop();

		if(p.last == end) {
			kc++;
			cout << "PATH #" << kc << " -> " << p.cost << endl;
			cout << "--------------------------------------" << endl;
			for(edge e : p.edges)
				cout << get<0>(e) << " >>>" << get<1>(e) << ">>> " << get<2>(e) << " [" << get<3>(e) << "]" << endl;
			cout << endl;
			//cout << "--------------------------------------" << endl;
			//cout << "TOTAL COST: " << p.cost << endl;
			//cout << "--------------------------------------" << endl << endl;
		}
		else {
			vector<edge> edges = cnet.query_concept(p.last, false);
			std::sort(edges.rbegin(), edges.rend(), [](edge const& a, edge const& b){ return get<3>(a) < get<3>(b); });
			set<string> concepts;
			for(edge e : edges) {
				Path q = p;
				double w = angular_distance(numb.get(get<0>(e)), numb.get(get<2>(e)));
				double wh = (p.last == get<0>(e) ? angular_distance(numb.get(end), numb.get(get<2>(e))) : angular_distance(numb.get(get<0>(e)), numb.get(end)));
				if(q.add(e, w, wh))
					if(concepts.find(q.last) == concepts.end()) {
						vector<string> qlast_eq = numb.get_equivalent(q.last);
						for(string eq : qlast_eq)
							concepts.insert(eq);
						concepts.insert(q.last);
						pq.push(q);
					}
			}
		}
	}
	cout << pq.size() << endl;
}

// Pandora box
int main(int argc, char **argv) {
	// Usage instructions
	if(argc != 3) {
		cerr << "Usage: " << argv[0] << " [numberbatch.txt] [input_edges.csv]" << endl;
		return 1;
	}

	// Load NumberBatch embeddings
	NumberBatch numb(argv[1]);
	cout << "NUMBERBATCH DONE!" << endl << endl;

	// Load ConceptNet edges
	set<string> relations = {"Antonym", "AtLocation", "CapableOf", "Causes", "CausesDesire", "CreatedBy", "DefinedAs", "DerivedFrom", "Desires", "DistinctFrom", "Entails", "EtymologicallyDerivedFrom", "EtymologicallyRelatedTo", "FormOf", "HasA", /*"HasContext",*/ "HasFirstSubevent", "HasLastSubevent", "HasPrerequisite", "HasProperty", "HasSubevent", "InstanceOf", "IsA", "LocatedNear", "MadeOf", "MannerOf", "MotivatedByGoal", "NotCapableOf", "NotDesires", "NotHasProperty", "PartOf", "ReceivesAction", /*"RelatedTo",*/ "SimilarTo", "SymbolOf", "Synonym", "UsedFor"/*, "dbpedia"*/};

	ConceptNet cnet(argv[2], numb.concepts, relations);
	cout << "CONCEPTNET DONE!" << endl << endl;

	k_shortest_paths_astar("cracking", "egg", cnet, numb);
	//k_shortest_paths_astar("put", "topping", cnet, numb);
	//k_shortest_paths_astar("put_over", "topping", cnet, numb);
	//k_shortest_paths_astar("put", "bun", cnet, numb);
	//k_shortest_paths_astar("put_together", "bun", cnet, numb);

}

