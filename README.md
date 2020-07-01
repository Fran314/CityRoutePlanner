# City Route Planner - Progetto ASD
Progetto di Francesco Baldino per il corso Algoritmi e Strutture Dati 2019/2020 della triennale di Matematica dell'Università di Pisa.

Il progetto contiene alcuni file utilizzati da Visual Studio (l'IDE che ho usato per scrivere e compilare il codice) non strettamente necessari per il funzionamento del programma.
Gli unici file necessari sono:
- ListaOrdinata.h
- Main.cpp
- network_nodes.csv
- network_temporal_day.csv
- network_walk.csv

Come spiegato in maggior dettaglio nei commenti del codice, l'algoritmo utilizzato è di fatto una modifica dell'Algoritmo di Dijkstra (per la ricerca del percorso ottimale nei grafi pesati). In questo caso, come peso del singolo arco ho utilizzato non solo la durata della tratta ma anche il tempo di attesa da quando si arriva sul nodo analizzato a quando parte la tratta dell'arco analizzato. Così facendo, si dimostra (in modo del tutto analogo all'algoritmo di Dijkstra) che quando si analizza un generico nodo, il percorso utilizzato per arrivare a quel nodo è ottimale (rispetto alla somma dei pesi considerati). Nel nostro caso quindi sarà il percosco che impiega meno tempo di tutti ad arrivare a destinazione. 

### Problemi
Il programma così come fornito è (a meno di bug di cui non mi sono accorto) perfettamente funzionante per quello che deve fare. Tuttavia, ci sono alcune problematiche che lo rendono leggermente meno "user friendly". In particolare:
- Il caricamento dei file impiega (sul mio computer) anche fino ad un minuto. Non credo che sia possibile ottimizzare questo tempo a meno di modificare i file dei dati.
- I nodi di partenza e di arrivo vanno inseriti come numeri e non come nomi delle fermate. Non sono sicuro che questo sia un reale problema all'utilizzo, anche perché esistono più nodi con lo stesso nome, e questo potrebbe creare confusione (anche se è vero che nodi con lo stesso nome sono tendenzialmente molto vicini uno dall'altro, quindi non cambierebbe di molto il risultato dato dall'algoritmo. In ogni caso renderebbe comunque più complicato il partire da una stazione specifica, dovendo specificare prima il nome e poi, comunque, l'indice della stazione).
- L'inserimento dell'ora di partenza e del tempo massimo sono in secondi. Era il modo più facile che avevo per implementare l'inserimento dell'ora di partenza, ma potrebbe risultare meno intuitivo dover dire di voler partire alle 61044 secondi invece che alle 16:57:24. In caso di necessità è possibile implementare questa funzione.
- Il programma NON fa particolari tentativi di error checking, ovvero si aspetta che i dati inseriti dall'utente siano sensati. Dei dati non reali (un tempo negativo, un indice di un nodo minore di zero, maggiore del massimo indice oppure un indice che non corrisponde a nessun nodo (ne esistono parecchi)) non daranno risposte sensate e non avviseranno l'utente che i dati inseriti non sono sensati. Per un esperienza migliore si prega di controllare i dati prima di inserirli.
