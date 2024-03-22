#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>
#include <iostream>
#include <sstream>

using namespace std;

template <typename T>
inline void hash_combine(std::size_t& seed, const T& val) {
	seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
// auxiliary generic functions to create a hash value using a seed
template <typename T> inline void hash_val(std::size_t& seed, const T& val) {
	hash_combine(seed, val);
}
template <typename T, typename... Types>
inline void hash_val(std::size_t& seed, const T& val, const Types &... args) {
	hash_combine(seed, val);
	hash_val(seed, args...);
}

template <typename... Types>
inline std::size_t hash_val(const Types &... args) {
	std::size_t seed = 0;
	hash_val(seed, args...);
	return seed;
}

struct pairHash {
	template <class T1, class T2>
	std::size_t operator()(const std::pair<T1, T2>& p) const {
		return hash_val(p.first, p.second);
	}
};

class DataGraph
{
public:
	/*
	* dimension 1: vertex ID := iterator sequence
	* dimension 2: the list of adjacent verticess
	*/
	vector<vector<double> > AdjList;
	unordered_map<double, double> id2seq; // map the vertex from ordinary dataset id to AdjList sequence
	unordered_map<double, double> seq2id; // map the sequence back to the id for presentation convenience
	unordered_map<pair<double, double>, double, pairHash>HashEdges;

	DataGraph(string fileName);
	void add_edges(string fileName2);
};
