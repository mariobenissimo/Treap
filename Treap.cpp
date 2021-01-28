#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

// Treap nodo
template <class T> struct TreapNode
{
    T data;
    int priority;
    TreapNode* left, *right;

    // costruttore che imposta la priority data
    TreapNode(T data,int priority)
    {
        this->data = data;
        this->priority = priority;
        this->left = this->right = nullptr;
    }
    // costruttore che imposta la priority casualmente
    TreapNode(T data)
    {
        this->data = data;
        this->priority = rand()%100;
        this->left = this->right = nullptr;
    }
};

template <class T> class Treap{

  private:

  TreapNode<T>* root; //radice dell'albero

  public:

    Treap(){ //costruttore
      this->root=nullptr;
    }

    TreapNode<T>* getRoot(){
      return root;
    }
    /*

          r                         R
         / \      Left Rotate      / \
        L   R      –------->      r   Y
           / \                   / \
          X   Y                 L   X
    */
    void rotateLeft(TreapNode<T>* &node)
    {
        TreapNode<T>* R = node->right;
        TreapNode<T>* X = node->right->left;

        R->left = node;
        node->right = X;

        node = R;
    }

    void rotateRight(TreapNode<T>* &node)
    {
        TreapNode<T>* L = node->left;
        TreapNode<T>* Y = node->left->right;

        L->right = node;
        node->left = Y;

        node = L;
    }
    void insertNode(TreapNode<T>* &node, T data,int priority)
    {
        if (node == nullptr){ // se l'albero è vuoto viene inserito il primo nodo come radice
            node = new TreapNode<T>(data,priority);
            return;
        }
        if (data < node->data){
            insertNode(node->left, data, priority); // ricerca la giusta collocazione nell'albero binario
            if (node->left != nullptr && node->left->priority > node->priority) // se non vengono rispettate le proprietà relative all'heap ovvero
                rotateRight(node);                                              // il suo figlio sinistro ha una priority più alta del nodo stesso viene effettuata una rotazione
        }
        else{
            insertNode(node->right, data, priority); // ricerca la giusta collocazione nell'albero binario
            if (node->right != nullptr && node->right->priority > node->priority)   // se non vengono rispettate le proprietà relative all'heap ovvero
                rotateLeft(node);                                                  // il suo figlio destro ha una priority più alta del nodo stesso viene effettuata una rotazione
        }
    }
    void deleteNode(TreapNode<T>* &node, T key) // alla prima chiamata il node sarà la root poi cambierà da chiamata in chiamata
    {
        if (node == nullptr) return; // nel caso in cui l'albero è vuoto
        if (key < node->data) deleteNode(node->left, key); // ricerca del nodo da cancellare
        else if (key > node->data) deleteNode(node->right, key);
        else
        {
            if (node->left == nullptr && node->right == nullptr){ // Caso 1: IL nodo del treap da cancellare non ha figli e quindi lo cancello senza
                delete node;                                      // provocare nessun problema
                node = nullptr;
            }
            else if (node->left && node->right){ // Il nodo del treap da cancellare ha entrambi i figli
                if (node->left->priority < node->right->priority){ // in base ai valori della priority dei rispettivi figli decido
                    rotateLeft(node);                              //se effettuare una rotazione verso sinistra o una rotazione verso destra a quel punto
                    deleteNode(node->left, key);                   // invoco nuovamente la delete fin quando il nodo da cancellare non verrà collocato nel
                                                                  // caso n°1 o nel caso n°2
                }
                else{
                    rotateRight(node);
                    deleteNode(node->right, key);
                }
            }
            else{ // Caso 2: Il nodo del treap da cancellare ha soltanto un figlio
                TreapNode<T>* child = (node->left)? node->left: node->right; // se il nodo ha soltanto un figlio memorizzo in child l'unico nodo che ha
                TreapNode<T>* curr = node; // memorizzo in curr il nodo da cancellare
                node = child; // il nodo figlio prende il posto del padre e successivamente cancello il nodo
                delete curr;
            }
        }
  }
  void heapify(TreapNode<T> *t) { // heapify function per mantenere inalterate le proprietà dell'heap
      if (!t) return;             // se il nodo su cui invochiamo la funzione è null la procedura termina

      TreapNode<T>* max = t;

      if (t->left != NULL  and t->left->priority  > max->priority) max = t->left; // il max sarà dunque il filgio sinistro
      if (t->right != NULL and t->right->priority > max->priority) max = t->right; // il max sarà dunque il filgio destro
      if (max != t){//se il max(nodo) è diverso da quello passato alla funzione si effettua lo scambio dei nodi e si chiama ricorsivamente sul nuovo max
          swap(t->priority, max->priority);
          heapify (max);
     }
  }
  TreapNode<T> *build(T v[], int l, int r) { // funzione che crea un heap partendo da un vettore
      if (l > r) return NULL; // se l'estremo sinistro è maggiore dell'estremo destro restituisce NULL ( caso base della ricorsione)

      int m = (l + r) / 2; // memorizzo la mediana

      TreapNode<T>*t = new TreapNode<T>(v[m]); // creo un nuovo nodo con valore v[m] e priority casuale
      t->left = build(v,l, m-1); //il nodo appena creato avrà come figlio sinitro il nodo restituito dalla stessa procedura invocata però nel sottoarray [l..m-1]
      t->right = build(v, m+1, r); ; //il nodo appena creato avrà come figlio destro il nodo restituito dalla stessa procedura invocata però nel sottoarray [m+1..r]

      heapify(t); // invoco heapify su t per verificare che il treap appena creato verifica le proprietà dell'heap
      return t;
  }
  void split(TreapNode<T>* &t, T key, TreapNode<T>* &l, TreapNode<T>* &r) { //procedura che permette di dividere un treap in due
      // il valore key rappresenta il punto, ovvero il nodo da cui voglio effettuare la split
      int inf = 0x3f3f3f3f; //valore sentinella molto alto, il massimo rappresentabile con un tipo INT
      insertNode(t, key, inf); //inserisco un nodo che ha priorità assoluta e lo stesso valore su cui voglio effettuare la split
      // a questo punto la procedura insert sistemerà il treap originale in due parti L e R in modo che sia L che R siano anch'essi treap.
      //tutti i nodi di L sono più piccoli o uguali alla chiave key e tutti i nodi di R sono maggiori di key.
      l = t->left;
      r = t->right;
  }
  void printTreap(TreapNode<T> *node, int space = 0, int height = 10) // funzione che permette di visualizzare un albero in maniera ordinata
  {                                                                   // visualizzare da sinistra verso destra, il primo nodo a sinistra corrisponde alla radice
      if (node == nullptr) return;                                    // andando da sinistra verso destra scendiamo nel nostro treap da livello a livello fino all'ultima colonna
      space += height;                                                // dove ci saranno le foglie
      printTreap(node->right, space);
      cout << endl;
      for (int i = height; i < space; i++)
          cout << ' ';
      cout << node->data << "(" << node->priority << ")\n";
      cout << endl;
      printTreap(node->left, space);
  }
  TreapNode<T>* search(TreapNode<T> * root, int key)
  {
    if (root == NULL || root->key == key)
       return root;
    if (root->key < key)
       return search(root->right, key);
    return search(root->left, key);
  }
};
// si è preferita una implementazione utilizzando i template in modo tale memorizzare qualsiasi tipo di dato all'interno del
//treap purchè è possibile effettuare i confronti
int main()
{
    int keys[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'L' };
    int priority[] = { 30, 33, 2, 21, 8, 60, 92, 46, 25, 10 };
    int n = sizeof(keys)/sizeof(int);
    Treap<char>* treap=new Treap<char>();
    TreapNode<char>* root = treap->getRoot();
    srand(time(nullptr));
    //ONLINE BUILDING n chiamate serial per inserire n nodi con pririorità impostata
    for(int i=0;i<n;i++){
      treap->insertNode(root, keys[i], priority[i]);
    }
    cout <<"ATTENZIONE: per poter apprezzare le procedure del treap e' stata implementata una funzione che tenta di rappresentare graficamente il treap.\n";
    cout <<"Chiave di lettura: la radice e' il primo elemento a sinistra e successivamente si visualizza da sinistra verso destra girando la testa di novanta gradi verso sinistra\n\n";
    cout << "Online building con N chiamate seriali alla operazione insert (le lettere sono le chiavi invece i numeri sono le pririorità):\n\n";
    treap->printTreap(root); //visualizzazione del treap
    cout << "cancellazione del G(92):\n\n";
    treap->deleteNode(root, 'G');
    treap->printTreap(root);


    cout<<"Offline building partendo dal un vettore di chiavi (la priorità è impostata casualmente): \n\n"; //implementazione tramite offline BUILDING
    int keysOffline[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90}; //vettore dei valori la priority viene impostata casuale
    Treap<int>* treapOffline=new Treap<int>();
    TreapNode<int>* t=treapOffline->build(keysOffline,0,9);
    treapOffline->printTreap(t);

    cout<<"Procedura di split, viene creato un nuovo albero\nVisualizzazione prima della procedura split:\n"; //procedura di split
    int keysSplit[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int prioritySplit[] = { 19, 21, 76, 21, 39, 54, 91, 88, 15};
    int num = sizeof(keysSplit)/sizeof(int);

    // Costruisco un albero su dove invocare la procedura di split
    Treap<int>* treapSplit=new Treap<int>();
    TreapNode<int>* rootSplit = treapSplit->getRoot();
    for(int i=0;i<num;i++){
      treapSplit->insertNode(rootSplit, keysSplit[i], prioritySplit[i]);
    }
    treapSplit->printTreap(rootSplit);
    TreapNode<int> *l = NULL;
    TreapNode<int> *r = NULL;

    treapSplit->split(rootSplit, 5, l, r); //invoco la procedura di split e voglio che si divida utilizzandocome chiave
                                          // di divisione il numero 5
    cout<<"Visualizzazione dopo la procedura split\nAlbero sinistro:\n";  // a questo punto visualizzo sia l'albero sinistro che albero destro
    treapSplit->printTreap(l);
    cout<<"Albero destro:\n";
    treapSplit->printTreap(r);
}
