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
	int t_walk;		// t_walk corrisponde al tempo impiegato per
					//	per percorrere la distanza d_walk del file
					//	supponendo di camminare a 5 km/h
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
bool* esiste;	// Array contenente un booleano per ogni indice.
				// Il motivo per cui questo array esiste è che non ogni
				//	indice è associato ad un nodo, e quindi un indice
				//	inserito dall'utente potrebbe non corrispondere a
				//	nessun nodo iniziale/finale. Questo array viene
				//	utilizzato per controllare quali indici sono
				//	associati a dei nodi, ovvero per sapere se dato
				//	[i] indice, se il nodo i-esimo esiste. L'i-esimo
				//	booleano di questo array è true se esiste un nodo
				//	associato all'indice i, false altrimenti


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

	esiste = new bool[N];
	grafo_pt = new vector<arco_pt>[N];
	grafo_walk = new vector<arco_walk>[N];

	cout << "Lettura di network_nodes... ";
	file_corrente.open("network_nodes.csv");
	getline(file_corrente, sottoriga);	// Ignora la prima riga
	for (int i = 0; i < N; i++) esiste[i] = false;
	while (!file_corrente.eof())
	{
		getline(file_corrente, sottoriga, ';');
		int indice = atoi(sottoriga.c_str());
		esiste[indice] = true;

		// Ignora il resto della riga
		getline(file_corrente, sottoriga);
	}
	file_corrente.close();
	cout << "Fatto." << endl;

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
		int t_walk = (atoi(sottoriga.c_str()) * 36.0 / 50.0);
		// Prende la distanza in d_walk e la corregge in tempo
		//	impiegato per percorrerla supponendo di camminare a 5 km/h
		nuovo_arco_andata.t_walk = t_walk;
		nuovo_arco_ritorno.t_walk = t_walk;

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
	int ore = time / 3600;
	int minuti = (time / 60) % 60;
	int secondi = time % 60;

	string da_ritornare = "";

	if (ore >= 10) da_ritornare += to_string(ore) + ":";
	else da_ritornare += "0" + to_string(ore) + ":";

	if (minuti >= 10) da_ritornare += to_string(minuti) + ":";
	else da_ritornare += "0" + to_string(minuti) + ":";

	if (secondi >= 10) da_ritornare += to_string(secondi);
	else da_ritornare += "0" + to_string(secondi);

	return da_ritornare;
}

// Funzione che trasforma un qualsiasi numero in una stringa di almeno
//	5 caratteri, eventualmente riempiendo gli spazi a sinistra con
//	degli zeri. Questo serve per la priorità degli elementi nella coda
//	dell'algoritmo di pathfinding, poiché questi vengono ordinati
//	principalmente rispetto ad un tempo scritto in secondi in una
//	stringa e ordinati in ordine alfabetico. C'è bisogno di unificare
//	la quantità di cifre poiché altrimenti la stringa 200, che
//	rappresenta il momento nel tempo 200 secondi dopo la mezzanotte
//	(00:03:20), verrebbe DOPO la stringa 10000, che rappresenta il
//	momento nel tempo 10000 secondi dopo la mezzanotte (02:46:40),
//	ma PRIMA di 3000 (ovvero 00:50:00) nonostante siano ENTRAMBI dopo
//	temporalmente, creado quindi un ordine non corrispondente a quello
//	temporale che a noi interessa.
//	Le stringhe 00200, 03000 e 10000 sono invece ugualmente ordinate
//	temporalmente e alfabeticamente
string intTo5DigitString(int arg)
{
	if (arg < 10) return "0000" + to_string(arg);
	if (arg < 100) return "000" + to_string(arg);
	if (arg < 1000) return "00" + to_string(arg);
	if (arg < 10000) return "0" + to_string(arg);
	return to_string(arg);
	// N.B: se l'argomento avesse PIU' di 5 cifre, anche l'output
	//	avrebbe più di 5 caratteri. Tuttavia, questa funzione verrà
	//	chiamata solo su numeri con al più 5 cifre, quindi non c'è
	//	bisogno di gestire questo tipo di "errore". Allo stesso modo,
	//	la funzione non si preoccupa di eventuali argomenti negativi
	//	poiché verrà chiamata solo su numeri positivi.
}

