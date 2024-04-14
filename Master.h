#pragma once

#include <stdlib.h>
#include <omp.h>
#include <ctime>
#include <unordered_map>
#include <string>
#include <iomanip>
#include "Partition.h"

using namespace std;

bool compareAmount(const double& a, const double& b);

class Master
{
public:

	Partition* partition;
	int ndeggt = 1;

	vector<vector<vector<double> > > candidates; // corresponds the subgraphs each vertex has

	vector<vector<double>>followers;
	unordered_map<double, pair<int,int>>anchor_cost;
	unordered_map<int, unordered_map<double,int>>anchor_cost_c; // The k value promoted to, vector save vertices and their required edges
	unordered_map<pair<double,int>, int, pairHash>anchor_update_level; // （u，k), old-coreness->[u]
	vector<pair<int, int>>anchor_edges;

	vector<double>candidate_anchors;

	int nfs;// the number of all followers;
	int acost; // the number of inserted edges;

	Master(Partition* partition);

	void Anchoring(int b, int modes, string check_dat);

	vector<double> GroupSelection(int b, vector<pair<double, int>>& followers_final, int& group_costs, int& group_followers, double& max_score, vector<pair<double, double>>& group_insert_edges, unordered_map<double, pair<int, int>>& group_anchor_cost);

	void GroupShrink(double v, unordered_map<double, int>&r, unordered_map<double, int>&budget, unordered_map<double, int>&vis_group);

	void GroupExpand(double w, unordered_map<double, int>& r_, unordered_map<double, int>& budget_, unordered_map<double, int>& visited_group,
		unordered_map<double, int>& r, unordered_map<double, int>& budget, unordered_map<double, int>& visit_group, vector<double>& new_group);

	int getGroupFollowers(vector<double>group, vector<pair<double, int>>& followers_gain, unordered_map<double, int>vis_group, int group_k);

	void ShrinkGroups(double v, unordered_map<double, int>& degplus, unordered_map<double, int>& Status, unordered_map<double, int>vis_group, unordered_map<double, int>& Survive, unordered_map<double, double>& mp_vis);

	pair<double, int> getAnchorScore(int b, double& node_score, int& node_followers, int& node_cost, vector<double>& final_followers);

	bool compareCoreness(const double& a, const double& b);

	void Anchor_compute();

	void collectFollowers_vertex(double anchor, vector<double>& followers_i);

	void ShrinkVertex(double v, unordered_map<double, int> &degplus, unordered_map<double, int>& Status);

	void Final_insert(string chec_dat);
};