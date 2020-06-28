#pragma once

// L'ora era tarda quando mi sono messo a commentare questo pezzo di
//	codice e mi sono permesso qualche commento poco "ortodosso"

#include<string>

using namespace std;

class ListaOrdinata
{
public:
	// Rimuovi il primo nodo della lista e ritornane il valore
	int EstraiMinimo()
	{
		if (lista != NULL)
		{
			int minimo = lista->val;

			ndl* temp = lista->successivo;
			free(lista);
			lista = temp;

			return minimo;
		}
		else
		{
			// Se la lista è vuota non c'è nessun minimo da ritornare,
			//	ritorna -1.
			// Tecnicamente non ci sarebbe da inserire questo check,
			//	poiché per come è implementato l'algoritmo,
			//	EstraiMinimo viene chiamato SEMPRE dopo l'aver
			//	controllato che la lista non sia vuota, quindi non ci
			//	imbatte mai in questo errore.
			// Tuttavia per questioni di completezza e poiché si tratta
			//	di pochissime righe di codice, ho deciso di
			//	implementare comunque questo check.

			// Qualcuno potrebbe obiettare che aggiungere questo enorme
			//	commento di giustificazione di esistenza di questo if
			//	else rende questo blocco non più semplicemente
			//	"pochissime righe di codice".
			// Beh, quel qualcuno avrebbe estremamente ragione, ma
			//	ormai siamo qui, queste righe sono scritte e non si
			//	torna più indietro.

			return -1;
		}
	}

	// Inserisci nella giusta posizione il seguente valore con la
	//	la seguente priorità
	void Inserisci(string priorità, int val)
	{
		ndl* temp = lista;
		// Il motivo per cui esiste il check sulla priorità
		//	dell'elemento in testa alla lista è che il blocco di codice
		//	che gestisce l'inserimento dell'elemento in posizione
		//	generica utilizza un puntatore (temp) all'elemento
		//	PRECEDENTE all'elemento nella posizione dove andrà inserito
		//	il nuovo elemento. Poiché il nodo in testa (è un bravo
		//	cittadino rispettoso della legge, ovvero) non ha nessun
		//	precedente, va trattato in modo diverso.
		if (temp == NULL || priorità.compare(temp->priorità) < 0)
		{
			// Se la lista è vuota o se l'elemento da inserire ha
			//	priorità alfabeticamente precedente a quella
			//	dell'elemento in testa alla lista, inserisci questo
			//	elemento in testa alla lista
			ndl* nuovo_nodo = new ndl;
			nuovo_nodo->priorità = priorità;
			nuovo_nodo->val = val;
			nuovo_nodo->successivo = lista;
			lista = nuovo_nodo;
		}
		else
		{
			while (temp->successivo != NULL && priorità.compare(temp->successivo->priorità) > 0)
			{
				temp = temp->successivo;
			}
			ndl* nuovo_nodo = new ndl;
			nuovo_nodo->priorità = priorità;
			nuovo_nodo->val = val;
			nuovo_nodo->successivo = temp->successivo;
			temp->successivo = nuovo_nodo;
		}
	}

	// Rimuovi dalla lista, se esiste, l'elemento con la seguente
	//	priorità
	void Rimuovi(string priorità)
	{
		ndl* temp = lista;
		if (temp != NULL)
		{
			// Il motivo per cui si tratta l'elemento in testa alla
			//	lista in modo diverso dal resto degli elementi è
			//	analogo a quello spiegato in Inserisci()
			if (priorità.compare(temp->priorità) == 0)
			{
				lista = lista->successivo;
				free(temp);
			}
			else
			{
				while (temp->successivo != NULL && priorità.compare(temp->successivo->priorità) > 0)
				{
					temp = temp->successivo;
				}
				// Il while scorre la lista finché essa non finisce o
				//	non si trova il primo valore con priorità uguale o
				//	(alfabeticamente) successiva a quella ricercata.
				// Infatti:
				//	- se la lista finisce, non serve cercare altrove
				//	- se abbiamo trovato un elemento con priorità
				//		uguale a quella cercata, non serve cercare
				//		altrove
				//	- se abbiamo trovato un elemento con priorità
				//		successiva a quella cercata, poiché gli
				//		elementi sono ordinati per priorità, sappiamo
				//		che sicuramente non esiste alcun elemento con
				//		la priorità cercata, quindi non serve cercare
				//		altrove

				// Controlliamo ora il motivo di arresto e nel caso sia
				//	il secondo, eliminiamo l'elemento
				if (temp->successivo != NULL && priorità.compare(temp->successivo->priorità) == 0)
				{
					ndl* temp2 = temp->successivo;
					temp->successivo = temp->successivo->successivo;
					free(temp2);
				}
			}
		}
	}

	// Ritorna true se la lista è vuota, false altrimenti
	bool Vuota()
	{
		return (lista == NULL);
	}

private:
	// Struct contenente il singolo nodo della lista (ndl)
	struct ndl
	{
		string priorità = "";	// Priorità secondo la quale viene
								//	ordinata la lista (alfabeticamente)
		int val = -1;
		ndl* successivo = NULL;
	};

	ndl* lista = NULL;
};