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
		if (temp == NULL || priorità.compare(temp->priorità) < 0)
		{
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

	void Rimuovi(string priorità)
	{
		ndl* temp = lista;
		if (temp != NULL)
		{
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
				if (temp->successivo != NULL && priorità.compare(temp->successivo->priorità) == 0)
				{
					ndl* temp2 = temp->successivo;
					temp->successivo = temp->successivo->successivo;
					free(temp2);
				}
			}
		}
	}

	bool Vuota()
	{
		return (lista == NULL);
	}

	/*
	void Stampa()
	{
		ndl* temp = lista;
		while (temp != NULL)
		{
			cout << temp->priorità << ", " << temp->val << endl;
			temp = temp->successivo;
		}
	}
	*/

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