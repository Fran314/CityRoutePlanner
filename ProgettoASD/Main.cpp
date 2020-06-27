#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include"ListaOrdinata.h"

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

vector<arco_pt>* grafo_pt;
vector<arco_walk>* grafo_walk;

void inizializzaGrafi()
{
	ifstream ntd;
	int N = 0;
	ntd.open("network_nodes.csv");

	cout << "Calcolo di N... ";
	string last_line, curr_line;
	while (ntd.good())
	{
		last_line = curr_line;
		getline(ntd, curr_line);
		if (!ntd.good())
		{
			last_line;
			N = atoi(last_line.substr(0, last_line.find(";")).c_str()) + 1;
		}
	}
	ntd.close();
	cout << N << endl;

	grafo_pt = new vector<arco_pt>[N];
	grafo_walk = new vector<arco_walk>[N];

	cout << "Lettura di network_temporal_day (potrebbe richiedere molto tempo)... ";
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
	ntd.close();
	cout << "Fatto." << endl;

	cout << "Lettura di network_walk (potrebbe richiedere molto tempo)... ";
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
	ntd.close();
	cout << "Fatto." << endl;
}

int main()
{
	inizializzaGrafi();

	return 0;
}
