#include "Master.h"


bool Master::compareCoreness(const double& a, const double& b)
{
	return partition->coreness[a] < partition->coreness[b];
}

Master::Master(Partition* partition)
{
	this->partition = partition;
	acost = 0;
	nfs = 0;
}


void Master::Anchoring(int b, int modes, string check_dat)
{
	double t_begin = (double)clock();
	partition->P_Decomposition();
	int round = 0;
	double group_anchor_time = 0.0, vertex_anchor_time = 0.0;
	while (acost < b)
	{
		cout << " -- Anchoring round: " << round << endl;
		double vertex_begin = (double)clock();
		double node_score = 0;
		int node_followers = -1, node_cost = -1;
		vector<double>final_followers;
		pair<double, int> ans = {-1,0};
		if (modes != 1)
		{
			ans = getAnchorScore(b, node_score, node_followers, node_cost, final_followers);
		}
		double vertex_end = (double)clock();
		vertex_anchor_time += vertex_end - vertex_begin;

		double group_begin = (double)clock();
		vector<pair<double, int>>followers_final;
		int group_costs = -1, group_followers = -1;
		double max_score = -1;
		vector<pair<double, double>>group_insert_edges;
		unordered_map<double, pair<int, int>> group_anchor_cost;
		vector<double>group;
		if (modes != 0)
		{
			group = GroupSelection(b, followers_final, group_costs, group_followers, max_score, group_insert_edges, group_anchor_cost);
		}
		double group_end = (double)clock();
		group_anchor_time += group_end - group_begin;
		if (group.size() == 0 && ans.first == -1)break;
		if (node_score > max_score)
		{
			if (ans.first == -1) { break; }
			acost += node_cost;
			nfs += node_followers;
			int tok = ans.second;
			double node = ans.first;
			int nums = node_cost;

			vector<double>id;
			for (int i = 0; i < partition->datagraph->AdjList.size(); i++)
			{
				id.push_back(i);
			}
			std::sort(id.begin(), id.end(), compareDegree);
			std::reverse(id.begin(), id.end());

			int pos = 0;
			while (nums)
			{
				while (partition->coreness[id[pos]] >= tok)
				{
					if (id[pos] == node)
					{
						pos++;
						continue;
					}
					if (partition->datagraph->HashEdges.find({ node, id[pos] }) == partition->datagraph->HashEdges.end())
					{
						break;
					}
					pos++;
				}
				nums--;
				anchor_edges.push_back({ id[pos],node });
				partition->datagraph->AdjList[id[pos]].push_back(node);
				partition->datagraph->AdjList[node].push_back(id[pos]);
				partition->datagraph->HashEdges.insert({ {id[pos],node},1 });
				partition->datagraph->HashEdges.insert({ {node, id[pos]},1 });
				int u = id[pos], v = node;
				int cu = partition->coreness[u], cv = partition->coreness[v];
				if (partition->nbrk[u].find(cv) != partition->nbrk[u].end())
				{
					partition->nbrk[u][cv]++;
				}
				else
				{
					partition->nbrk[u].insert({ cv,1 });
				}

				if (partition->nbrk[v].find(cu) != partition->nbrk[v].end())
				{
					partition->nbrk[v][cu]++;
				}
				else
				{
					partition->nbrk[v].insert({ cu,1 });
				}
				if (cu >= cv)
				{
					partition->deggt[v]++;
				}
				if (cv >= cu)
				{
					partition->deggt[u]++;
				}
				pos++;
			}

			partition->P_Maintenance(ans.first, ans.second, final_followers);
			cout << "node" << endl;
		}
		else
		{
			if (group.size() == 0)break;
			acost += group_costs;
			nfs += group_followers;
			for (auto tt : group_insert_edges)
			{
				anchor_edges.push_back({ tt.first,tt.second });
				partition->datagraph->AdjList[tt.first].push_back(tt.second);
				partition->datagraph->AdjList[tt.second].push_back(tt.first);
				partition->datagraph->HashEdges.insert({ {tt.first,tt.second},1 });
				partition->datagraph->HashEdges.insert({ {tt.second,tt.first},1 });
				int u = tt.first, v = tt.second;
				int cu = partition->coreness[u], cv = partition->coreness[v];
				if (partition->nbrk[u].find(cv) != partition->nbrk[u].end())
				{
					partition->nbrk[u][cv]++;
				}
				else
				{
					partition->nbrk[u].insert({ cv,1 });
				}

				if (partition->nbrk[v].find(cu) != partition->nbrk[v].end())
				{
					partition->nbrk[v][cu]++;
				}
				else
				{
					partition->nbrk[v].insert({ cu,1 });
				}
				if (cu >= cv)
				{
					partition->deggt[v]++;
				}
				if (cv >= cu)
				{
					partition->deggt[u]++;
				}
			}
			vector<double>id;
			for (int i = 0; i < partition->datagraph->AdjList.size(); i++)
			{
				id.push_back(i);
			}
			std::sort(id.begin(), id.end(), compareDegree);
			std::reverse(id.begin(), id.end());

			for (auto tt : group_anchor_cost)
			{
				double node = tt.first;
				int nums = tt.second.first;
				int tok = tt.second.second;

				int pos = 0;
				while (nums)
				{
					while (partition->coreness[id[pos]] >= tok)
					{
						if (group_anchor_cost.find(id[pos]) != group_anchor_cost.end())
						{
							pos++;
							continue;
						}
						if (partition->datagraph->HashEdges.find({ node, id[pos] }) == partition->datagraph->HashEdges.end())
						{
							break;
						}
						pos++;
					}
					nums--;
					anchor_edges.push_back({ id[pos],node });
					partition->datagraph->AdjList[id[pos]].push_back(node);
					partition->datagraph->AdjList[node].push_back(id[pos]);
					partition->datagraph->HashEdges.insert({ {id[pos],node},1 });
					partition->datagraph->HashEdges.insert({ {node, id[pos]},1 });
					int u = id[pos], v = node;
					int cu = partition->coreness[u], cv = partition->coreness[v];
					if (partition->nbrk[u].find(cv) != partition->nbrk[u].end())
					{
						partition->nbrk[u][cv]++;
					}
					else
					{
						partition->nbrk[u].insert({ cv,1 });
					}

					if (partition->nbrk[v].find(cu) != partition->nbrk[v].end())
					{
						partition->nbrk[v][cu]++;
					}
					else
					{
						partition->nbrk[v].insert({ cu,1 });
					}
					if (cu >= cv)
					{
						partition->deggt[v]++;
					}
					if (cv >= cu)
					{
						partition->deggt[u]++;
					}
					pos++;
				}

			}
			partition->GroupMaintenance(group, followers_final);
			cout << "group" << endl;
		}
		round++;
	}
	double t_end = (double)clock();
	cout << "the anchoring time is:" << (t_end - t_begin) / CLOCKS_PER_SEC << "s." << endl;
	cout << "the group time is:" << group_anchor_time / CLOCKS_PER_SEC << "s." << endl;
	cout << "the vertex time is:" << vertex_anchor_time / CLOCKS_PER_SEC << "s." << endl;

	Final_insert(check_dat);
}


