/*-------------------------------------------------------------------------------------------------------------/
/ Author: maups                                                                                                /
/ Date: 03/22/2020                                                                                             /
/------------------------------------------------------------------------------------------------------------ */
#include <fstream>
#include "numberbatch.hpp"

// Disjoint Set Union (DSU) with path compression
class DSU{
	public:
		vector<int> parent;
		DSU(int n): parent(n) { for(int i=0; i<n; i++) parent[i] = i; }
		void join(int a, int b) { parent[find(b)] = find(a); }
		int find(int a) { return a == parent[a] ? a : parent[a] = find(parent[a]); }
		bool check(int a, int b) { return find(a) == find(b); }
};

// Load a TXT file
NumberBatch::NumberBatch(string filename) {
	// Open file
	ifstream ifp(filename, ifstream::in);

	// Read number of concepts and size of embedding
	int n, size;
	ifp >> n >> size;

	// Equivalent terms as a DSU
	DSU dsu(n);
	// Use first embedding value as key to speed up the search for equivalent terms while saving memory
	map<float,vector<int>> hash_emb;

	// Read all concepts and their respective embeddings
	string s;
	for(int i=0; i < n; i++) {
		ifp >> s;
		concepts.add(s);

		embedding emb(size);
		for(int j=0; j < size; j++)
			ifp >> emb[j];
		embeddings.push_back(emb);

		// Look for equivalent terms
		bool flag = true;
		for(int j : hash_emb[emb[0]])
			if(not dsu.check(i, j))
				if(emb == embeddings[j]) {
					dsu.join(i, j);
					flag = false;
					break;
				}
		if(flag)
			hash_emb[emb[0]].push_back(i);
	}

	ifp.close();

	// Compute groups of equivalent concepts
	map<int,vector<int>> groups;

	for(int i=0; i < n; i++)
		if(dsu.find(i) != i)
			groups[dsu.find(i)].push_back(i);

	group_index.resize(n, -1);
	int gcount = 0;
	for(auto it = groups.begin(); it != groups.end(); it++, gcount++) {
		vector<string> v;
		v.push_back(concepts.word(it->first));
		group_index[it->first] = gcount;
		for(int i : it->second) {
			v.push_back(concepts.word(i));
			group_index[i] = gcount;
		}
		grouped_concepts.push_back(v);
	}
}

embedding& NumberBatch::get(string concept) {
	if(concepts.has(concept))
		return embeddings[concepts.index(concept)];
	throw "Concept not found";
}

embedding& NumberBatch::get(int index) {
	if(index < embeddings.size())
		return embeddings[index];
	throw "Concept not found";
}

vector<string>& NumberBatch::get_equivalent(string concept) {
	static vector<string> empty;
	if(concepts.has(concept)) {
		if(group_index[concepts.index(concept)] >= 0)
			return grouped_concepts[group_index[concepts.index(concept)]];
		else
			return empty;
	}
	else
		throw "Concept not found";
}

/*
#include <iostream>
#include <assert.h>

bool equal(embedding &a, embedding &b) {
	assert(a.size() == b.size());
	for(int i=0; i < a.size(); i++)
		if(a[i] != b[i])
			return false;
	return true;
}

void NumberBatch::findEquivalentTerms() {
	DSU dsu(embeddings.size());

	map<float,vector<int>> h;
	for(int i=0; i < embeddings.size(); i++)
		h[embeddings[i][0]].push_back(i);

	for(int i=0; i < embeddings.size(); i++) {
		for(int j : h[embeddings[i][0]])
		//for(int j=i+1; j < embeddings.size(); j++)
			if(j > i and not dsu.check(i, j))
				if(equal(embeddings[i], embeddings[j]))
					dsu.join(i, j);
		cout << "\r" << i*100.0/embeddings.size();
	}
	cout << endl;

	map<int,vector<int>> s;

	int n=0;
	for(int i=0; i < embeddings.size(); i++)
		if(dsu.find(i) == i)
			n++;
		else
			s[dsu.find(i)].push_back(i);

	cout << "# nodes: " <<	embeddings.size() << endl;
	cout << "# h nodes: " << h.size() << endl;
	cout << "# unique nodes: " << n << endl;
	cout << "# sets: " << s.size() << endl;

	int M = dsu.find(concepts.index("jazz_apple"));
	cout << concepts.word(M);
	for(int i : s[M])
		cout << " " << concepts.word(i);
	cout << endl;
}
*/

