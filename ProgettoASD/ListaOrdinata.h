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
			// Se la lista � vuota non c'� nessun minimo da ritornare,
			//	ritorna -1.
			// Tecnicamente non ci sarebbe da inserire questo check,
			//	poich� per come � implementato l'algoritmo,
			//	EstraiMinimo viene chiamato SEMPRE dopo l'aver
			//	controllato che la lista non sia vuota, quindi non ci
			//	imbatte mai in questo errore.
			// Tuttavia per questioni di completezza e poich� si tratta
			//	di pochissime righe di codice, ho deciso di
			//	implementare comunque questo check.

			// Qualcuno potrebbe obiettare che aggiungere questo enorme
			//	commento di giustificazione di esistenza di questo if
			//	else rende questo blocco non pi� semplicemente
			//	"pochissime righe di codice".
			// Beh, quel qualcuno avrebbe estremamente ragione, ma
			//	ormai siamo qui, queste righe sono scritte e non si
			//	torna pi� indietro.

			return -1;
		}
	}

	// Inserisci nella giusta posizione il seguente valore con la
	//	la seguente priorit�
	void Inserisci(string priorit�, int val)
	{
		ndl* temp = lista;
		// Il motivo per cui esiste il check sulla priorit�
		//	dell'elemento in testa alla lista � che il blocco di codice
		//	che gestisce l'inserimento dell'elemento in posizione
		//	generica utilizza un puntatore (temp) all'elemento
		//	PRECEDENTE all'elemento nella posizione dove andr� inserito
		//	il nuovo elemento. Poich� il nodo in testa (� un bravo
		//	cittadino rispettoso della legge, ovvero) non ha nessun
		//	precedente, va trattato in modo diverso.
		if (temp == NULL || priorit�.compare(temp->priorit�) < 0)
		{
			// Se la lista � vuota o se l'elemento da inserire ha
			//	priorit� alfabeticamente precedente a quella
			//	dell'elemento in testa alla lista, inserisci questo
			//	elemento in testa alla lista
			ndl* nuovo_nodo = new ndl;
			nuovo_nodo->priorit� = priorit�;
			nuovo_nodo->val = val;
			nuovo_nodo->successivo = lista;
			lista = nuovo_nodo;
		}
		else
		{
			while (temp->successivo != NULL && priorit�.compare(temp->successivo->priorit�) > 0)
			{
				temp = temp->successivo;
			}
			ndl* nuovo_nodo = new ndl;
			nuovo_nodo->priorit� = priorit�;
			nuovo_nodo->val = val;
			nuovo_nodo->successivo = temp->successivo;
			temp->successivo = nuovo_nodo;
		}
	}

	// Rimuovi dalla lista, se esiste, l'elemento con la seguente
	//	priorit�
	void Rimuovi(string priorit�)
	{
		ndl* temp = lista;
		if (temp != NULL)
		{
			// Il motivo per cui si tratta l'elemento in testa alla
			//	lista in modo diverso dal resto degli elementi �
			//	analogo a quello spiegato in Inserisci()
			if (priorit�.compare(temp->priorit�) == 0)
			{
				lista = lista->successivo;
				free(temp);
			}
			else
			{
				while (temp->successivo != NULL && priorit�.compare(temp->successivo->priorit�) > 0)
				{
					temp = temp->successivo;
				}
				// Il while scorre la lista finch� essa non finisce o
				//	non si trova il primo valore con priorit� uguale o
				//	(alfabeticamente) successiva a quella ricercata.
				// Infatti:
				//	- se la lista finisce, non serve cercare altrove
				//	- se abbiamo trovato un elemento con priorit�
				//		uguale a quella cercata, non serve cercare
				//		altrove
				//	- se abbiamo trovato un elemento con priorit�
				//		successiva a quella cercata, poich� gli
				//		elementi sono ordinati per priorit�, sappiamo
				//		che sicuramente non esiste alcun elemento con
				//		la priorit� cercata, quindi non serve cercare
				//		altrove

				// Controlliamo ora il motivo di arresto e nel caso sia
				//	il secondo, eliminiamo l'elemento
				if (temp->successivo != NULL && priorit�.compare(temp->successivo->priorit�) == 0)
				{
					ndl* temp2 = temp->successivo;
					temp->successivo = temp->successivo->successivo;
					free(temp2);
				}
			}
		}
	}

	// Ritorna true se la lista � vuota, false altrimenti
	bool Vuota()
	{
		return (lista == NULL);
	}

private:
	// Struct contenente il singolo nodo della lista (ndl)
	struct ndl
	{
		string priorit� = "";	// Priorit� secondo la quale viene
								//	ordinata la lista (alfabeticamente)
		int val = -1;
		ndl* successivo = NULL;
	};

	ndl* lista = NULL;
};