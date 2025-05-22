#include "main.h"
#include "watek_komunikacyjny.h"

/* wątek komunikacyjny; zajmuje się odbiorem i reakcją na komunikaty */
void *startKomWatek(void *ptr)
{
    MPI_Status status;
    int is_message = FALSE;
    packet_t pakiet;
    
    while (1) {
        MPI_Recv( &pakiet, 1, MPI_PAKIET_T, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        lamport = max(lamport, pakiet.ts);

        switch (status.MPI_TAG) {
            case REQ_JAR:
                enqueue_request(pakiet.ts, pakiet.src);
                // debug("Odebralem REQ_JAR od %d, odsylam OK_JAR", pakiet.src);
                packet_t *pkt = malloc(sizeof(packet_t));
                sendPacket(pkt, pakiet.src, OK_JAR);
            break;

            case OK_JAR:
                pending--;
            break;

            case NEW_JAM:
                available_jams++;
                available_jars--;
                dequeue_request(pakiet.src);
            break;

            case REQ_JAM:
                enqueue_request(pakiet.ts, pakiet.src);
                // debug("Odebralem REQ_JAM od %d, odsylam OK_JAR", pakiet.src);
                packet_t *pakiet_req = malloc(sizeof(packet_t));
                sendPacket(pakiet_req, pakiet.src, OK_JAM);
            break;

            case OK_JAM:
                pending--;
            break;

            case REL_JAR:
                available_jams--;
                available_jars++;
                dequeue_request(pakiet.src);
            break;


            default:
            break;
        }
    }
    
}
