#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nodo_Auto {
    int car[512];
    int size;
}Parcheggio;

typedef struct Nodo_Stazione {
    int distanza;
    int numCar;
    int maxAuto;
    int passi;
    int prevHop;
    Parcheggio* parcheggio;
    struct Nodo_Stazione* next;
    struct Nodo_Stazione* prev;
}Station;

typedef Station* Stazione;

void aggiungiStazione(int distanza, int numCar);
Stazione aggiungiS(int distanza, int numCar);

void demoliscistazione(int distanza);

void aggiungiAuto(int distanza, int autonomia);
void aggiungiA(Stazione current, int autonomia);

void rotamaAuto(int distanza, int autonomia);
void rottamaA( Stazione current, int autonomia );

void pianificaPercorso(int partenza, int arrivo);

void pianificaDiretto(int partenza, int arrivo);
void print(Stazione partenza, Stazione arrivo);

void pianificaInverso(int partenza, int arrivo);
void stampa(Stazione partenza, Stazione fine);


Stazione iniziale = NULL;
Stazione finale = NULL;
int n = 0;

int main() {

    char stringa[20];
    int uno;
    int due;
    int lettura;

    lettura = scanf( "%s", stringa);

    if(lettura > 0) {
        while (!feof(stdin)) {
            if (!strcmp(stringa, "aggiungi-stazione")) {
                lettura = scanf("%s", stringa);
                uno = atoi(stringa);
                lettura = scanf("%s", stringa);
                due= atoi(stringa);
                aggiungiStazione(uno, due);
            } else if (!strcmp(stringa, "demolisci-stazione")) {
                lettura = scanf("%s", stringa);
                uno = atoi(stringa);
                demoliscistazione(uno);
            } else if (!strcmp(stringa, "aggiungi-auto")) {
                lettura = scanf("%s", stringa);
                uno = atoi(stringa);
                lettura = scanf("%s", stringa);
                due = atoi(stringa);
                aggiungiAuto(uno, due);
            } else if (!strcmp(stringa, "rottama-auto")) {
                lettura = scanf("%s", stringa);
                uno = atoi(stringa);
                lettura = scanf("%s", stringa);
                due = atoi(stringa);
                rotamaAuto(uno, due);
            } else if (!strcmp(stringa, "pianifica-percorso")) {
                lettura = scanf("%s", stringa);
                uno = atoi(stringa);
                lettura = scanf("%s", stringa);
                due = atoi(stringa);
                pianificaPercorso(uno, due);
            }
            lettura = scanf("%s", stringa);
        }
    }

    return 0;

}

void aggiungiStazione(int distanza, int numCar) {
    Stazione current = iniziale;
    Stazione new = NULL;

    if(iniziale == NULL) {
        iniziale = aggiungiS(distanza, numCar);
        finale = iniziale;
        printf("aggiunta\n");
        return;
    } else if(distanza <= iniziale->distanza) {
        if(distanza == iniziale->distanza) {
            printf("non aggiunta\n");
            return;
        }
        new = aggiungiS(distanza, numCar);
        iniziale->prev = new;
        new->next = iniziale;
        iniziale = new;
        printf("aggiunta\n");
        return;
    } else if( distanza >= finale->distanza ){
        if(distanza == finale->distanza) {
            printf("non aggiunta\n");
            return;
        }
        new = aggiungiS(distanza, numCar);
        finale->next = new;
        new->prev = finale;
        finale = new;
        printf("aggiunta\n");
        return;
    } else {

        while(current->next != NULL && current->next->distanza < distanza) {
            current = current->next;
        }

        if( current->next == NULL || (current->next != NULL && distanza != current->next->distanza)) {
            new = aggiungiS(distanza, numCar);
            new->next = current->next;
            new->prev = current;
            current->next->prev = new;
            current->next = new;

            printf("aggiunta\n");
            return;
        }
    }
    printf("non aggiunta\n");
    return;
}