void Master::Anchor_compute()
{
	followers.clear();
	int cnt = 0;
	for (double i = 0; i < partition->datagraph->AdjList.size(); i++)
	{
		followers.push_back(vector<double>());
		cnt++;
		collectFollowers_vertex(i, followers[i]);
	}
	//cout << cnt << endl;
	for (int i = 0; i < followers.size(); i++)sort(followers[i].begin(), followers[i].end(), compareDegree);
}

void Master::collectFollowers_vertex(double anchor, vector<double>& followers_i)
{
	priority_queue<pair<double, double>, vector<pair<double, double>>, greater<pair<double, double>> > H; // 1 为优先级 core+ 0.1 layer 2为顶点
	unordered_map<double, int>degplus;
	double num_N = partition->max_layer + 100.0;
	H.push({ partition->coreness[anchor] * 1.0 + partition->shells[anchor] * 1.0 / num_N, anchor });
	unordered_map<double, int>Status; // 0 survive 1 outof survive
	unordered_map<double, int>inQueue; 
	inQueue.insert({ anchor,1 });
	while (H.size())
	{
		double v = H.top().second;
		H.pop();
		inQueue.erase(v);
		int degplusv = 0;
		if (v == anchor && Status.find(v) == Status.end())
		{
			for (auto u : partition->datagraph->AdjList[v])
			{
				if (Status.find(u) == Status.end())
				{
					if (partition->coreness[v] == partition->coreness[u] && partition->shells[v] < partition->shells[u] && inQueue.find(u)==inQueue.end())
					{
						H.push({ partition->coreness[u] * 1.0 + partition->shells[u] * 1.0 / num_N, u });
						inQueue.insert({ u,1 });
					}
					if (partition->coreness[u] > partition->coreness[v] && inQueue.find(u) == inQueue.end())
					{
						H.push({ partition->coreness[u] * 1.0 + partition->shells[u] * 1.0 / num_N, u });
						inQueue.insert({ u,1 });
					}
				}
			}
			continue;
		}
		for (auto u : partition->datagraph->AdjList[v])
		{
			if (partition->coreness[u] >= partition->coreness[v] + 1 || u==anchor)
			{
				degplusv++;
			}
			else if(partition->coreness[v] == partition->coreness[u] && u!=anchor)
			{
				//survived 
				if (Status.find(u) != Status.end())
				{
					if (Status[u] == 0)degplusv++;
				}
				else { // unexplored
					if (partition->shells[u] > partition->shells[v])degplusv++;
					else if (partition->shells[u] <= partition->shells[v] && inQueue.find(u) != inQueue.end())degplusv++;
				}
			}
		}
		if (degplusv >= partition->coreness[v] + 1)
		{
			Status.insert({ v,0 });
			degplus.insert({ v,degplusv });
			for (auto u : partition->datagraph->AdjList[v])//unexplored tca> =(u)
			{
				if (u == anchor) continue;
				if (Status.find(u) == Status.end())
				{
					if (partition->coreness[v] == partition->coreness[u] && partition->shells[v] < partition->shells[u] && inQueue.find(u) == inQueue.end())
					{
						H.push({ partition->coreness[u] * 1.0 + partition->shells[u] * 1.0 / num_N, u });
						inQueue.insert({ u,1 });
					}
				}
			}
		}
		else
		{
			Status.insert({ v,1 });
			degplus.insert({ v,degplusv });
			ShrinkVertex(v, degplus, Status);
		}
	}
	// get followers
	for (auto it : Status)
	{
		double u = it.first;
		if (u == anchor)continue;
		int status_node = it.second;
		if (status_node == 0 && degplus[u] >= partition->coreness[u] + 1)
		{
			followers_i.push_back(u);
		}
	}
}

