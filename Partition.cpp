#include "Partition.h"

bool compareDegree(const double& a, const double& b)
{
	return Partition::coreness[a] < Partition::coreness[b];
}

vector<int> Partition::coreness;

Partition::Partition(DataGraph* datagraph)
{
	this->datagraph = datagraph;
	max_degree = 0;
	for (int i = 0; i < datagraph->AdjList.size(); i++)
	{
		if (datagraph->AdjList[i].size() > max_degree)
			max_degree = datagraph->AdjList[i].size();
		nbrk.push_back(unordered_map<int, int>());
		deggt.push_back(0);
		shells.push_back(0);
		coreness_ins.push_back(0);
	}
}

void Partition::coreDecomposition()
{
	coreness.clear(); //d
	shell_tag.clear(); //b
	total_order.clear(); //D
	order_pointer.clear(); //p
	int max_degs = 0;

	for (double i = 0; i < datagraph->AdjList.size(); i++)
	{
		total_order.push_back(i);
		int degree = datagraph->AdjList[i].size();
		max_degs = max(max_degs,degree);
		coreness.push_back(degree);
		order_pointer.push_back(INITIAL);
	}
	cout<<"max_degree:"<<max_degs<<endl;

	sort(total_order.begin(), total_order.end(), compareDegree);
	int bi = 0;
	for (double i = 0; i < total_order.size(); i++)
	{
		double id = total_order[i];
		int degree = coreness[id];
		order_pointer[id] = i;

		while (degree >= bi)
		{
			shell_tag.push_back(i);
			bi++;
		}
	}

	for (double i = 0; i < total_order.size(); i++)
	{
		double v = total_order[i];

		for (int j = 0; j < datagraph->AdjList[v].size(); j++)
		{
			double u = datagraph->AdjList[v][j];
			if (coreness[u] > coreness[v])
			{
				int du = coreness[u]; double pu = order_pointer[u];
				double pw = shell_tag[du]; double w = total_order[pw];

				if (u != w)
				{
					total_order[pu] = w; total_order[pw] = u;
					order_pointer[u] = pw; order_pointer[w] = pu;
				}

				shell_tag[du]++; coreness[u]--;
			}
		}
	}
	long long sum_coreness = 0;
	for (double i = 0; i < datagraph->AdjList.size(); i++)
	{
		max_coreness = max(max_coreness, coreness[i]);
		sum_coreness += (long long)coreness[i];
	}
	cout << "all coreness:" << sum_coreness << endl;
	if (pre_coreness == -1)
	{
		pre_coreness = sum_coreness;
	}
	else
	{
		after_coreness = sum_coreness;
		cout << "coreness gain:" << after_coreness-pre_coreness << endl;
	}

}

void Partition::layerDecomposition()
{
	shells.clear();
	for (int i = 0; i < coreness.size(); i++)
	{
		shells.push_back(0);
	}
	int k = -1;
	vector<pair<int,double>>coreness_copy;
	for (int i = 0; i < coreness.size(); i++)
	{
		coreness_copy.push_back({ coreness[i],i });
	}
	sort(coreness_copy.begin(), coreness_copy.end());
	k = coreness_copy[0].first;
	int max_k = coreness_copy[coreness_copy.size() - 1].first;
	int pos = 0;
	while (pos < coreness_copy.size())
	{
		vector<double>shell_tag_;
		unordered_map<double, int>shell_deg;
		unordered_map<double, int>in_shellk;
		unordered_map<double, vector<double>>vert_set_;

		while (pos < coreness_copy.size() && coreness_copy[pos].first == k)
		{
			shell_tag_.push_back(coreness_copy[pos].second);
			shell_deg.insert({ coreness_copy[pos].second ,0});
			in_shellk.insert({ coreness_copy[pos].second , k });
			vert_set_.insert({ coreness_copy[pos].second , vector<double>() });
			pos++;
		}
		for (int i = 0; i < shell_tag_.size();i++)
		{
			double u = shell_tag_[i];
			for (int j = 0; j < datagraph->AdjList[u].size(); j++)
			{
				double v = datagraph->AdjList[u][j];
				if (coreness[v]>=k)
				{
					shell_deg[u]++;
					if (in_shellk.find(v) != in_shellk.end())
					{
						vert_set_[u].push_back(v);
					}
				}
			}
		}
		int cnt = 1;
		queue<double>to_delete;
		for (int i = 0; i < shell_tag_.size(); i++)
		{
			double u = shell_tag_[i];
			if (shell_deg[u] <= k)
			{
				to_delete.push(u);
				shells[u] = cnt;
				max_layer = max(max_layer, shells[u]);
			}
		}
		do
		{
			queue<double>tmp_que;
			++cnt;
			while (!to_delete.empty())
			{
				double u = to_delete.front();
				to_delete.pop();
				for (int i = 0; i < vert_set_[u].size(); i++)
				{
					double v = vert_set_[u][i];
					if (shells[v] == 0)
					{
						--shell_deg[v];
						if (shell_deg[v] <= k)
						{
							tmp_que.push(v);
							shells[v] = cnt;
							max_layer = max(max_layer, shells[v]);
						}
					}
				}
			}
			to_delete.swap(tmp_que);
		} while (!to_delete.empty());
		if(pos < coreness_copy.size())k = coreness_copy[pos].first;
	}

}

