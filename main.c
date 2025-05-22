
#include "main.h"
#include "babcia.h"
#include "studentka.h"
#include "watek_komunikacyjny.h"
#include <pthread.h>


int available_jams = 0;
int available_jars = P;
char *role;
int size,rank;
int lamport = 0, pending = 0, queue_size = 0;
Request queue[MAX_Q_SIZE];

MPI_Datatype MPI_PAKIET_T;
pthread_t threadKom;

void check_thread_support(int provided)
{
    printf("THREAD SUPPORT: chcemy %d. Co otrzymamy?\n", provided);
    switch (provided) {
        case MPI_THREAD_SINGLE: 
            printf("Brak wsparcia dla wątków, kończę\n");
            /* Nie ma co, trzeba wychodzić */
	    fprintf(stderr, "Brak wystarczającego wsparcia dla wątków - wychodzę!\n");
	    MPI_Finalize();
	    exit(-1);
	    break;
        case MPI_THREAD_FUNNELED: 
            printf("tylko te wątki, ktore wykonaly mpi_init_thread mogą wykonać wołania do biblioteki mpi\n");
	    break;
        case MPI_THREAD_SERIALIZED: 
            /* Potrzebne zamki wokół wywołań biblioteki MPI */
            printf("tylko jeden watek naraz może wykonać wołania do biblioteki MPI\n");
	    break;
        case MPI_THREAD_MULTIPLE: printf("Pełne wsparcie dla wątków\n"); /* tego chcemy. Wszystkie inne powodują problemy */
	    break;
        default: printf("Nikt nic nie wie\n");
    }
}

void randomSleep(int min_seconds, int max_seconds) {
    if (min_seconds > max_seconds) {
        int temp = min_seconds;
        min_seconds = max_seconds;
        max_seconds = temp;
    }

    srand(time(NULL) + rank);
    int sleep_time = min_seconds + rand() % (max_seconds - min_seconds + 1);
    sleep(sleep_time);
}

void inicjuj(int *argc, char ***argv)
{

    int provided;
    MPI_Init_thread(argc, argv,MPI_THREAD_MULTIPLE, &provided);
    // check_thread_support(provided);

    const int nitems=3; /* bo packet_t ma trzy pola */
    int       blocklengths[3] = {1,1,1};
    MPI_Datatype typy[3] = {MPI_INT, MPI_INT, MPI_INT};

    MPI_Aint     offsets[3]; 
    offsets[0] = offsetof(packet_t, ts);
    offsets[1] = offsetof(packet_t, src);
    offsets[2] = offsetof(packet_t, data);

    MPI_Type_create_struct(nitems, blocklengths, offsets, typy, &MPI_PAKIET_T);
    MPI_Type_commit(&MPI_PAKIET_T);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    srand(rank);

    pthread_create( &threadKom, NULL, startKomWatek , 0);
}

void sendPacket(packet_t *pkt, int destination, int tag)
{
    lamport++;
    int freepkt=0;
    if (pkt==0) { pkt = malloc(sizeof(packet_t)); freepkt=1;}
    pkt->src = rank;
    pkt->ts = lamport;
    MPI_Send( pkt, 1, MPI_PAKIET_T, destination, tag, MPI_COMM_WORLD);
    if (freepkt) free(pkt);
}

void sendToAll(packet_t *pkt, int tag, int type) {
    for (int i = (type == BABCIA ? 0 : B); i < (type == BABCIA ? B : size); i++) {
        if (i == rank) continue;
        sendPacket(pkt, i, tag);
    }
}

int compare_requests(const void *a, const void *b) {
    Request *ra = (Request *)a;
    Request *rb = (Request *)b;
    
    if (ra->timestamp != rb->timestamp)
        return ra->timestamp - rb->timestamp;
    return ra->rank - rb->rank;
}

void enqueue_request(int timestamp, int rank) {
    if (queue_size >= MAX_Q_SIZE) {
        fprintf(stderr, "Queue overflow!\n");
        return;
    }

    queue[queue_size].timestamp = timestamp;
    queue[queue_size].rank = rank;
    queue_size++;

    qsort(queue, queue_size, sizeof(Request), compare_requests);
}

void dequeue_request(int rank) {
    int found = 0;
    for (int i = 0; i < queue_size; i++) {
        if (queue[i].rank == rank) {
            found = 1;
        }
        if (found && i < queue_size - 1) {
            queue[i] = queue[i + 1]; // shift left
        }
    }
    if (found) queue_size--;
}

int is_in_top(int rank, int limit) {
    for (int i = 0; i < queue_size && i < limit; i++) {
        if (queue[i].rank == rank)
            return 1;
    }
    return 0;
}

int main(int argc, char** argv) {
    inicjuj(&argc,&argv);
    sleep(rank);
    
    if (rank < B) {
        role = "BABCIA";
        babcia();
    } else {
        role = "STUDEN";
        studentka();
    }
}
