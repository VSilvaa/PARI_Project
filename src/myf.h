#include <VS_Modbus.h>

#ifdef _MAIN_C_
	GtkBuilder *builderG;
	int error=0;
	int socket_desc =0;
	datastruct datag;      //generic pointer to serve as link for the shared memory
	datastruct *datagtk = & datag;
	int s;
#else 
	extern GtkBuilder *builderG;
	extern int error;
	extern int socket_desc;
	extern datastruct datag;      //generic pointer to serve as link for the shared memory
	extern datastruct *datagtk;
	extern int s;
#endif

#include "prototypes.h"
