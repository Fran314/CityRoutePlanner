#include<iostream>
#include<fstream>
#include<string>
#include<vector>

#define N 27560

using namespace std;

struct arco_pt
{
	int to_stop;
	int dep_time;
	int arr_time;
	int route_type;
};

struct arco_walk
{
	int to_stop;
	int d_walk;
};

vector<arco_pt> grafo_pt[N];
vector<arco_walk> grafo_walk[N];


int main()
{
	ifstream ntd;
	ntd.open("network_temporal_day.csv");
	while (ntd.good())
	{
		string subline;

		getline(ntd, subline, ';');
		int indice = atoi(subline.c_str());
		arco_pt nuovo_arco;

		getline(ntd, subline, ';');
		int to_stop = atoi(subline.c_str());
		nuovo_arco.to_stop = to_stop;

		getline(ntd, subline, ';');
		int dep_time = atoi(subline.c_str());
		nuovo_arco.dep_time = dep_time;

		getline(ntd, subline, ';');
		int arr_time = atoi(subline.c_str());
		nuovo_arco.arr_time = arr_time;

		getline(ntd, subline, ';');
		int route_type = atoi(subline.c_str());
		nuovo_arco.route_type = route_type;

		getline(ntd, subline);

		grafo_pt[indice].push_back(nuovo_arco);
	}

	ntd.open("network_walk.csv");
	while (ntd.good())
	{
		string subline;

		getline(ntd, subline, ';');
		int indice = atoi(subline.c_str());
		arco_walk nuovo_arco;

		getline(ntd, subline, ';');
		int to_stop = atoi(subline.c_str());
		nuovo_arco.to_stop = to_stop;

		getline(ntd, subline, ';');

		getline(ntd, subline);
		int d_walk = atoi(subline.c_str());
		nuovo_arco.d_walk = d_walk;

		grafo_walk[indice].push_back(nuovo_arco);
	}
	cout << grafo_walk[3].front().to_stop << ", " << grafo_walk[3].front().d_walk << endl;

	return 0;
}