#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include"ListaOrdinata.h"

using namespace std;

// Struct contenente tutte le informazioni che ci interessano contenute
//	in un arco di una tratta di un generico trasporto pubblico
struct arco_pt
{
	int to_stop;	// to_stop corrisponde a to_stop_I del file
	int dep_time;
	int arr_time;
	int route_type;
};

// Struct contenente tutte le informazioni che ci interessano contenute
//	in un arco di una camminata
struct arco_walk
{
	int to_stop;	// to_stop corrisponde a to_stop_I del file
	int d_walk;
};


int N;	// Valore massimo (+1) degli indici dei nodi.
		// N.B: questo NON equivale al numero massimo di nodi. Il
		//		numero di nodi è (nel caso di Parigi) MOLTO minore
		//		rispetto al valore massimo degli indici (11951 vs
		//		27559). Infatti, molti indici disponibili non vengono
		//		utilizzati (ad esempio per Parigi non esiste nessun
		//		nodo di indice 2). Ciò causa un leggero spreco di
		//		memoria poiché la lista di adiacenza contiene spazi di
		//		memoria relativi agli indici non assegnati ad alcun
		//		nodo. Tuttavia, stiamo parlando di uno spreco di
		//		qualche decina di KB per grafo, quindi trascurabile.
vector<arco_pt>* grafo_pt;	// Grafo contenente solo gli archi dei
							//	trasporti pubblici (pt)
vector<arco_walk>* grafo_walk;	// Grafo contenente solo gli archi
								//	delle camminate (walk)
// Assieme, i due grafi rappresentano interamente la città e le
//	modalità di trasporto come grafo. Sono separati in due grafi poiché
//	gli archi dei trasporti pubblici e gli archi delle camminate
//	verranno trattati in modi diversi, poiché gli archi dei trasporti
//	pubblici richiedono un dep_time non esistente per le camminate

// Inizializza i grafi
void inizializzaGrafi()
{
	ifstream file_corrente;

	cout << "Calcolo di N... ";
	file_corrente.open("network_nodes.csv");
	string riga_precedente, riga_corrente;
	// E' necessario salvare il valore della riga precedente poiché
	//	l'ultima del file network_nodes è una riga vuota, mentre il
	//	valore dell'indice dell'ultimo nodo (quindi il valore massimo
	//	degli indici, poiché elencati in ordine crescente) è scritto
	//	sulla penultima riga
	while (file_corrente.eof())
	{
		riga_precedente = riga_corrente;
		getline(file_corrente, riga_corrente);
	}
	file_corrente.close();
	// Quando esce dal while, riga_corrente contiene il valore
	//	dell'ultima riga, mentre riga_precedente contiene il valore
	//	della penultima riga
	N = atoi(riga_precedente.substr(0, riga_precedente.find(";")).c_str()) + 1;
	cout << N << endl;

	grafo_pt = new vector<arco_pt>[N];
	grafo_walk = new vector<arco_walk>[N];

	cout << "Lettura di network_temporal_day (potrebbe richiedere molto tempo)... ";
	file_corrente.open("network_temporal_day.csv");
	while (file_corrente.eof())
	{
		string sottoriga;	// Stringa contenente la sottoriga
							//	corrispondente al i-esimo campo della
							//	riga corrente
		arco_pt nuovo_arco;

		getline(file_corrente, sottoriga, ';');
		int indice = atoi(sottoriga.c_str());

		getline(file_corrente, sottoriga, ';');
		int to_stop = atoi(sottoriga.c_str());
		nuovo_arco.to_stop = to_stop;

		getline(file_corrente, sottoriga, ';');
		int dep_time = atoi(sottoriga.c_str());
		nuovo_arco.dep_time = dep_time;

		getline(file_corrente, sottoriga, ';');
		int arr_time = atoi(sottoriga.c_str());
		nuovo_arco.arr_time = arr_time;

		getline(file_corrente, sottoriga, ';');
		int route_type = atoi(sottoriga.c_str());
		nuovo_arco.route_type = route_type;

		// Ignora il resto della riga
		getline(file_corrente, sottoriga);

		grafo_pt[indice].push_back(nuovo_arco);
	}
	file_corrente.close();
	cout << "Fatto." << endl;

	cout << "Lettura di network_walk (potrebbe richiedere molto tempo)... ";
	file_corrente.open("network_walk.csv");
	while (file_corrente.eof())
	{
		string sottoriga;	// Stringa contenente la sottoriga
							//	corrispondente al i-esimo campo della
							//	riga corrente
		arco_walk nuovo_arco;

		getline(file_corrente, sottoriga, ';');
		int indice = atoi(sottoriga.c_str());

		getline(file_corrente, sottoriga, ';');
		int to_stop = atoi(sottoriga.c_str());
		nuovo_arco.to_stop = to_stop;

		// Ignora il terzo dato
		getline(file_corrente, sottoriga, ';');

		getline(file_corrente, sottoriga);
		int d_walk = atoi(sottoriga.c_str());
		nuovo_arco.d_walk = d_walk;

		grafo_walk[indice].push_back(nuovo_arco);
	}
	file_corrente.close();
	cout << "Fatto." << endl;
}

int main()
{
	inizializzaGrafi();

	return 0;
}