void Master::ShrinkVertex(double v, unordered_map<double, int>& degplus, unordered_map<double, int>& Status)
{
	vector<double>Shrinknodes;
	for (auto u : partition->datagraph->AdjList[v])
	{
		if (Status.find(u) != Status.end() && Status[u] == 0)
		{
			if (partition->coreness[u] == partition->coreness[v])
			{
				degplus[u]--;
				if (degplus[u] < partition->coreness[u] + 1)
				{
					Status[u] = 1;
					Shrinknodes.push_back(u);
				}
			}
		}
	}
	for (auto u : Shrinknodes)
	{
		ShrinkVertex(u, degplus, Status);
	}
}

pair<double, int> Master::getAnchorScore(int b, double& node_score, int& node_followers, int& node_cost, vector<double>& final_followers)
{
	//ofstream out("output/Master.txt", ios::app);
	double anchor = INITIAL;
	int get_k = 0;
	int Cost = 0;
	int followers_num = 0;
	for (double i = 0; i < partition->datagraph->AdjList.size(); i++)
	{
		vector<double>followers_g;
		collectFollowers_vertex(i, followers_g);
		sort(followers_g.begin(), followers_g.end(), compareDegree);
		int posk = followers_g.size() - 1;
		int poskk = posk;
		// Traverse all possible k values of each vertex and find F(x,k)
		for (auto j : partition->nbrk[i])
		{
			int k = j.first;// now k
			// Add a simple pruning and add a simple constraint to the upper and lower bounds of nbr coreness
			if (k < partition->coreness[i] || k == partition->max_coreness)continue;
			k++;
			int cnt_followers = 0;
			int cost = k;  
			while (posk >= 0 && partition->coreness[followers_g[posk]] > k - 1)
			{
				posk--;
			}// posk represents the last position in the vertex follower where coreness is equal to core(i)
			poskk = posk;
			while (poskk >= 0 && partition->coreness[followers_g[poskk]] >= k - 1)
			{
				poskk--;
			}// posk represents the last position in the vertex follower where coreness is equal to core(i)-1

			// cnt_followers F(x,k) = F(x, infty) - F(x, infty, k)
			cnt_followers = posk + 1;
			// Subtract the intersection of N(x) and F(x,k,=k-1)
			for (int kk = poskk + 1; kk <= posk; kk++)
			{
				if (partition->datagraph->HashEdges.find({ i,followers_g[kk] }) != partition->datagraph->HashEdges.end())
				{
					cost--;
				}
			}
			// Subtract the intersection of N(x) and C_k(G_A)
			//cost -= followers[i].size() - posk - 1;
			for (auto u : partition->datagraph->AdjList[i])
			{
				if (partition->coreness[u] >= k)
				{
					cost--;
				}
			}

			cnt_followers += k - partition->coreness[i];//The final number of followers, that is, the increase in coreness, should be included in the peak of the current anchor.
			if (cost <= 0)cost = MAX_COST;
			double score = (1.0 * cnt_followers) / (1.0 * cost);
			if ((score > node_score || (score == node_score && cost > Cost)) && cost + acost <= b)
			{
				node_score = score;
				anchor = i;
				get_k = k;
				Cost = cost;
				followers_num = cnt_followers;
				final_followers.clear();
				final_followers.assign(followers_g.begin(), followers_g.begin() + posk + 1);
			}
			//cout << i << ' ' << k << ' ' << cnt_followers << ' ' << cost << ' ' << score << endl;
		}
	}
	/*out << "anchor: " << anchor << "; #followers = " << followers_num << endl;
	cout << "anchor: " << anchor << "; #followers = " << followers_num << endl;*/
	node_followers = followers_num;
	node_cost = Cost;
	return { anchor , get_k };
}

