#include "main.h"
#include "studentka.h"

void *studentka() {
    while (1) {
        //losowy czas nie robi nic
        randomSleep(1, 10);

        //chce zjesc słoik - wysyla REQ_JAM do wszystkich studentek
        debug("Chcę zjeść konfiture.");

        //dodaj sie do kolejki
        enqueue_request(lamport, rank);

        //ile ok potrzeba
        pending = S - 1;

		packet_t *pkt = malloc(sizeof(packet_t));
        sendToAll(pkt, REQ_JAM, STUDENTKA);

        while (!moze_wejsc_studentka()) {
            usleep(1000);
        }

        debug("Wchodze do sekcji krytycznej");

        // - sekcja krytyczna - //
        
        randomSleep(1, 10); // losowy czas robienia sloika
        
        available_jams--;

        packet_t *new = malloc(sizeof(packet_t));

        dequeue_request(rank); // usuwanie siebie z kolejki
        sendToAll(new, REL_JAR, BABCIA); // REL_JAR do babć 
        sendToAll(new, REL_JAR, STUDENTKA); // REL_JAR do studentek 

        debug("Wychodze z sekcji krytycznej");
    }
}

int moze_wejsc_studentka() {
    if (pending > 0) return FALSE;
    if (!is_in_top(rank, available_jams)) return FALSE;

    return TRUE;
}