#pragma once

#include <unordered_set>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <set>
#include <queue>
#include "DataGraph.h"

#define INITIAL -1
#define MAX_COST 1e9

using namespace std;

bool compareDegree(const double& a, const double& b);

class Partition
{
public:

	DataGraph* datagraph;
	static vector<int> coreness; // VLDB2015's d
	/*
	* vec_seq: ascending total order of (#shell, #layer)
	* vec_ele: vertex ID
	*/
	vector<double> total_order; // VLDB2015's D
	/*
	* vec_seq: vertex ID
	* vec_ele: vector position of the vertex in total_order
	*/
	vector<double> order_pointer; // VLDB2015's p
	/*
	* vec_seq: k-shell number
	* vec_ele: the start position of vertices having this #shell in total_order
	*/
	vector<double> shell_tag; // VLDB2015's b
	unordered_set<double> anchor_verts; // the set of chosen anchored vertices
	vector<int>shells; // shell-layer
	vector<int>deggt;  //deg>=
	vector<unordered_map<int, int>>nbrk; // nbr <k,number>
	int max_degree; // auxiliary variable help adapt to VLDB2015's algorithm
	int max_coreness = 0;
	int max_layer = 0;
	vector<int>coreness_ins;
	long long pre_coreness = -1;
	long long after_coreness = 0;

	Partition(DataGraph* datagraph);

	void coreDecomposition();
	void layerDecomposition();
	void P_Decomposition();
	void P_Maintenance(double anchor, int k, vector<double>followers);
	void GroupMaintenance(vector<double>group, vector<pair<double, int>>& followers_final);
};