vector<double> Master::GroupSelection(int b, vector<pair<double, int>>& followers_final, int& group_costs, int& group_followers, double& max_score, vector<pair<double, double>>& group_insert_edges, unordered_map<double, pair<int, int>>& group_anchor_cost)
{
	vector<double>candidate_group;
	unordered_map<int, int>mp;

	for (int i = 0; i < partition->coreness.size(); i++)
	{
		int kk = partition->coreness[i];
		if (kk == partition->max_coreness)continue;
		int degt = partition->deggt[i];
		if (degt < kk + 1)continue;
		candidate_group.push_back(i);
	}


	vector<double>group_nodes;

	for (auto u : candidate_group)
	{
		vector<double>group;
		unordered_map<double, int>vis_group;
		group.push_back(u);
		vis_group.insert({ u,1 });
		int k = partition->coreness[u];

		for (auto v : partition->datagraph->AdjList[u])
		{
			if (partition->coreness[v] == partition->coreness[u])
			{
				group.push_back(v);
				vis_group.insert({ v,1 });
			}
		}
		unordered_map<double, int>r;
		unordered_map<double, int>budget;
		// coumpute r and budget
		for (auto v : group)
		{
			r.insert({ v,k + 1 });
			budget.insert({ v,0 });
			for (auto w : partition->datagraph->AdjList[v])
			{
				if (partition->coreness[w] > k || vis_group.find(w) != vis_group.end())
				{
					r[v]--;
					if (vis_group.find(w) != vis_group.end())
					{
						budget[v]++;
					}
				}
			}
		}
		// group reduction
		for (auto v : group)
		{
			if (vis_group[v] && budget[v] < r[v])
			{
				vis_group[v] = 0;
				r.erase(v);
				budget.erase(v);
				GroupShrink(v, r, budget, vis_group);
			}
		}
		for (auto v : group)
		{
			if (r.find(v) == r.end())continue;
			if (r[v] < 0)r[v] = 0;
		}

		int p = 0, q = 0;
		while (p < group.size())
		{
			if (vis_group[group[p]])
			{
				group[q] = group[p];
				q++;
			}
			else vis_group.erase(group[p]);
			p++;
		}
		group.resize(q);
		
		
		vector<double>new_group;

		unordered_map<double, int>visited_group;
		unordered_map<double, int>r_;
		unordered_map<double, int>b_;
		for (auto v : group)
		{
			for (auto w : partition->datagraph->AdjList[v])
			{
				if (partition->coreness[w] > partition->coreness[v] || vis_group.find(w) != vis_group.end())continue;

				int budget_ = 0;
				int cost_ = k + 1;
				if (visited_group.find(w) == visited_group.end())visited_group.insert({ w,1 });
				else continue;
				vector<double>update_r;
				for (auto z : partition->datagraph->AdjList[w])
				{
					if (partition->coreness[z] > k)cost_--;
					if (vis_group.find(z) != vis_group.end())
					{
						if (r[z] > 0)
						{
							budget_++;
							update_r.push_back(z);
						}
						cost_--;
					}
				}
				if (cost_ < 0)cost_ = 0;
				r_.insert({ w,cost_ });
				b_.insert({ w, budget_ });
				if (budget_ >= cost_) 
				{
					new_group.push_back(w);
					vis_group.insert({ w,1 });
					for (auto ttt : update_r)
					{
						if (r[ttt] > 0)
						{
							r[ttt]--;
							if (r[ttt] == 0)
							{
								for (auto ttnbr : partition->datagraph->AdjList[ttt])
								{
									if (vis_group.find(ttnbr)==vis_group.end() && visited_group.find(ttnbr) != visited_group.end())
									{
										b_[ttnbr]--;
									}
								}
							}
						}
					}
					r.insert({ w, cost_ });
					budget.insert({ w,budget_ });
					GroupExpand(w, r_, b_, visited_group, r, budget, vis_group, new_group);
				}
			}
		}
		for (auto t : new_group)
		{
			group.push_back(t);
		}
		unordered_map<double, int>r2;
		for (auto v : group)
		{
			r2.insert({ v,k + 1 });
			for (auto w : partition->datagraph->AdjList[v])
			{
				if (partition->coreness[w] > k || vis_group.find(w) != vis_group.end())
				{
					r2[v]--;
				}
			}
		}
		for (auto v : group)
		{
			if (r2[v] < 0)r2[v] = 0;
		}
		for (auto v : group)
		{
			if (r[v] != r2[v])
			{
				cout << v << ' ' << r[v] << ' ' << r2[v] << endl;
			}
		}

		//compute cost of group
		int group_cost = 0;
		unordered_map<double, pair<int, int>> group_anchor_cost_tem;
		vector<pair<double, double>>group_insert_edges_tem;

		vector<pair<double, int>>followers_gain;

		int coreness_gain = getGroupFollowers(group, followers_gain, vis_group, k + 1);
		for (int i = 0; i < group.size(); i++)
		{
			double w = group[i];
			if (r[w] == 0)continue;
			for (int j = i + 1; j < group.size(); j++)
			{
				double v = group[j];
				if (r[w] == 0)break;
				if (r[v] == 0)continue;
				if (partition->datagraph->HashEdges.find({ w,v }) == partition->datagraph->HashEdges.end())
				{
					group_cost++;
					group_insert_edges_tem.push_back({ w,v });
					r[w]--;
					r[v]--;
				}
			}
			if (r[w] > 0)
			{
				group_cost += r[w];
				group_anchor_cost_tem.insert({ w,{r[w], k+1}});
			}
		}

		if (group_cost > b - acost)continue;
		//compute followers of group
		if (group_cost == 0)group_cost = MAX_COST;
		double score = coreness_gain * 1.0 / (group_cost * 1.0);

		if (score > max_score || (score==max_score && group_cost > group_costs))
		{
			group_nodes.swap(group);
			//cout << group_nodes.size() << endl;
			max_score = score;
			group_costs = group_cost;
			group_followers = coreness_gain;
			followers_final.swap(followers_gain);
			group_anchor_cost = group_anchor_cost_tem;
			group_insert_edges.swap(group_insert_edges_tem);
		}
	}
	//cout << group_nodes.size() << endl;
	return group_nodes;
}


