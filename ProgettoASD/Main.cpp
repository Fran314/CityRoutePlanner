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

// Struct contenente le etichette di un nodo utilizzate nell'algoritmo
//	di pathfinding, simile alle etichette f e J citate nella pagina
//	Wikipedia per l'algoritmo di Dijkstra
struct etichetta_nodo
{
	bool visitato = false;
	int arr_time = INT_MAX;	// Ora di arrivo al nodo nel percorso
							//	ottimale.
							// Svolge la stessa funzione della distanza
							//	nell'algoritmo di Dijkstra

	int last_node = -1;
	int route_type = -2;
	int dep_time = 0;	// Ora di partenza dell'arco che da last_node
						//	raggiunge il nodo corrispondente a questa
						//	etichetta
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


void inizializzaGrafi()
{
	ifstream file_corrente;
	string sottoriga;	// Stringa contenente la sottoriga
						//	corrispondente al i-esimo campo della
						//	riga corrente

	cout << "Calcolo di N... ";
	file_corrente.open("network_nodes.csv");
	string riga_precedente, riga_corrente;
	// E' necessario salvare il valore della riga precedente poiché
	//	l'ultima del file network_nodes è una riga vuota, mentre il
	//	valore dell'indice dell'ultimo nodo (quindi il valore massimo
	//	degli indici, poiché elencati in ordine crescente) è scritto
	//	sulla penultima riga
	while (!file_corrente.eof())
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
	getline(file_corrente, sottoriga);	// Ignora la prima riga
	while (!file_corrente.eof())
	{
		arco_pt nuovo_arco;

		getline(file_corrente, sottoriga, ';');
		int indice = atoi(sottoriga.c_str());

		getline(file_corrente, sottoriga, ';');
		int to_stop = atoi(sottoriga.c_str());
		nuovo_arco.to_stop = to_stop;

		getline(file_corrente, sottoriga, ';');
		int dep_time = atoi(sottoriga.c_str());
		nuovo_arco.dep_time = dep_time - 1481500800;
			// Il tempo fornito dai dati è in Unix time, il che vuol
			//	dire che lo 0 corrisponde al 01/01/1970. I dati presi
			//	corrispondono al 12/12/2016. L'ora esatta del 00:00:00
			//	del 12/12/2016 corrisponde al tempo Unix 1481500800,
			//	quindi sottraendo questo valore ai dati otterremo i
			//	tempi di partenza e di arrivo dei bus relativi alla
			//	mezzanotte, come in una normale tabella dei mezzi.
			// Questa modifica è fatta per semplificare l'inserimento
			//	dell'orario di partenza da input, potendo indicare
			//	semplicemente l'ora del giorno invece del tempo in
			//	tempo Unix

		getline(file_corrente, sottoriga, ';');
		int arr_time = atoi(sottoriga.c_str());
		nuovo_arco.arr_time = arr_time - 1481500800;	// Vedi sopra

		getline(file_corrente, sottoriga, ';');
		int route_type = atoi(sottoriga.c_str());
		nuovo_arco.route_type = route_type;

		// Ignora il resto della riga
		getline(file_corrente, sottoriga);

		grafo_pt[indice].push_back(nuovo_arco);
	}
	file_corrente.close();
	cout << "Fatto." << endl;

	cout << "Lettura di network_walk (potrebbe richiedere un po' di tempo)... ";
	file_corrente.open("network_walk.csv");
	getline(file_corrente, sottoriga);	// Ignora la prima riga
	while (!file_corrente.eof())
	{
		arco_walk nuovo_arco_andata, nuovo_arco_ritorno;

		getline(file_corrente, sottoriga, ';');
		int indice = atoi(sottoriga.c_str());
		nuovo_arco_ritorno.to_stop = indice;

		getline(file_corrente, sottoriga, ';');
		int to_stop = atoi(sottoriga.c_str());
		nuovo_arco_andata.to_stop = to_stop;

		// Ignora il terzo dato
		getline(file_corrente, sottoriga, ';');

		getline(file_corrente, sottoriga);
		int d_walk = atoi(sottoriga.c_str());
		nuovo_arco_andata.d_walk = d_walk;
		nuovo_arco_ritorno.d_walk = d_walk;

		grafo_walk[indice].push_back(nuovo_arco_andata);
		grafo_walk[to_stop].push_back(nuovo_arco_ritorno);
	}
	file_corrente.close();
	cout << "Fatto." << endl;
}

// Funzione che converte un tempo in secondi a partire dalla mezzanotte
//	in una stringa della forma hh:mm:ss
string intToTimeString(int time)
{
	int hours = time / 3600;
	int minutes = (time / 60) % 60;
	int seconds = time % 60;

	string to_return = "";

	if (hours >= 10) to_return += to_string(hours) + ":";
	else to_return += "0" + to_string(hours) + ":";

	if (minutes >= 10) to_return += to_string(minutes) + ":";
	else to_return += "0" + to_string(minutes) + ":";

	if (seconds >= 10) to_return += to_string(seconds);
	else to_return += "0" + to_string(seconds);

	return to_return;
}

//--- TFS = Time First Search ---//
// L'algoritmo, seppur l'avessi pensato originariamente come una
//	modifica del BFS (da cui il nome TFS), è in realtà una versione
//	riadattata dell'algoritmo di Dijkstra
void TFS(int nodo_iniziale, int nodo_finale, int tempo_iniziale, int tempo_massimo)
{
	etichetta_nodo* nodi = new etichetta_nodo[N];
	nodi[nodo_iniziale].arr_time = tempo_iniziale;

	ListaOrdinata coda;
	coda.Inserisci(to_string(tempo_iniziale) + "-" + to_string(nodo_iniziale), nodo_iniziale);

	bool guardia = true;

	while (!coda.Vuota() && !nodi[nodo_finale].visitato)
	{
		int u = coda.EstraiMinimo();
		nodi[u].visitato = true;
		if(nodi[u].arr_time - tempo_iniziale> tempo_massimo) break;
		for (arco_pt a : grafo_pt[u])
		{
			if (a.dep_time >= nodi[u].arr_time)
			{
				if (!nodi[a.to_stop].visitato && a.arr_time < nodi[a.to_stop].arr_time)
				{
					coda.Rimuovi(to_string(nodi[a.to_stop].arr_time) + "-" + to_string(a.to_stop));
					nodi[a.to_stop].arr_time = a.arr_time;
					nodi[a.to_stop].last_node = u;
					nodi[a.to_stop].route_type = a.route_type;
					nodi[a.to_stop].dep_time = a.dep_time;
					coda.Inserisci(to_string(a.arr_time) + "-" + to_string(a.to_stop), a.to_stop);
				}
			}
		}
		for (arco_walk a : grafo_walk[u])
		{
			int arr_time = nodi[u].arr_time + a.d_walk;

			if (!nodi[a.to_stop].visitato && arr_time < nodi[a.to_stop].arr_time)
			{
				coda.Rimuovi(to_string(nodi[a.to_stop].arr_time) + "-" + to_string(a.to_stop));
				nodi[a.to_stop].arr_time = arr_time;
				nodi[a.to_stop].last_node = u;
				nodi[a.to_stop].route_type = -1;
				nodi[a.to_stop].dep_time = nodi[u].arr_time;
				coda.Inserisci(to_string(arr_time) + "-" + to_string(a.to_stop), a.to_stop);
			}
		}
	}

	int x = nodo_finale;
	while (nodi[x].last_node != -1)
	{
		cout << "[" << intToTimeString(nodi[x].dep_time) << "-" << intToTimeString(nodi[x].arr_time) << "] da ";
		cout << nodi[x].last_node << " a " << x;
		switch (nodi[x].route_type)
		{
		case -1:
			cout << " camminando (walk)";
			break;
		case 0:
			cout << " con il tram";
			break;
		case 1:
			cout << " con la metropolitana (subway)";
			break;
		case 2:
			cout << " con il treno (rail)";
			break;
		case 3:
			cout << " con il bus";
			break;
		case 4:
			cout << " con il traghetto (ferry)";
			break;
		case 5:
			cout << " con il cablecar";
			break;
		case 6:
			cout << " con la gondola";
			break;
		case 7:
			cout << " con la funicolare";
			break;
		}
		cout << endl;
		x = nodi[x].last_node;
	}
}

int main()
{
	inizializzaGrafi();

	int nodo_iniziale, nodo_finale, tempo_iniziale, tempo_massimo;

	while (true)
	{
		cout << "Inserire nodo iniziale: ";
		cin >> nodo_iniziale;
		cout << "Inserire nodo finale: ";
		cin >> nodo_finale;
		cout << "Inserire tempo di partenza (in termini di secondi dalla mezzanotte, da 0 a 86399): ";
		cin >> tempo_iniziale;
		cout << "Inserire tempo massimo (in termini di secondi): ";
		cin >> tempo_massimo;

		cout << endl;

		TFS(nodo_iniziale, nodo_finale, tempo_iniziale, tempo_massimo);

		cout << endl << endl;
	}

	return 0;
}