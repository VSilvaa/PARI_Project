/**
 *       @file  main.c
 *      @brief  Ficheiro principal do código fonte utilizado na realização do projeto final 
 * da unidade corricular de Projeto em Automação e Robótica Indústrial
 *
 * O ficheiro está associado ao processo principal ( PAI ) da aplicação que é responsável
 * por mostrar a interface gráfica e ficar à espera das ordens do Operador para encaminhar 
 * os eventos gerados para o ficheiro callbacks.c. É responsável ainda por inicializar o timer
 * que irá atualizar alguns dos campos da interface e atualizar dados.
 * O processo em questão irá comunicar com o processo filho através de uma memória partilhada que
 * é inicializada neste mesmo ficheiro. Quando se pretendem fechar ambos os processos, este é o 
 * responsável por "eliminar" a memória ficando disponível para outros processos.
 *
 *     @author  Vitor Silva, vans@ua.pt
 *
 *   @internal
 *     Created  21-Set-2016
 *     Company  University of Aveiro
 *   Copyright  Copyright (c) 2016, Vitor Silva
 *
 * =====================================================================================
 */

#define _MAIN_C_

#include "myf.h" 

void main(int argc, char *argv[])
{

	// Initialize GTK
	gtk_init(&argc, &argv);

        // Load the interface after a configuration file
        builderG = gtk_builder_new();

        //gtk_builder_add_from_file(builderG, "exp.glade", NULL);
	gtk_builder_add_from_file(builderG, "Modbus_v2.glade", NULL);

        // Connect the signals in the interface
        gtk_builder_connect_signals(builderG, NULL);

        // Get main window Widget ID and connect special signals
        GtkWidget *t = GTK_WIDGET(gtk_builder_get_object(builderG, "window1"));

        // Use signal to catch SIGINT  (CTRL-C)
	signal(SIGINT, InterceptCTRL_C);

        // Start the event loop
	int shm_id;

	//Gets a shared memory and connects to it
        shm_id = GetSharedMem();

        if(shm_id == -1) error=1;     //failure

        // Attach to the memory segment to get a pointer to it
        datagtk = (datastruct *)shmat(shm_id, (void *) 0, 0);

        if(datagtk == (datastruct *) (-1))     //failure
        {
                perror("shmat");
                exit(1);
        }

	// Initialize some variables
	datagtk->ready=1;
	datagtk->signal=0;
	
	// Initialize Callback Timer
	p_InitTimer();

	if (error) exit(0); // Terminate Process
	else gtk_main();  // Hand control over to the main loop

	// Detach from the memory segment
        if(  shmdt(datagtk) == -1 )
        {
                perror("shmdt");
                exit(1);
        }

	//Allow elimination of shared memory
	if(shm_id > 0) shmctl(shm_id, IPC_RMID, NULL);  

	//Terminate the process
	exit(0);
}
