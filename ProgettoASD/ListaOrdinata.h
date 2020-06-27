#pragma once

#include<string>
using namespace std;

// ndl = Nodo Di Lista
struct ndl
{
	string priorità = "";
	int val = -1;
	ndl* successivo = NULL;
};

class ListaOrdinata
{
public:
	int RimuoviMinimo()
	{
		int minimo = lista->val;

		ndl* temp = lista->successivo;
		free(lista);
		lista = temp;

		return minimo;
	}

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
	ndl* lista;
};