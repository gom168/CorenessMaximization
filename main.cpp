#include <iostream> 
#include "DataGraph.h"
#include "Partition.h"
#include "Master.h"

using namespace std;

// args: data.txt b insert.txt check pre mode
/*
*  data.txt 
*  n 	m in the first line
*  Read the graph. Just input the edges directly in the graph format. The edges will be renumbered, so you don’t need to consider the actual number of the edges.
*  b  anchor at most b edges
*  insert.txt 
*  a \t  b in each line
*  The txt format used to check the actual inserted edge is the same as data.txt
*  check The value is 0 or 1, call the algorithm to perform anchor edges when 0. When 1, verify whether the increase in coreness value is correct according to insert.txt.
*  pre   The value is 0 or 1. 0 means there is no coarse-grained filtering. 1 means there is coarse-grained filtering.
*  mode The value is 0 1 2. When 0, there is only VertexSelection. When 1, there is only GroupSelection. When 2, there is VertexSelection+GroupSelection.
*/

int main(int argc, char* argv[])
{
	//cout << "** Master node is setting up PGAC..." << endl;
	cout << "dataset: ";
	string dataset = argv[1];
	DataGraph datagraph(dataset);
	Partition partition(&datagraph);
	string chec_dat = argv[3];
	int check = atoi(argv[4]);
	int mode = atoi(argv[5]);
	if (check==1)
	{
		partition.coreDecomposition();
		partition.datagraph->add_edges(chec_dat);
		partition.coreDecomposition();
	}
	else if(check==0)
	{
		Master master(&partition);
		cout << "Loaded dataset successfully!" << endl;
		int b;
		b = atoi(argv[2]);
		master.Anchoring(b, mode, chec_dat);
		cout << "Number of Followers:" << master.nfs << endl;
	}
	else if(check==2)
	{
		partition.coreDecomposition();
	}
	
	return 0;
}
