#include "main.h"
#include "babcia.h"

void *babcia() {
    while (1) {
        //losowy czas nie robi nic
        randomSleep(1, 10);

        //chce zrobić słoik - wysyla REQ_JAR do wszystkich
        debug("Chcę zrobić słoik.");

        //dodaj sie do kolejki
        enqueue_request(lamport, rank);

        //ile ok potrzeba
        pending = B - 1;

		packet_t *pkt = malloc(sizeof(packet_t));
        sendToAll(pkt, REQ_JAR, BABCIA);

        while (!moze_wejsc()) {
            usleep(1000);
        }

        debug("Wchodze do sekcji krytycznej");

        // - sekcja krytyczna - //
        
        randomSleep(1, 10); // losowy czas robienia sloika
        
        available_jams++;
        available_jars--;

        packet_t *new = malloc(sizeof(packet_t));

        dequeue_request(rank); // usuwanie siebie z kolejki
        sendToAll(new, NEW_JAM, BABCIA); // NEW_JAM do babci 
        sendToAll(new, NEW_JAM, STUDENTKA); // NEW_JAM do studentek 

        debug("Wychodze z sekcji krytycznej");
    }
}

int moze_wejsc() {
    if (pending > 0) return FALSE;
    if (!is_in_top(rank, available_jars)) return FALSE;
    // if (available_jars < 1) return FALSE;
    if (available_jams >= K) return FALSE;

    return TRUE;
}