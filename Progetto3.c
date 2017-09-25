/*Progetto realizzato da: Fabrizio Siciliano
Algoritmi per l'informatica
Matricola 843770
Codice Persona 10522031*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define X (255*255)+300
typedef struct albero{
	struct albero *right, *left, *padre;
	char path[X];
}tree;
typedef struct node{
	struct node *testa, *padre, *left, *right;
	char *file; 
	int figli; 
	int check;
	char nome[255];
}nodo;
void crea(char *, nodo *, int);
void write(char *,char *, nodo *);
int insert(nodo*, char*, int);
void read(char *, nodo *);
void delete(char *, nodo *);
void delete_r(char *, nodo *);
tree* find(nodo*, char*, char*,tree*);
nodo *move(nodo*, char *);
nodo* cancella(nodo *, nodo *);
void cancella_ricorsiva(nodo *);
void inorder_tree_walk(tree *);
tree* tree_insert(tree *, tree*);
nodo* tree_minimum(nodo*);
nodo* tree_successor(nodo*);

void stampa(nodo *head){
	if(head==NULL)
		return;
	printf("%s\n", head->nome);
	printf("%d\n",head->check );
	stampa(head->left);
	stampa(head->right);
	if(head->check==0)
		stampa(head->testa);
}
int main (){
	char comando[11], par1[X], par2[65820];
	int i=0;
	//inizializzazione dell'elemento root del filesystem
	nodo *head;
	head = (nodo*) malloc(sizeof(nodo));
	head->padre = NULL; head->testa = NULL; head->left = NULL; head->right = NULL; head->file = NULL; head->figli = 0; head->check = 0;
	//ciclo infinito per permettere l'inserimento di più comandi
	while(1){
		//verifica della scanf corretta
		if(scanf("%s", comando)==1){
			//if-else per capire quale comando è stato inserito
			if (strcmp(comando,"create_dir")==0){
				//verifica della corretta scansione in input del parametro 1
				if(scanf("%s", par1)==1){
					//chiamo la funzione crea con il percorso completo, la root del FS e il bit di controllo a zero (creo una directory)
					crea(par1, head, 0);
					printf("\n");
				}
				else
					printf("no\n");
			}
			else if (strcmp(comando,"create")==0){
				//verifica della corretta scansione in input del parametro 1
				if(scanf("%s", par1)==1){
					//chiamo la funzione crea con il percorso completo, la root del FS e il bit di controllo a uno (creo un file)
					crea(par1, head, 1);
					printf("\n");
				}
				else
					printf("no\n");
			}
			else if (strcmp(comando,"write")==0){
				//verifica della corretta scansione in input dei parametri 1 e 2
				if(scanf("%s", par1)==1 && scanf("%[^\n]", par2)==1){
					write(par1, par2,head);
					printf("\n");
				}
				else
					printf("no\n");
			}
			else if (strcmp(comando,"find")==0){
				//verifica della corretta scansione in input del parametro 1
				if(scanf("%s", par1)==1){
					//inizializzo la testa dell'albero binario che utilizzerò per l'ordinamento lessicografico dei percorsi eventualmente trovati
					tree* ALBERO = NULL;
					//inizializzo il percorso come stringa vuota
					char *path = (char*)malloc(sizeof(char));
					strcpy(path,"\0");
					//chiamo la find sul primo elemento del sottoalbero della root
					ALBERO = find(head->testa, par1, path, ALBERO);
					//libero la stringa vuota
					free(path);
					//se l'albero è vuoto, torna no
					if(ALBERO==NULL)
						printf("no\n");
					//altrimenti richiama l'algoritmo di stampa inordine
					else
						inorder_tree_walk(ALBERO);
				}
				else
					printf("no\n");
			}
			else if (strcmp(comando,"read")==0){
				//verifica della corretta scansione in input del parametro 1
				if(scanf("%s", par1)==1){
					read(par1, head);
					printf("\n");
				}
				else
					printf("no\n");
			}
			else if (strcmp(comando,"delete")==0){
				//verifica della corretta scansione in input del parametro 1
				if(scanf("%s", par1)==1){
					delete(par1, head);
					printf("\n");
				}
				else
					printf("no\n");
			}
			else if (strcmp(comando,"delete_r")==0){
				//verifica della corretta scansione in input del parametro 1
				if(scanf("%s", par1)==1){
					delete_r(par1,head);
					printf("\n");
				}
				else
					printf("no\n");
			}
			else if (strcmp(comando, "stampa")==0)
				stampa(head->testa);
			else if (strcmp(comando, "exit")==0){
				//libero l'elemento iniziale del filesystem e termino il programma
				free(head);
				return 0;
			}
		}
	}
	return 0;
}
void crea(char *path, nodo *head, int directory){
	nodo *ptr, *newnodo, *father; int i=0, controllo = 0; char *percorso[255], *p; 
	ptr = head;
	//se non comincia con / allora non fare nulla
	if(path[0]!='/'){ printf("no"); return;}
	//dividi il percorso in un array di stringhe
	for (p = strtok(path, "/"); p; p = strtok(NULL, "/")){
		if(i<=255){
			percorso[i]=(char*)malloc(strlen(p)*sizeof(char));
			strcpy(percorso[i], p);
			i++;
		}
		else{
			printf("no");
			return;
		}
	}
	//setta il percorso i+1 a NULL
	percorso[i]=NULL;
	//cicla sull'array appena creato
	for(i=0; percorso[i]!=NULL; i++){
		//sono alla fine dell'array
		if(percorso[i+1]==NULL){
			//in ptr sta adesso l'eventuale cartella in cui si dovrebbe trovare la posizione in cui inserire la cartella o il file!
			if(ptr->figli<1024 && ptr->check==0){
				//salvo il valore di ptr, che dopo si sposta forse
				father = ptr;
				//directory indica 0 se devo creare una cartella, 1 se un file
				controllo = insert(ptr, percorso[i], directory);
				//se l'inserimento è positivo (non ci sono risorse con lo stesso nome) aumenta il contatore dei figli del padre
				if(controllo == 1){
					father->figli +=1;
					printf("ok");
					return;
				}
				else{
					printf("no");
					return;
				}
			}
			//se ci sono troppi figli o sto tentando di inserire all'interno di un file
			else{
				printf("no");
				return;
			}
		}
		//sto ciclando all'interno del percorso
		else{
			//sto all'interno di una cartella
			if(ptr->check==0)
				ptr = move(ptr->testa, percorso[i]);
			else{
				printf("no");
				return;
			}
			//se la cartella corrispondente non è stata trovata, errore
			if(ptr==NULL){
				printf("no");
				return;
			}
		}
	}
}
//algoritmo search in BT slide di Marcello Maria Bersani
nodo *move(nodo*x, char *stringa){
	if(x==NULL)
		return x;
	if(strcmp(x->nome,stringa)==0)
		return x;
	if(strcmp(x->nome,stringa)<0)
		return move(x->left, stringa);
	else
		return move(x->right, stringa);
}
//algoritmo insert in BT slide di Marcello Maria Bersani leggermente modificato per permettere la creazione sia di file che subdir
int insert(nodo*T, char *key, int directory){
	nodo *y = NULL, *x=T->testa,*z;
	while(x!=NULL){
		y=x;
		if(strcmp(x->nome,key)<0)
			x = x->left;
		else if(strcmp(x->nome,key)>0)
			x = x->right;
		else if(strcmp(x->nome,key)==0)
			return 0;
	}
	//inizio parte aggiunta per permettere il passaggio della sola key da confrontare (dovrebbe essere l'ultimo elemento dell'array nella funzione crea)
	z = (nodo*)malloc(sizeof(nodo));
	z->padre = y;
	z->testa = NULL; z->right = NULL; z->left = NULL; 
	z->figli = 0;
	strcpy(z->nome, key);
	z->check = directory;
	z->file = NULL;
	//fine parte aggiunta
	if(y==NULL)
		T->testa = z;
	else if(strcmp(y->nome, z->nome)<0)
		y->left = z;
	else 
		y->right = z;
	return 1;
}
void write(char *path,char *string, nodo * head){
	nodo *ptr; int i=0, len; char *percorso[255], *p;
	ptr = head;
	//se non comincia con / allora non fare nulla
	if(path[0]!='/'){ printf("no"); return;}
	//dividi il percorso in un array di stringhe
	for (p = strtok(path, "/"); p; p = strtok(NULL, "/")){
		if(i<=255){
			percorso[i]=(char*)malloc(strlen(p)*sizeof(char));
			strcpy(percorso[i], p);
			i++;
		}
		else{
			printf("no");
			return;
		}
	}
	//setta il percorso i+1 a NULL
	percorso[i]=NULL;
	//elimina le virgolette dal contenuto del file
	string = strtok(string, "\"");
	string = strtok(NULL, "\"");
	//se il contenuto del file è !=NULL, esegui l'assegnamento normale, sennò len=0
	if(string){
		len = strlen(string);
	}
	else{
		len=0;
	}
	//cicla sull'array prima creato
	for(i=0; percorso[i]!=NULL; i++){
		//sono alla fine dell'array
		if(percorso[i+1]==NULL){
			//se la risorsa al cui interno sta il file è una cartella, esegui
			if(ptr->testa!=NULL && ptr->check==0)
				ptr = ptr->testa;
			else{printf("no");return;}
			//cicla all'interno della cartella per arrivare all'elemento che ti interessa
			while(ptr!=NULL){
				//vai a sinistra
				if(strcmp(ptr->nome, percorso[i])<0){
					ptr = ptr->left;
				}
				//vai a destra
				else if(strcmp(ptr->nome, percorso[i])>0){
					ptr = ptr->right;
				}
				//hai trovato la risorsa
				else if(strcmp(ptr->nome, percorso[i])==0){
					//la risorsa è file?
					if(ptr->check==1){
						//il contenuto da scrivere è diverso da NULL?
						if(string!=NULL){
							if(ptr->file!=NULL)
								free(ptr->file);
							ptr->file = (char*)malloc(sizeof(char)*strlen(string));
							strcpy(ptr->file, string);
						}
						else{
							if(ptr->file!=NULL)
								free(ptr->file);
							ptr->file = (char*)malloc(sizeof(char));
							strcpy(ptr->file, "\0");
						}
						//stampa il numero di caratteri scritti nel file
						printf("ok %d", len);
						return;
					}
					//se la risorsa è una cartella, non fare nulla ma dai errore
					else{
						printf("no");
						return;
					}
				}
			}
			//se sei uscito dal ciclo vuol dire che non hai trovato l'elemento che stavi cercando
			printf("no");
			return ;
		}
		//sto ciclando all'interno del percorso
		else{
			//sto all'interno di una cartella
			if(ptr->check==0)
				ptr = move(ptr->testa, percorso[i]);
			else{
				printf("no");
				return;
			}
			//se la cartella corrispondente non è stata trovata, errore
			if(ptr==NULL){
				printf("no");
				return;
			}
		}
	}
}
void read(char *path, nodo *head){
	nodo *ptr; char *percorso[255], *p; int check=0, i = 0;
	ptr = head; 
	//se non comincia con / allora non fare nulla
	if(path[0]!='/'){ printf("no"); return;}
	//dividi il percorso in un array di stringhe
	for (p = strtok(path, "/"); p; p = strtok(NULL, "/")){
		if(i<=255){
			percorso[i]=(char*)malloc(strlen(p)*sizeof(char));
			strcpy(percorso[i], p);
			i++;
		}
		else{
			printf("no");
			return;
		}
	}
	//setta il percorso i+1 a NULL
	percorso[i]=NULL;
	//cicla sull'array appena creato
	for(i=0; percorso[i]!=NULL; i++){
		//sono alla fine del percorso
		if(percorso[i+1]==NULL){
			//sono all'interno di una directory non vuota
			if(ptr->testa!=NULL && ptr->check==0)
				ptr = ptr->testa;
			else{printf("no");return;}
			//ricerca all'interno della directory il file
			while(ptr!=NULL){
				//vai a sinistra
				if(strcmp(ptr->nome, percorso[i])<0)
					ptr = ptr->left;
				//vai a destra
				else if(strcmp(ptr->nome, percorso[i])>0)
					ptr = ptr->right;
				//hai trovato la risorsa giusta
				else if(strcmp(ptr->nome, percorso[i])==0){
					//la risorsa trovata è un file
					if(ptr->check==1){
						//stampa il contenuto del file
						if(ptr->file!=NULL)
							printf("contenuto %s", ptr->file);
						else
							printf("contenuto ");
						return;
					}
					//la risorsa trovata non è file ma directory
					else{
						printf("no");
						return;
					}
				}
			}
			//se sei uscito dal ciclo, non hai trovato la risorsa che ti serviva all'interno della cartella
			printf("no");
			return;
		}
		//sto ciclando all'interno del percorso
		else{
			//sto all'interno di una cartella
			if(ptr->check==0)
				ptr = move(ptr->testa, percorso[i]);
			else{
				printf("no");
				return;
			}
			//se la cartella corrispondente non è stata trovata, errore
			if(ptr==NULL){
				printf("no");
				return;
			}
		}
	}
}
void delete(char *path, nodo *head){
	nodo *ptr, *father, *elim; char *percorso[255], *p; int i=0;
	ptr = head;
	//se non comincia con / allora non fare nulla
	if(path[0]!='/'){ printf("no"); return;}
	//dividi il percorso in un array di stringhe
	for (p = strtok(path, "/"); p; p = strtok(NULL, "/")){
		if(i<=255){
			percorso[i]=(char*)malloc(strlen(p)*sizeof(char));
			strcpy(percorso[i], p);
			i++;
		}
		else{
			printf("no");
			return;
		}
	}
	//setta il percorso i+1 a NULL
	percorso[i]=NULL;
	//cicla sull'array appena creato
	for(i=0;percorso[i]!=NULL;i++){
		//sono alla fine del percorso
		if(percorso[i+1]==NULL){
			//salvo ptr per sapere da quale nodo togliere l'eventuale figlio cancellato
			father = ptr;
			//se la risorsa su cui sto è una directory ed ha dei figli, va bene, altrimenti esci
			if(ptr->testa!=NULL && ptr->check==0)
				ptr = ptr->testa;
			else{printf("no");return;}
			//cicla tra i figli della cartella 
			while(ptr!=NULL){
				//vai a sinistra
				if(strcmp(ptr->nome, percorso[i])<0)
					ptr = ptr->left;
				//vai a destra
				else if(strcmp(ptr->nome, percorso[i])>0)
					ptr = ptr->right;
				//hai trovato la tua risorsa
				else{
					//se la risorsa non ha figli, elimina, altrimenti errore
					if(ptr->testa==NULL && ptr->figli==0)
						//ritorna l'elemento da tornare una volta passata la testa dell'albero e l'elemento da eliminare, fa il fixup della delete(vedi cancella)
						elim = cancella(father, ptr);
					else{
						printf("no");
						return;
					}
					if(/*elim==NULL || */elim->testa !=NULL){		//elim non può mai essere NULL e elim->testa verifico che sia ==NULL già quando chiamo la elim=cancella, RIDONDANTE(?)
						printf("no");
						return;
					}
					else{
						//se è un file, dealloco la memoria anche del file dinamico
						if(elim->file!=NULL)
							free(elim->file);
						//tolgo un figlio al padre
						father->figli -= 1;
						//dealloco la memoria dell'elemento da eliminare
						free(elim);
						printf("ok");
						return;
					}
				}
			}
			//se sei uscito dal ciclo vuol dire che non hai trovato la risorsa richiesta
			printf("no");
			return;
		}
		//sto ciclando all'interno del percorso
		else{
			//sto all'interno di una cartella
			if(ptr->check==0)
				ptr = move(ptr->testa, percorso[i]);
			else{
				printf("no");
				return;
			}
			//se la cartella corrispondente non è stata trovata, errore
			if(ptr==NULL){
				printf("no");
				return;
			}
		}
	}
}
//algoritmo delete in BT slide di Marcello Maria Bersani
nodo* cancella(nodo *T, nodo *z){
	nodo *x=NULL, *y=NULL, *ptr = T;
	if(z->left==NULL || z->right == NULL)
		y = z;
	else
		y = tree_successor(z);
	if(y->left != NULL)
		x = y->left;
	else x = y->right;
	if(x!=NULL)
		x->padre = y->padre;
	if(y->padre == NULL)
		T->testa = x;
	else if(y == y->padre->left)
		y->padre->left = x;
	else
		y->padre->right = x;
	if(strcmp(y->nome, z->nome)!=0){
		strcpy(z->nome, y->nome);
		z->testa = y->testa;
		z->check = y->check;
		z->figli = y->figli;
		z->file = y->file;
	}
	return y;
}
//algoritmo tree_successor in BT slide di Marcello Maria Bersani
nodo *tree_successor(nodo*x){
	nodo *y=NULL;
	if(x->right != NULL)
		return tree_minimum(x->right);
	y = x->padre;
	while(y!=NULL && strcmp(x->nome, y->right->nome)==0){
		x = y;
		y = y->padre;
	}
	return y;
}
//algoritmo tree_minimum in BT slide di Marcello Maria Bersani
nodo* tree_minimum(nodo*x){
	while(x->left!=NULL)
		x = x->left;
	return x;
}
void delete_r(char *path, nodo *head){
	char *percorso[255], *p; nodo *ptr = head, *father, *elim; int i=0;
	//se non comincia con / allora non fare nulla
	if(path[0]!='/'){ printf("no"); return;}
	//dividi il percorso in un array di stringhe
	for (p = strtok(path, "/"); p; p = strtok(NULL, "/")){
		if(i<=255){
			percorso[i]=(char*)malloc(strlen(p)*sizeof(char));
			strcpy(percorso[i], p);
			i++;
		}
		else{
			printf("no");
			return;
		}
	}
	//setta il percorso i+1 a NULL
	percorso[i]=NULL;
	//cicla sull'array appena creato
	for(i=0; percorso[i]!=NULL; i++){
		//sono arrivato alla fine dell'array, percorso[i] è l'elemento da eliminare
		if(percorso[i+1]==NULL){
			//salva il ptr per capire a quale nodo scalare il contatore figli
			father = ptr;
			//ptr è una cartella? posso entrarci e cercare l'elemento?
			if(ptr->testa!=NULL && ptr->check==0)
				ptr = ptr->testa;
			else{
				printf("no"); return;
			}
			//cerca l'elemento all'interno della cartella
			while(ptr!=NULL){
				//hai trovato l'elemento corretto
				if(strcmp(ptr->nome, percorso[i])==0){
					//passa i figli dell'elemento da eliminare e cancellali tutti
					cancella_ricorsiva(ptr->testa);
					//per sicurezza poni i figli a NULL
					ptr->testa = NULL;
					//cicla nuovamente all'interno del padre per il fixup del sottoalbero del padre
					elim = cancella(father, ptr);
					//scala il numero di figli del padre
					father->figli -= 1;
					//la risorsa è un file, dealloca lo spazio del file
					if(elim->file!=NULL)
						free(elim->file);
					//dealloca lo spazio dell'elemento
					free(elim);
					printf("ok");
					return;
				}
				//vai a sinistra
				else if(strcmp(ptr->nome, percorso[i])<0)
					ptr = ptr->left;
				//vai a destra
				else if(strcmp(ptr->nome, percorso[i])>0)
					ptr = ptr->right;
			}
			//se sei uscito dal ciclo non hai trovato la tua risorsa da cancellare, quindi errore
			printf("no");
			return;
		}
		//sto ciclando all'interno del percorso
		else{
			//sto all'interno di una cartella
			if(ptr->check==0)
				ptr = move(ptr->testa, percorso[i]);
			else{
				printf("no");
				return;
			}
			//se la cartella corrispondente non è stata trovata, errore
			if(ptr==NULL){
				printf("no");
				return;
			}
		}
	}
}
void cancella_ricorsiva(nodo *x){
	//se l'elemento è NULL, non fare nulla
	if(x==NULL)
		return;
	//richiama ricorsivamente su tutti i puntatori disponibili
	cancella_ricorsiva(x->right);
	cancella_ricorsiva(x->left);
	cancella_ricorsiva(x->testa);
	//se l'elemento è un file, dealloca lo spazio del file
	if(x->check==1)
		free(x->file);
	//dealloca lo spazio dell'elemento
	free(x);
	return;
}
tree* find(nodo *head, char* risorsa, char*percorso, tree*R){
	nodo *ptr = head; tree *new = NULL;
	//se ptr è NULL, ritorna la testa dell'albero R, è l'albero binario creato esclusivamente per l'ordinamento lessicografico dei percorsi eventualmente trovati. R inizializzato a NULL
	if(ptr == NULL)
		return R;
	//scorri la funzione sui figli destri e sinistri dell'albero che stai analizzando
	R = find(ptr->left, risorsa, percorso, R);
	R = find(ptr->right, risorsa, percorso, R);
	//se hai trovato l'elemento, inserisci nell'albero
	if(strcmp(ptr->nome, risorsa)==0){
		//alloca un nuovo elemento dell'albero binario
		new = (tree*)malloc(sizeof(tree));
		//inserisci nel path il percorso finora trovato e il nome della risorsa, con un / in mezzo
		strcpy(new->path, percorso); strcat(new->path, "/"); strcat(new->path, ptr->nome);
		//inizializzazioni di rito
		new->left = NULL; new->padre = NULL; new->right = NULL;
		//inserisci nell'albero binario
		R = tree_insert(R, new);
	}
	//la risorsa è anche una cartella
	if(ptr->check==0){
		//alloco un nuovo percorso
		char *newpercorso =(char*)malloc(sizeof(char)*(strlen(percorso)+strlen(ptr->nome)+10));
		//il nuovo percorso è formato dal vecchio percorso + il nome della nuova directory separati da un /
		strcpy(newpercorso, percorso); strcat(newpercorso, "/"); strcat(newpercorso, ptr->nome);
		//richiama find sui figli della cartella trovata
		R = find(ptr->testa, risorsa, newpercorso, R);
		//dealloco lo spazio del nuovo percorso perchè non mi serve più
		free(newpercorso);
	}
	return R;
}
//algoritmo insert in BT slide di Marcello Maria Bersani (crea un albero per ordinare tutti i percorsi degli elementi trovati dalla find, viene cancellato man mano che viene stampato dalla inorder_tree_walk sotto)
tree* tree_insert(tree* T, tree* z){
    tree* y=NULL, *x=T;
    while(x!=NULL) {
        y=x;
        if(strcmp(z->path,x->path)<0)
            x=x->left;
        else
            x=x->right;
    }
    z->padre=y;
    if(y==NULL)
        T=z;
    else{
        if(strcmp(z->path, y->path)<0)
            y->left=z;
        else
            y->right=z;
    }
    return T;
}
//algoritmo di cammino in ordine per la stampa dei percorsi in ordine lessicografico dalle slide di Marcello Maria Bersani
void inorder_tree_walk(tree *x){
	if(x){
		inorder_tree_walk(x->left);
		printf("ok %s\n",x->path);
		inorder_tree_walk(x->right);
		//free(x->path);
		free(x);
	}
}