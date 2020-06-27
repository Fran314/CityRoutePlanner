#pragma once

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
			return -1;
		}
	}

	// Inserisci nella giusta posizione il seguente valore con la
	//	la seguente priorità
	void Inserisci(string priorità, int val)
	{
		ndl* temp = lista;
		if (temp != NULL)
		{
			while (temp->successivo != NULL && priorità.compare(temp->successivo->priorità) > 0)
			{
				temp = temp->successivo;
			}
			ndl* nuovo_nodo = new ndl;
		}
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

	ndl* lista;
};