void Master::GroupExpand(double w, unordered_map<double, int>& r_, unordered_map<double, int>& budget_, unordered_map<double, int>& visited_group, unordered_map<double, int>& r, unordered_map<double, int>& budget, unordered_map<double, int>& vis_group, vector<double>& new_group)
{
	for (auto u : partition->datagraph->AdjList[w])
	{
		if (visited_group.find(u) != visited_group.end())
		{
			if (r_[u] > 0)r_[u]--;
		}
	}

	for (auto u : partition->datagraph->AdjList[w])
	{
		if (r[w] == 0)
		{
			break;
		}
		if (visited_group.find(u) != visited_group.end())
		{
			budget_[u]++; // a little change
			if (budget_[u] >= r_[u] && vis_group.find(u)==vis_group.end())
			{
				new_group.push_back(u);
				vis_group.insert({ u,1 });
				for (auto v : partition->datagraph->AdjList[u])
				{
					if (vis_group.find(v) != vis_group.end())
					{
						if (r[v] > 0)
						{
							r[v]--;
							if (r[v] == 0)
							{
								for (auto q : partition->datagraph->AdjList[v])
								{
									if (vis_group.find(q) == vis_group.end() && budget_.find(q) != budget_.end())
									{
										budget_[q]--;
									}
								}
							}
						}
					}
				}
				r.insert({ u, r_[u] });
				budget.insert({ u,budget_[u] });
				GroupExpand(u, r_, budget_, visited_group, r, budget, vis_group, new_group);
			}
		}
	}
}

