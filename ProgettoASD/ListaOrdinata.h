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
	//	la seguente priorit�
	void Inserisci(string priorit�, int val)
	{
		ndl* temp = lista;
		if (temp == NULL || priorit�.compare(temp->priorit�) < 0)
		{
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

	void Rimuovi(string priorit�)
	{
		ndl* temp = lista;
		if (temp != NULL)
		{
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
				if (temp->successivo != NULL && priorit�.compare(temp->successivo->priorit�) == 0)
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
			cout << temp->priorit� << ", " << temp->val << endl;
			temp = temp->successivo;
		}
	}
	*/

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