//--- TFS = Time First Search ---//
// L'algoritmo, seppur l'avessi pensato originariamente come una
//	modifica del BFS (da cui il nome TFS), è in realtà una versione
//	riadattata dell'algoritmo di Dijkstra.
// La differenza principale dall'algoritmo di Dijkstra consiste nel
//	peso degli archi: se in Dijkstra il peso dell'arco è costante e
//	noto a priori, nel nostro caso il peso del singolo arco sarà
//	composto da una parte costante (la durata del percorso) e da una
//	parte variabile dipendente dal contesto (il tempo di attesa da
//	quando si arriva su un nodo a quando si prende il mezzo pubblico
//	del relativo arco).
// Questo potrebbe sembrare un problema, ma può essere risolto in modo
//	molto semplice: osserviamo innanzitutto che nell'algoritmo di
//	Dijkstra, supponendo di disporre di un altro modo per calcolare la
//	distanza dal generico nodo alla sorgente, non è fondamentale sapere
//	il peso degli archi, e quindi nel nostro caso non sarebbe
//	necessario calcolare il tempo di attesa. Inoltre, un'altra cosa che
//	è lecita fare e che non previene l'algoritmo dal funzionare
//	correttamente, è incrementare tutte le distanze dei vari nodi di
//	una costante positiva, in modo che la sorgente si trovi a
//	"distanza K" da se stesso invece che a distanza 0, e tutti gli
//	altri a distanza d+K. Questa modifica fa comunque funzionare
//	correttamente l'algoritmo poiché l'unica cosa che importa non è la
//	distanza effettiva ma l'ordine tra le distanze.
// Fatte queste premesse, immaginiamo ora di incrementare tutte le
//	distanze di [tempo_iniziale]. Così facendo, i nodi adiacenti si
//	troveranno a distanza ["tempo di attesa"]+["durata della tratta"]
//	+[tempo_iniziale]. Osserviamo però che indipendentemente dall'arco,
//	la somma di questi tre valori corrisponde sempre al arr_time
//	dell'arco con il quale si raggiunge il nodo. Allora disponiamo di
//	modo di calcolare la distanza senza dover necessariamente sapere
//	il tempo di attesa per la singola tratta, potendo usare
//	semplicemente il valore di arr_time come distanza del nodo dalla
//	sorgente.
void TFS(int nodo_iniziale, int nodo_finale, int tempo_iniziale, int tempo_massimo)
{
	etichetta_nodo* etichette = new etichetta_nodo[N];
	etichette[nodo_iniziale].arr_time = tempo_iniziale;
	// Tanto quanto in Dijkstra settiamo la distanza del primo nodo a
	//	0, qui settiamo la distanza del primo nodo a [tempo_iniziale].
	// Il perché è spiegato sopra questa funzione.

	ListaOrdinata coda;
	coda.Inserisci(intTo5DigitString(tempo_iniziale) + "-" + to_string(nodo_iniziale), nodo_iniziale);
	// La priorità di ogni elemento della lista non è data SOLO dal
	//	tempo di arrivo (come spiegato sopra questa funzione) ma dal
	//	tempo di arrivo e dall'indice del corrispondente nodo. Questo
	//	perché per semplicità di codice, si suppone che ogni elemento
	//	della lista ordinata abbia priorità unica tra le altre. Poiché
	//	però è possibile che due nodi abbiamo la stessa distanza in
	//	tempo dal nodo iniziale, ci assicuriamo l'unicità della
	//	priorità aggiungendo l'indice del nodo (infatti ogni nodo
	//	compare al più una volta nella coda, casomai cambiando
	//	posizione). La scelta di mettere un trattino tra i due valori
	//	è puramente cosmesi estetica dei dati.

	bool sforato_tempo_massimo = false;

	while (!coda.Vuota() && !etichette[nodo_finale].visitato)
	{
		int u = coda.EstraiMinimo();
		etichette[u].visitato = true;
		if (etichette[u].arr_time - tempo_iniziale > tempo_massimo)
		{
			// Se il tempo impiegato per raggiungere il nodo che stiamo
			//	analizzando è maggiore del tempo massimo, allora poiché
			//	non abbiamo analizzato ancora il nodo finale (infatti
			//	altrimenti saremmo usciti dal while), sappiamo che
			//	anche fosse possibile raggiungerlo, lo raggiungeremmo
			//	dopo questo nodo e quindi dopo il tempo massimo. Allora
			//	termina il while.
			// N.B: anche se il nodo che stiamo analizzando fosse il
			//	nodo finale, l'algoritmo dovrebbe dare come risultato
			//	che era impossibile raggiungere il nodo entro il tempo
			//	massimo. Per questo motivo, nell'output oltre il while,
			//	viene prima controllato che non sia stato sforato il
			//	tempo massimo.
			sforato_tempo_massimo = true;
			break;
		}

		// Gli archi rappresentanti trasporti pubblici e gli archi
		//	rappresentanti camminate vengono trattati in modo
		//	differente poiché nel secondo caso non esiste nessun tempo
		//	di attesa (a meno che non siate pigri e vogliate riprendere
		//	fiato, ma in quel caso state guardando l'algoritmo
		//	sbagliato. Questo algoritmo è solo per veri camminatori
		//	esperti e instancabili), e va quindi calcolato il tempo di
		//	arrivo al nodo successivo sommando il tempo di arrivo al
		//	nodo attuale e la durata della camminata.
		for (arco_pt a : grafo_pt[u])
		{
			// Considera l'arco solo se il suo tempo di partenza è
			//	successivo al tempo di arrivo al nodo attuale.
			// Se così non fosse, sarebbe impossibile prendere il
			//	relativo mezzo pubblico.
			if (a.dep_time >= etichette[u].arr_time)
			{
				if (!etichette[a.to_stop].visitato && a.arr_time < etichette[a.to_stop].arr_time)
				{
					coda.Rimuovi(intTo5DigitString(etichette[a.to_stop].arr_time) + "-" + to_string(a.to_stop));
					etichette[a.to_stop].arr_time = a.arr_time;
					etichette[a.to_stop].last_node = u;
					etichette[a.to_stop].route_type = a.route_type;
					etichette[a.to_stop].dep_time = a.dep_time;
					coda.Inserisci(intTo5DigitString(a.arr_time) + "-" + to_string(a.to_stop), a.to_stop);
				}
			}
		}
		for (arco_walk a : grafo_walk[u])
		{
			int arr_time = etichette[u].arr_time + a.t_walk;
			// Calcolo del tempo di arrivo al nodo successivo sommando
			//	il tempo di arrivo al nodo attuale e la durata della
			//	camminata.

			if (!etichette[a.to_stop].visitato && arr_time < etichette[a.to_stop].arr_time)
			{
				coda.Rimuovi(intTo5DigitString(etichette[a.to_stop].arr_time) + "-" + to_string(a.to_stop));
				etichette[a.to_stop].arr_time = arr_time;
				etichette[a.to_stop].last_node = u;
				etichette[a.to_stop].route_type = -1;
				etichette[a.to_stop].dep_time = etichette[u].arr_time;
				coda.Inserisci(intTo5DigitString(arr_time) + "-" + to_string(a.to_stop), a.to_stop);
			}
		}
	}

	if (sforato_tempo_massimo)
	{
		cout << "Non e' stato possibile raggiungere il nodo finale entro il tempo massimo";
	}
	if (etichette[nodo_finale].visitato)
	{
		// Percorso trovato: stampa il percorso

		int x = nodo_finale;
		while (etichette[x].last_node != -1)
		{
			cout << "[" << intToTimeString(etichette[x].dep_time) << "-" << intToTimeString(etichette[x].arr_time) << "] da ";
			cout << etichette[x].last_node << " a " << x;
			switch (etichette[x].route_type)
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
			x = etichette[x].last_node;
		}
	}
	else
	{
		// Se siamo arrivati a questo blocco di codice è perché il
		//	il while è stato terminato perché la coda è vuota e
		//	l'ultimo nodo analizzato NON era quello finale. Questa
		//	osservazione va fatta poiché nel raro caso in cui nella
		//	coda fosse rimasto un solo nodo che fosse anche quello
		//	finale e che questo non avesse vicini da aggiungere alla
		//	coda, allora avremmo trovato il percorso ANCHE SE la coda
		//	al termine dell'algoritmo era vuota. Per risolvere questo
		//	problema, prima controlliamo che il nodo finale non sia
		//	stato effettivamente trovato
		// Non è stato trovato nessun percorso perché la coda, ad un
		//	certo punto dell'algoritmo, era vuota.
		// Allora non esistono percorsi che utilizzino solo mezzi del
		//	lunedì per arrivare dal nodo finale.

		// N.B: sono quasi sicuro che in realtà non possa mai accadere
		//	che, dati due nodi esistenti, l'algoritmo su quei due input
		//	si arresti per coda vuota. Infatti gli archi di camminata
		//	(che non dipendono da un'ora di partenza fissata) rendono
		//	il grafo connesso. Inoltre, anche se non esistono archi di
		//	trasporti pubblici oltre una certa ora del lunedì, gli
		//	archi di camminata esistono indipendentemente dall'ora,
		//	quindi supponendo di avere sufficiente tempo massimo, è
		//	possibile completare qualsiasi percorso al più solo
		//	camminando.
		// Tuttavia, per completezza di codice (e per essere sicuro che
		//	funzioni su tutti gli input (di file) possibili, quindi
		//	anche quelli dove non sono garantite le proprietà appena
		//	citate), ho deciso di inserire anche questo caso di output.

		cout << "Non e' stato possibile completare il percorso poiche' i due nodi non sono connessi a partire dall'ora iniziale data" << endl;
	}
}