void Partition::P_Decomposition()
{
	coreDecomposition();
	layerDecomposition();

	// compute deg>= and Knbr
	for (double i = 0; i < total_order.size(); i++)
	{
		double u = total_order[i];
		for (double j = 0; j < datagraph->AdjList[u].size(); j++)
		{
			double v = datagraph->AdjList[u][j];
			if (coreness[v] >= coreness[u])
			{
				deggt[u]++;
			}
			int kv = coreness[v];
			if (nbrk[u].find(kv) == nbrk[u].end())
			{
				nbrk[u].insert({ coreness[v],1 });
			}
			else nbrk[u][coreness[v]]++;
		}
	}
}

void Partition::P_Maintenance(double anchor, int k, vector<double>followers)
{
	anchor_verts.insert(anchor);
	int before_anchor = coreness[anchor];
	coreness_ins[before_anchor]++;
	coreness[anchor] = k;
	max_coreness = max(max_coreness, k);
	for (double i =0; i < datagraph->AdjList[anchor].size(); i++)
	{
		double v = datagraph->AdjList[anchor][i];
		if (coreness[v] > before_anchor && coreness[v] <= k)
		{
			deggt[v]++;
			if(coreness[v]!=k)deggt[anchor]--;
		}
		if (coreness[v] == before_anchor)deggt[anchor]--;

		nbrk[v][before_anchor]--;
		if (nbrk[v][before_anchor] == 0)nbrk[v].erase(before_anchor);

		if (nbrk[v].find(k) == nbrk[v].end())nbrk[v].insert({ k,1 });
		else nbrk[v][k]++;
	}
	//coreness[anchor] = max_degree;
	for (int i = 0; i < followers.size(); i++)
	{
		double u = followers[i];
		coreness_ins[coreness[u]]++;
		coreness[u]++;
		for (double j = 0; j < datagraph->AdjList[u].size(); j++)
		{
			double v = datagraph->AdjList[u][j];
			if (coreness[v] == coreness[u])deggt[v]++;
			if (coreness[v] == coreness[u] - 1)deggt[u]--;
			nbrk[v][coreness[u] - 1]--;
			if (nbrk[v][coreness[u] - 1] == 0)nbrk[v].erase(coreness[u] - 1);

			if (nbrk[v].find(coreness[u]) == nbrk[v].end())nbrk[v].insert({ coreness[u],1 });
			else nbrk[v][coreness[u]]++;
		}
	}
	layerDecomposition();
}

void Partition::GroupMaintenance(vector<double>group, vector<pair<double, int>>& followers_final)
{
	for (auto u : group)
	{
		anchor_verts.insert(u);
	}
	for (auto p : followers_final)
	{
		int before_anchor = coreness[p.first];
		coreness_ins[before_anchor]++;
		coreness[p.first] += p.second;
		max_coreness = max(max_coreness, coreness[p.first]);
		double u = p.first;
		for (double j = 0; j < datagraph->AdjList[u].size(); j++)
		{
			double v = datagraph->AdjList[u][j];
			if (coreness[v] > before_anchor && coreness[v] <= coreness[u])
			{
				deggt[v]++;
				if (coreness[v] != coreness[u])deggt[u]--;
			}
			if (coreness[v] == before_anchor)deggt[u]--;
			nbrk[v][before_anchor]--;
			if (nbrk[v][before_anchor] == 0)nbrk[v].erase(before_anchor);

			if (nbrk[v].find(coreness[u]) == nbrk[v].end())nbrk[v].insert({ coreness[u],1 });
			else nbrk[v][coreness[u]]++;
		}
	}
	layerDecomposition();
}
