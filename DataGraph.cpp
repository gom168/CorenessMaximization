#include "DataGraph.h"



DataGraph::DataGraph(string fileName)
{
	vector<vector<double> >().swap(AdjList);
	unordered_map<double, double>().swap(id2seq);
	unordered_map<double, double>().swap(seq2id);
	ifstream in(fileName.c_str());

	if (!in)
	{
		cout << "Fail to read " << fileName << "." << endl;
		return;
	}
	string line;
	getline(in,line); // read n and m no use
	while (getline(in, line))
	{
		std::istringstream iss(line);
		double src,dst;
		iss>>src>>dst;

		if (src != dst)
		{
			unordered_map<double, double>::iterator it;
			int seq1, seq2;
			
			it = id2seq.find(src);
			if (it == id2seq.end())
			{
				seq1 = AdjList.size();
				id2seq[src] = seq1;
				seq2id[seq1] = src;
				vector<double> newVertex;
				AdjList.push_back(newVertex);
			}
			else
			{
				seq1 = it->second;
			}

			it = id2seq.find(dst);
			if (it == id2seq.end())
			{
				seq2 = AdjList.size();
				id2seq[dst] = seq2;
				seq2id[seq2] = dst;
				vector<double> newVertex;
				AdjList.push_back(newVertex);
			}
			else
			{
				seq2 = it->second;
			}
			HashEdges.insert({ {id2seq[src],id2seq[dst]},1 });
			HashEdges.insert({ {id2seq[dst],id2seq[src]},1 });
			int flag;

			flag = 0;
			for (double i = 0; i < AdjList[seq1].size(); i++)
			{
				if (AdjList[seq1][i] == seq2)
				{
					flag = 1;
					break;
				}
			}
			if (flag == 0) AdjList[seq1].push_back(seq2);

			flag = 0;
			for (double i = 0; i < AdjList[seq2].size(); i++)
			{
				if (AdjList[seq2][i] == seq1)
				{
					flag = 1;
					break;
				}
			}
			if (flag == 0) AdjList[seq2].push_back(seq1);
		}
	}
}


void DataGraph::add_edges(string fileName2)
{
	ifstream in(fileName2.c_str());

	if (!in)
	{
		cout << "Fail to read " << fileName2 << "." << endl;
		return;
	}

	string line;
	while (getline(in, line))
	{
		string src_s = line.substr(0, line.find("\t"));
		string dst_s = line.substr(line.find("\t") + 1, line.find("\n") - line.find("\t") - 1);

		double src = stof(src_s);
		double dst = stof(dst_s);
		if (src != dst)
		{
			unordered_map<double, double>::iterator it;
			int seq1, seq2;

			it = id2seq.find(src);
			if (it == id2seq.end())
			{
				seq1 = AdjList.size();
				id2seq[src] = seq1;
				seq2id[seq1] = src;
				vector<double> newVertex;
				AdjList.push_back(newVertex);
			}
			else
			{
				seq1 = it->second;
			}

			it = id2seq.find(dst);
			if (it == id2seq.end())
			{
				seq2 = AdjList.size();
				id2seq[dst] = seq2;
				seq2id[seq2] = dst;
				vector<double> newVertex;
				AdjList.push_back(newVertex);
			}
			else
			{
				seq2 = it->second;
			}
			HashEdges.insert({ {id2seq[src],id2seq[dst]},1 });
			HashEdges.insert({ {id2seq[dst],id2seq[src]},1 });
			int flag;

			flag = 0;
			for (double i = 0; i < AdjList[seq1].size(); i++)
			{
				if (AdjList[seq1][i] == seq2)
				{
					flag = 1;
					break;
				}
			}
			if (flag == 0) AdjList[seq1].push_back(seq2);

			flag = 0;
			for (double i = 0; i < AdjList[seq2].size(); i++)
			{
				if (AdjList[seq2][i] == seq1)
				{
					flag = 1;
					break;
				}
			}
			if (flag == 0) AdjList[seq2].push_back(seq1);
		}
	}
}