int main()
{
	inizializzaGrafi();

	int nodo_iniziale, nodo_finale, tempo_iniziale, tempo_massimo;

	// Il motivo di questo ciclo while è dato dalla lentezza di
	//	caricamento dei dati iniziali.
	// L'algoritmo di pathfinding è relativamente veloce, ma il tempo
	//	di lettura dei dati (quantomeno quelli relativi a Parigi) si
	//	aggira attorno al minuto sul mio computer, che è davvero tanto.
	// Quindi, nel caso si volesse testare il programma con più di un
	//	percorso, questo ciclo while permette di testarne più di uno
	//	senza dover ricaricare i dati degli archi.

	while (true)
	{
		bool ripeti_input = true;
		do
		{
			cout << "Inserire l'indice del nodo iniziale (o inserire -1 per uscire): ";
			cin >> nodo_iniziale;
			if (nodo_iniziale == -1 || (nodo_iniziale >= 0 && nodo_iniziale < N && esiste[nodo_iniziale]))
				ripeti_input = false;

			if(ripeti_input)
				cout << "Indice non esistente. ";
		} while (ripeti_input);
		if (nodo_iniziale == -1) break;

		ripeti_input = true;
		do
		{
			cout << "Inserire l'indice del nodo finale: ";
			cin >> nodo_finale;
			if (nodo_finale >= 0 && nodo_finale < N && esiste[nodo_finale])
				ripeti_input = false;

			if (ripeti_input)
				cout << "Indice non esistente. ";
		} while (ripeti_input);

		ripeti_input = true;
		do
		{
			string input = "";
			cout << "Inserire tempo di partenza [formato hh:mm:ss]: ";
			cin >> input;

			if (input.length() >= 8)
			{
				if (input[2] == ':' && input[5] == ':' && isdigit(input[0]) &&
					isdigit(input[1]) && isdigit(input[3]) && isdigit(input[4]) &&
					isdigit(input[6]) && isdigit(input[7]))
				{
					int ore = atoi(input.substr(0, 2).c_str());
					int minuti = atoi(input.substr(3, 2).c_str());
					int secondi = atoi(input.substr(6, 2).c_str());

					if (ore < 24 && minuti < 60 && secondi < 60)
					{
						tempo_iniziale = (ore * 3600) + (minuti * 60) + secondi;
						ripeti_input = false;
					}
				}
			}

			if (ripeti_input)
				cout << "La stringa inserita non è del formato giusto. ";
		} while (ripeti_input);

		ripeti_input = true;
		do
		{
			string input = "";
			cout << "Inserire tempo massimo [formato hh:mm:ss] (N.B: NON ora massima): ";
			cin >> input;

			if (input.length() >= 8)
			{
				if (input[2] == ':' && input[5] == ':' && isdigit(input[0]) &&
					isdigit(input[1]) && isdigit(input[3]) && isdigit(input[4]) &&
					isdigit(input[6]) && isdigit(input[7]))
				{
					int ore = atoi(input.substr(0, 2).c_str());
					int minuti = atoi(input.substr(3, 2).c_str());
					int secondi = atoi(input.substr(6, 2).c_str());

					if (ore < 24 && minuti < 60 && secondi < 60)
					{
						tempo_massimo = (ore * 3600) + (minuti * 60) + secondi;
						ripeti_input = false;
					}
				}
			}

			if (ripeti_input)
				cout << "La stringa inserita non è del formato giusto. ";
		} while (ripeti_input);

		cout << endl;

		TFS(nodo_iniziale, nodo_finale, tempo_iniziale, tempo_massimo);

		cout << endl << endl;
	}

	return 0;
}