Stazione aggiungiS(int distanza, int numCar) {
    Stazione new = NULL;
    char stringa[10];
    int autonomia;
    int lettura;
    int i;

    new = malloc(sizeof(Station));
    new->distanza = distanza;
    new->numCar = numCar;
    new->next = NULL;
    new->prev = NULL;
    new->maxAuto = 0;
    new->passi = 0;
    new->prevHop = 0;
    new->parcheggio = NULL;
    if(numCar > 0) {
        new->parcheggio = malloc(sizeof(int)*512);
        new->parcheggio->size = numCar;
        for( i = 0; i < numCar; i++ ) {
            lettura = scanf( "%s", stringa);
            if(lettura < 0) {

            }
            autonomia = atoi(stringa);
            new->parcheggio->car[i] = autonomia;
            if(autonomia > new->maxAuto) {
                new->maxAuto = autonomia;
            }
        }
    }
    n++;
    return new;
}

void demoliscistazione(int distanza) {
    Stazione current = NULL;
    if(distanza == iniziale->distanza) {
        current = iniziale->next;
        if(current != NULL) {
            current->prev = NULL;
        }
        free(iniziale->parcheggio);
        free(iniziale);
        iniziale = current;
        if(iniziale == NULL) {
            finale = NULL;
        }
        printf("demolita\n");
        n--;
        return;
    } else if( distanza == finale->distanza) {
        current = finale->prev;
        if(current != NULL) {
            current->next = NULL;
        }
        free(finale->parcheggio);
        free(finale);
        finale = current;
        if(finale == NULL) {
            iniziale = NULL;
        }
        printf("demolita\n");
        n--;
        return;
    } else {
        current = iniziale;
        while( current != NULL && current->distanza != distanza ) {
            current = current->next;
        }
        if(current != NULL) {
            current->prev->next = current->next;
            current->next->prev = current->prev;
            if(current->numCar > 0) {
                free(current->parcheggio);
            }
            free(current);
            printf("demolita\n");
            n--;
            return;
        }
    }

    printf("non demolita\n");
    return;
}

void aggiungiAuto(int distanza, int autonomia) {
    Stazione current = NULL;
    if(distanza == iniziale->distanza) {
        aggiungiA(iniziale, autonomia);
        return;
    } else if( distanza == finale->distanza ) {
        aggiungiA(finale, autonomia);
        return;
    } else {
        current = iniziale;
        while( current != NULL && current->distanza != distanza ) {
            current = current->next;
        }
        if(current != NULL) {
            aggiungiA( current, autonomia );
            return;
        }
    }
    printf("non aggiunta\n");
    return;
}

void aggiungiA(Stazione current, int autonomia) {
    if(current->parcheggio == NULL) {
        current->parcheggio = malloc(sizeof(int)*512);
        current->parcheggio->size = 0;
        current->parcheggio->car[0] = autonomia;
        current->parcheggio->size++;
        current->maxAuto = autonomia;
        printf("aggiunta\n");
        return;
    } else {
        current->parcheggio->car[current->parcheggio->size] = autonomia;
        current->parcheggio->size++;
        if(autonomia > current->maxAuto) {
            current->maxAuto = autonomia;
        }
        printf("aggiunta\n");
        return;
    }
}

void rotamaAuto(int distanza, int autonomia) {

    Stazione current = NULL;

    if(distanza == iniziale->distanza) {
        rottamaA(iniziale, autonomia);
        return;
    } else if( distanza == finale->distanza ) {
        rottamaA(finale, autonomia);
        return;
    } else {
        current = iniziale;
        while( current != NULL && current->distanza != distanza) {
            current = current->next;
        }
        if(current != NULL) {
            rottamaA( current, autonomia );
            return;
        }
    }
    printf("non rottamata\n");
    return;
}

void rottamaA( Stazione current, int autonomia ) {

    int trovato;
    int nuovoMax;
    int i, j;

    if( current->parcheggio == NULL ) {
        printf("non rottamata\n");
        return;
    } else {
        trovato = 0;
        for( i = 0; i < current->parcheggio->size && trovato == 0; i++ ) {
            if( current->parcheggio->car[i] == autonomia ) {
                trovato = 1;
                for( j = i; j < current->parcheggio->size-1; j++) {
                    current->parcheggio->car[j] = current->parcheggio->car[j+1];
                }
                current->parcheggio->size--;
                current->numCar--;
            }
        }

        if(trovato == 1) {
            if(current->numCar == 0) {
                free(current->parcheggio);
                current->maxAuto = 0;
                printf("rottamata\n");
                return;
            } else {
                nuovoMax = current->parcheggio->car[0];
                for( int i = 1; i < current->parcheggio->size; i++ ) {
                    if(nuovoMax < current->parcheggio->car[i]) {
                        nuovoMax = current->parcheggio->car[i];
                    }
                }
                current->maxAuto = nuovoMax;
                printf("rottamata\n");
                return;
            }
        }
    }

    printf("non rottamata\n");
    return;
}