void Master::GroupShrink(double v, unordered_map<double, int>&r, unordered_map<double, int>&budget, unordered_map<double, int>&vis_group)
{
	for (auto w : partition->datagraph->AdjList[v])
	{
		if (vis_group.find(w) != vis_group.end() && vis_group[w] != 0)
		{
			r[w]++;
			budget[w]--;
			if (budget[w] < r[w])
			{
				vis_group[w] = 0;
				r.erase(w);
				budget.erase(w);
				GroupShrink(w, r, budget, vis_group);
			}
		}
	}
}



int Master::getGroupFollowers(vector<double>group, vector<pair<double, int>>& followers_gain, unordered_map<double, int>vis_group, int group_k)
{
	priority_queue<pair<double, double>, vector<pair<double, double>>, greater<pair<double, double>> > q; // 1 为优先级 core+ 0.1 layer 2为顶点
	unordered_map<double, int>degplus;
	unordered_map<double, int>inQueue;
	double num_N = partition->max_layer + 100.0;
	for (auto v : group)
	{
		q.push({ partition->coreness[v] * 1.0 + partition->shells[v] * 1.0 / num_N, v });
		inQueue.insert({ v,1 });
	}
	unordered_map<double, int>Status; // 1 survive 2 discarded
	unordered_map<double, int>Survive; // coreness increase
	int beforek = 0;
	unordered_map<double, double>mp_vis;
	while (q.size())
	{
		double v = q.top().second;
		int corenessv = partition->coreness[v];
		if (Status.find(v) != Status.end())corenessv += Survive[v];
		// hierarchy expansion
		if (corenessv > beforek)
		{
			for (auto it : mp_vis)
			{
				q.push({it.second,it.first});
				inQueue.insert({ it.first, 1 });
			}
			mp_vis.clear();
			v = q.top().second;
			beforek = corenessv;
		}
		q.pop();
		inQueue.erase(v);
		int degplusv = 0;
		// initial expansion
		if (vis_group.find(v) != vis_group.end() && Status.find(v) == Status.end())
		{
			Status.insert({ v,1 });
			Survive.insert({ v, group_k - partition->coreness[v] });
			degplus.insert({ v, partition->coreness[v] + 1 });
			inQueue.insert({ v,1 });
			for (auto u : partition->datagraph->AdjList[v])
			{
				if (vis_group.find(u) != vis_group.end())continue;
				if (Status.find(u) == Status.end() && inQueue.find(u) == inQueue.end())
				{
					if (partition->coreness[v] == partition->coreness[u] && partition->shells[v] < partition->shells[u])
					{
						q.push({ partition->coreness[u] * 1.0 + partition->shells[u] * 1.0 / num_N, u });
						inQueue.insert({ u,1 });
					}
					else if (partition->coreness[v] < partition->coreness[u] && partition->coreness[u] < group_k)
					{
						q.push({ partition->coreness[u] * 1.0 + partition->shells[u] * 1.0 / num_N, u });
						inQueue.insert({ u,1 });
					}
				}
			}
			continue;
		}
		for (auto u : partition->datagraph->AdjList[v])
		{
			/*if (vis_group.find(u) != vis_group.end())continue;*/
			// d>(u)
			int survivev = 0;
			if (Survive.find(v) != Survive.end())survivev = Survive[v];
			// d+
			if (partition->coreness[u] >= (partition->coreness[v] + survivev + 1) || vis_group.find(u) != vis_group.end())
			{
				degplusv++;
			}
			else if (Status.find(u) == Status.end()) //unexplored
			{
				if (partition->coreness[u] == (partition->coreness[v] + survivev))
				{
					if (survivev>0)
					{
						degplusv++;
					}
					else if (partition->shells[v] < partition->shells[u] || inQueue.find(u) != inQueue.end())
					{
						degplusv++;
					}
				}
			}
			else if (Status.find(u) != Status.end() && Status[u] == 1)
			{
				int surviveu = 0;
				if (Survive.find(u) != Survive.end())surviveu = Survive[u];
				if ((partition->coreness[u] + surviveu) > (partition->coreness[v] + survivev) || ((partition->coreness[u] + surviveu) == (partition->coreness[v] + survivev) && inQueue.find(u)!=inQueue.end()))
				{
					degplusv++;
				}
			}
		}
		int survivev = 0;
		if (Survive.find(v) != Survive.end())survivev = Survive[v];
		if (degplusv >= partition->coreness[v] + survivev + 1)
		{
			for (auto u : partition->datagraph->AdjList[v])
			{
				if (vis_group.find(u) != vis_group.end())continue;
				if (Status.find(v) == Status.end() && inQueue.find(u) == inQueue.end())
				{
					if (Status.find(u) == Status.end() && partition->coreness[v] == partition->coreness[u] && partition->shells[v] < partition->shells[u])
					{
						q.push({ partition->coreness[u] * 1.0 + partition->shells[u] * 1.0 / num_N ,u });
						inQueue.insert({ u,1 });
					}
				}
				else if (Status.find(v) != Status.end() && Status[v] == 1 && inQueue.find(u) == inQueue.end())
				{
					if (Status.find(u) == Status.end() && (partition->coreness[v] + survivev) == partition->coreness[u])
					{
						q.push({ partition->coreness[u] * 1.0 + partition->shells[u] * 1.0 / num_N ,u });
						inQueue.insert({ u,1 });
					}
				}
			}
			if (Status.find(v) == Status.end())Status.insert({ v,1 });
			else Status[v] = 1;
			if (Survive.find(v) == Survive.end())Survive.insert({ v,1 });
			else Survive[v]++;
			if (degplus.find(v) == degplus.end())degplus.insert({ v,degplusv });
			else degplus[v] = degplusv;
			mp_vis.insert({ v ,(partition->coreness[v] + Survive[v]) * 1.0 });
			/*q.push({ (partition->coreness[v] + Survive[v]) * 1.0,v });
			inQueue.insert({ v,1 });*/
		}
		else
		{
			if (Status.find(v) == Status.end())Status.insert({ v,2 });
			else Status[v] = 2;
			if (degplus.find(v) == degplus.end())degplus.insert({ v,degplusv });
			else degplus[v] = degplusv;
			ShrinkGroups(v, degplus, Status, vis_group, Survive, mp_vis);
		}
	}
	int gains = 0;
	for (auto it : Survive)
	{
		double u = it.first;
		int status_node = it.second;
		if (vis_group.find(u) != vis_group.end())
		{
			followers_gain.push_back({ u, group_k - partition->coreness[u] });
			gains += group_k - partition->coreness[u];
		}
		else
		{
			followers_gain.push_back({ u, Survive[u] });
			gains += Survive[u];
		}
	}
	return gains;
}


