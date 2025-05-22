#ifndef MAIN_H
#define MAIN_H

#include <stddef.h> 
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define B 4
#define S 4

#define P 2
#define K 10

#define DEBUG 

extern int available_jams;
extern int available_jars;

extern int rank;
extern int size;
extern int lamport;
extern int queue_size;
extern int pending;

extern char *role;

#define MAX_Q_SIZE 100

typedef struct {
    int timestamp;
    int rank;
} Request;

typedef struct {
    int ts;       /* timestamp (zegar lamporta */
    int src;      /* pole nie przesyłane, ale ustawiane w main_loop */

    int data; 
} packet_t;
extern MPI_Datatype MPI_PAKIET_T;


#ifdef DEBUG
#define debug(FORMAT,...) printf("%c[%d;%dm [%s] [%d] [%d]:\t" FORMAT "%c[%d;%dm\n",  27, (1+(rank/7))%2, 31+(6+rank)%7, role,rank, lamport, ##__VA_ARGS__, 27,0,37);
#else
#define debug(...) ;
#endif

#define FALSE 0
#define TRUE 1

#define P_WHITE printf("%c[%d;%dm",27,1,37);
#define P_BLACK printf("%c[%d;%dm",27,1,30);
#define P_RED printf("%c[%d;%dm",27,1,31);
#define P_GREEN printf("%c[%d;%dm",27,1,33);
#define P_BLUE printf("%c[%d;%dm",27,1,34);
#define P_MAGENTA printf("%c[%d;%dm",27,1,35);
#define P_CYAN printf("%c[%d;%d;%dm",27,1,36);
#define P_SET(X) printf("%c[%d;%dm",27,1,31+(6+X)%7);
#define P_CLR printf("%c[%d;%dm",27,0,37);

/* printf ale z kolorkami i automatycznym wyświetlaniem RANK. Patrz debug wyżej po szczegóły, jak działa ustawianie kolorków */
#define println(FORMAT, ...) printf("%c[%d;%dm [%d]: " FORMAT "%c[%d;%dm\n",  27, (1+(rank/7))%2, 31+(6+rank)%7, rank, ##__VA_ARGS__, 27,0,37);

void sendPacket(packet_t *pkt, int destination, int tag);
void sendToAll(packet_t *pkt, int tag, int type);
void randomSleep(int min_seconds, int max_seconds);

//kolejki
int compare_requests(const void *a, const void *b);
void enqueue_request(int timestamp, int rank);
void dequeue_request(int rank);
int is_in_top(int rank, int limit);

#define max(a, b) ((a) > (b) ? (a) : (b))

#define STUDENTKA 1
#define BABCIA 0

#define REQ_JAR 1
#define OK_JAR 2
#define NEW_JAM 3
#define REL_JAR 4
#define REQ_JAM 5
#define OK_JAM 6

#endif