void pianificaPercorso(int partenza, int arrivo) {
    if(partenza < arrivo) {
        pianificaDiretto( partenza, arrivo);
    } else {
        pianificaInverso( partenza, arrivo);
    }
}

void pianificaDiretto(int partenza, int arrivo) {

    Stazione lento = iniziale;
    Stazione veloce = NULL;
    Stazione stazPartenza = NULL;
    Stazione stazArrivo = NULL;
    Stazione stazP = NULL;
    int passi;

    while(lento->distanza != partenza) {
        lento = lento->next;
    }
    stazPartenza = lento;
    stazP = lento;

    while(stazP->distanza != arrivo) {
        stazP->passi = 0;
        stazP->prevHop = -1;
        stazP = stazP->next;
    }

    if(stazP != NULL) {
        stazP->passi = 0;
        stazP->prevHop = -1;
    }

    stazArrivo = stazP;
    veloce = lento->next;

    while( lento != NULL && veloce != NULL ) {
        veloce = lento->next;
        while(veloce != NULL && (veloce->distanza <= (lento->distanza + lento->maxAuto))) {
            passi = (lento->passi)+1;
            if(veloce->passi == 0) {
                veloce->passi = passi;
                veloce->prevHop = lento->distanza;
            }
            veloce = veloce->next;
        }
        lento = lento->next;
    }

    print(stazPartenza, stazArrivo);

}

void print(Stazione partenza, Stazione arrivo) {
    
    Stazione current = arrivo;
    int array[n];
    int i = 0;
    int distanza;

    while( current != partenza ) {
        distanza = current->prevHop;
        if( distanza == -1 ) {
            printf("nessun percorso\n");
            return;
        }
        array[i] = distanza;
        i++;
        while(current->distanza != distanza) {
            current = current->prev;
        }
    }

    for( int j = i-1; j >= 0; j-- ) {
        printf("%d ", array[j]);
    }
    printf("%d\n", arrivo->distanza);

}

void pianificaInverso(int partenza, int arrivo) {
    Stazione lento = iniziale;
    Stazione veloce = NULL;
    Stazione stazPartenza = NULL;
    Stazione stazArrivo = NULL;
    Stazione stazP = NULL;
    int passi;

    while(lento->distanza != partenza) {
        lento = lento->next;
    }
    stazPartenza = lento;
    stazP = lento;

    while(stazP->distanza != arrivo) {
        stazP->passi = 0;
        stazP->prevHop = -1;
        stazP = stazP->prev;
    }

    if(stazP != NULL) {
        stazP->passi = 0;
        stazP->prevHop = -1;
    }

    stazArrivo = stazP;

    while(lento != NULL) {
        veloce = lento->prev;
        while(veloce != NULL && (veloce->distanza >= (lento->distanza - lento->maxAuto))) {
            passi = (lento->passi)+1;
            if(veloce->passi == 0) {
                veloce->passi = passi;
                veloce->prevHop = lento->distanza;
            } else if(veloce->passi != 0 && passi == veloce->passi) {
                veloce->prevHop = lento->distanza;
            }
            veloce = veloce->prev;
        }
        lento = lento->prev;
    }

    stampa(stazPartenza, stazArrivo);

}

void stampa(Stazione partenza, Stazione arrivo) {

    Stazione current = arrivo;
    int array[n];
    int i = 0;
    int distanza;

    while( current != partenza ) {
        distanza = current->prevHop;
        if( distanza == -1 ) {
            printf("nessun percorso\n");
            return;
        }
        array[i] = distanza;
        i++;
        while(current->distanza != distanza) {
            current = current->next;
        }
    }

    for( int j = i-1; j >= 0; j-- ) {
        printf("%d ", array[j]);
    }
    printf("%d\n", arrivo->distanza);

}