void Master::ShrinkGroups(double v, unordered_map<double, int>& degplus, unordered_map<double, int>& Status, unordered_map<double, int>vis_group, unordered_map<double, int>& Survive, unordered_map<double,double>& mp_vis)
{
	vector<double>Shrinknodes;
	for (auto u : partition->datagraph->AdjList[v])
	{
		if (Status.find(u) != Status.end() && Status[u] == 1 && vis_group.find(u) == vis_group.end())
		{
			int surviveu = 0;
			int survivev = 0;
			if (Survive.find(u) != Survive.end())surviveu = Survive[u];
			if (Survive.find(v) != Survive.end())survivev = Survive[v];
			if ((partition->coreness[u] + surviveu) == (partition->coreness[v] + survivev + 1))
			{
				degplus[u]--;
				if (degplus[u] < (partition->coreness[u] + surviveu))
				{
					Status[u] = 2;
					Survive[u]--;
					if (Survive[u] == 0)Survive.erase(u);
					if (mp_vis.find(u) != mp_vis.end())
					{
						mp_vis.erase(u);
					}
					Shrinknodes.push_back(u);
				}
			}
			
		}
	}
	for (auto u : Shrinknodes)
	{
		ShrinkGroups(u, degplus, Status, vis_group, Survive, mp_vis);
	}
}

void Master::Final_insert(string chec_dat)
{
	std::ofstream outputFile(chec_dat);
	if (!outputFile.is_open()) {
		std::cerr << "Error opening file!" << std::endl;
	}

	for (auto element : anchor_edges) {
		outputFile << std::fixed << std::setprecision(0) << partition->datagraph->seq2id[element.first] << '\t' << partition->datagraph->seq2id[element.second] << endl;
		//cout << partition->datagraph->seq2id[element.first] << ' ' << partition->datagraph->seq2id[element.second] << endl;
	}
	// close file
	outputFile.close();

	std::cout << "Data written to insert_edges.txt successfully." << std::endl;


}