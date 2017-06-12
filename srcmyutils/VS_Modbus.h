#include <gtk/gtk.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SHM_SIZE 128 /**< 128 bytes for shared memory */
#define MAXCHARS 4096   //just a large buffer for data chunks
#define MAXMSG 20

#define min(a,b) ( (a) > (b) ? (a) : (b) )
#define max(a,b) ( (a) > (b) ? (b) : (a) )

typedef struct {  // Final Project Struct - Client Modbus TCP/IP
	char moving;
        char prg; 
	char mvjoints;
	char mvabs;
	char mvprg;
	char gtjoints;
	char gtabs;
	char connected;
	char ready;
	char signal;
	int J[6];
	int C[3];
} datastruct;

#include "prototypes.h" 
