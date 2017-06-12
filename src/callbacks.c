/**
 *       @file  callbacks.c
 *      @brief  Ficheiro de suporte ao main.c, contém todas as funções e sinais que não estão comtemplados
 * na biblioteca VS_ModBus
 *
 * Este ficheiro contém todas as callbacks geradas pelos eventos associados às ações do utilizador na aplicação Mosbus_Fanuc, contém ainda o importante timer que é responsável pela atualização de valores da memória partilhada, e por pequenas alterações na interface em função do estado da conexão.
 * O botão 6 (Connect) é responsável pela biforcação dos processos ficando um responsável pela interface e o segundo (filho) responsável pela ligação tcp/ip ao servidor modbus. Deste modo o botão 6 fica responsável por importantes tarefas. Optou-se por não passar nem a criação da memória partilhada nem o estabelecimento da ligação tcp/ip para funções pois desta forma teria que se criar um novo ficheiro de suporte ao main.c com apenas essas funções ou incluir as referidas funções na biblioteca gerada, o que não seria uma boa prática pois ficaria com imensos parâmetros de entrada. Pensou-se que desta forma ficava mais claro para um leitor que fosse analisar o código ou alterá-lo. Como ponto negativo a callback associada ao clique no botão 6 ficou bastante extensa.
 * 
 *
 *     @author  Vitor Silva, vans@ua.pt
 *
 *   @internal
 *     Created  30-Nov-2016
 *     Company  University of Aveiro
 *   Copyright  Copyright (c) 2016, Vitor Silva
 *
 * =====================================================================================
 */

#include "myf.h"

/**
 *     @brief  Inicialização do timer que faz refresh dos labels de conexão, de estado e
 * ativar e desativar a sensibilidade dos botões perante o estado da conexão e atualizar 
 * os campos das coordenadas e juntas caso seja para o fazer
 *     @param  Vazio
 *     @return Vazio
 */ 
void p_InitTimer()
{
	GtkLabel *lbl= GTK_LABEL( gtk_builder_get_object(builderG, "label55") ); // Lbl Connected
	g_timeout_add (500, p_RefreshStatus, lbl); // 500 ms interval
}

/**
 *     @brief  Timer que faz refresh dos labels de conexão, de estado e
 * ativar e desativar a sensibilidade dos botões perante o estado da conexão e atualizar 
 * os campos das coordenadas e juntas caso seja para o fazer
 *     @param  user_data - Label Status
 *     @return TRUE - timer continua; FALSE - timer pára
 */ 
gboolean p_RefreshStatus(gpointer user_data)
{
	char string[24];
  	time_t timep=time(NULL);
        struct tm *tm=localtime(&timep);
	sprintf(string,"%02d-%02d-%02d  |  %02d:%02d:%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
	
	// Actualize Date/Hour
	GtkLabel *lbl_hora= GTK_LABEL( gtk_builder_get_object(builderG, "label14") );
	gtk_label_set_text(lbl_hora,string);

	// Create variables for the objects
	GtkWidget *b1 = GTK_WIDGET(gtk_builder_get_object(builderG, "button6"));
	GtkWidget *b2 = GTK_WIDGET(gtk_builder_get_object(builderG, "button7"));
	GtkWidget *b3 = GTK_WIDGET(gtk_builder_get_object(builderG, "button4"));
	GtkWidget *b4 = GTK_WIDGET(gtk_builder_get_object(builderG, "button8"));
	GtkWidget *b5 = GTK_WIDGET(gtk_builder_get_object(builderG, "button10"));
	GtkWidget *b6 = GTK_WIDGET(gtk_builder_get_object(builderG, "button5"));
	GtkSpinner *s1 = GTK_SPINNER(gtk_builder_get_object(builderG, "spinner1"));


	if(datagtk->connected==1) // If connected
	{
		// change buttons sensitive
		gtk_widget_set_sensitive(b1,FALSE);
		gtk_widget_set_sensitive(b2,TRUE);
		gtk_widget_set_sensitive(b3,TRUE);
		gtk_widget_set_sensitive(b4,TRUE);
		gtk_widget_set_sensitive(b5,TRUE);
		gtk_widget_set_sensitive(b6,TRUE);

		// stop spinner
		gtk_spinner_stop (s1);		
		
		GtkLabel *lbl;

		// Actualize label status
		if( ! user_data )
			lbl=GTK_LABEL( gtk_builder_get_object(builderG, "label55") );
		else
			lbl=GTK_LABEL(user_data);

		// Actualize switches status
		GtkSwitch *swit= GTK_SWITCH( gtk_builder_get_object(builderG, "switch1") );

		if(datagtk->ready==0){
			gtk_switch_set_active (swit, TRUE);
		}else{
			gtk_switch_set_active (swit, FALSE);
		}

		GtkSwitch *swit1= GTK_SWITCH( gtk_builder_get_object(builderG, "switch3") );
		if(datagtk->ready==0){
			gtk_switch_set_active (swit1, TRUE);
		}else{
			gtk_switch_set_active (swit1, FALSE);
		}

		GtkSwitch *swit2= GTK_SWITCH( gtk_builder_get_object(builderG, "switch4") );
		if(datagtk->mvjoints==1){
			gtk_switch_set_active (swit2, TRUE);
		}else{
			gtk_switch_set_active (swit2, FALSE);
		}

		GtkSwitch *swit3= GTK_SWITCH( gtk_builder_get_object(builderG, "switch5") );
		if(datagtk->mvabs==1){
			gtk_switch_set_active (swit3, TRUE);
		}else{
			gtk_switch_set_active (swit3, FALSE);
		}

		GtkSwitch *swit4= GTK_SWITCH( gtk_builder_get_object(builderG, "switch6") );
		if(datagtk->mvprg==1){
			gtk_switch_set_active (swit4, TRUE);
		}else{
			gtk_switch_set_active (swit4, FALSE);
		}

		GtkSwitch *swit5= GTK_SWITCH( gtk_builder_get_object(builderG, "switch7") );
		if(datagtk->gtjoints==1){
			gtk_switch_set_active (swit5, TRUE);
		}else{
			gtk_switch_set_active (swit5, FALSE);
		}

		GtkSwitch *swit6= GTK_SWITCH( gtk_builder_get_object(builderG, "switch8") );
		if(datagtk->gtabs==1){
			gtk_switch_set_active (swit6, TRUE);
		}else{
			gtk_switch_set_active (swit6, FALSE);
		}

		// Actualize absolute coordinates in the interface
		if(datagtk->gtabs==1)
		{
			char C1[3], C2[3], C3[3];

			sprintf(C1,"%d",datagtk->C[0]);
			sprintf(C2,"%d",datagtk->C[1]);
			sprintf(C3,"%d",datagtk->C[2]);

			GtkLabel *lblc1= GTK_LABEL( gtk_builder_get_object(builderG, "label7") );
			gtk_label_set_text(lblc1,C1);

			GtkLabel *lblc2= GTK_LABEL( gtk_builder_get_object(builderG, "label8") );
			gtk_label_set_text(lblc2,C2);

			GtkLabel *lblc3= GTK_LABEL( gtk_builder_get_object(builderG, "label9") );
			gtk_label_set_text(lblc3,C3);
		}

		// Actualize Joints values in the interface
		if(datagtk->gtjoints==1)
		{
			char J1[3], J2[3], J3[3], J4[3], J5[3], J6[3];

			sprintf(J1,"%d",datagtk->J[0]);
			sprintf(J2,"%d",datagtk->J[1]);
			sprintf(J3,"%d",datagtk->J[2]);
			sprintf(J4,"%d",datagtk->J[3]);
			sprintf(J5,"%d",datagtk->J[4]);
			sprintf(J6,"%d",datagtk->J[5]);

			GtkLabel *lblj1= GTK_LABEL( gtk_builder_get_object(builderG, "label30") );
			gtk_label_set_text(lblj1,J1);

			GtkLabel *lblj2= GTK_LABEL( gtk_builder_get_object(builderG, "label31") );
			gtk_label_set_text(lblj2,J2);

			GtkLabel *lblj3= GTK_LABEL( gtk_builder_get_object(builderG, "label32") );
			gtk_label_set_text(lblj3,J3);

			GtkLabel *lblj4= GTK_LABEL( gtk_builder_get_object(builderG, "label43") );
			gtk_label_set_text(lblj4,J4);

			GtkLabel *lblj5= GTK_LABEL( gtk_builder_get_object(builderG, "label44") );
			gtk_label_set_text(lblj5,J5);

			GtkLabel *lblj6= GTK_LABEL( gtk_builder_get_object(builderG, "label45") );
			gtk_label_set_text(lblj6,J6);
		}

		// Actualize connection status
		if( datagtk->connected==1 ){
			gtk_label_set_text(lbl,"Status : Connected");
		}else{
			gtk_label_set_text(lbl,"Status : Disconnected");
		}

		// Actualize running status
		GtkLabel *lbl_robot;
		lbl_robot=GTK_LABEL( gtk_builder_get_object(builderG, "label54") );
		if( datagtk->ready==1 ){
			gtk_label_set_text(lbl_robot,"WAITING");
		}else{
			gtk_label_set_text(lbl_robot,"RUNNING");
		}

               
	}else if (datagtk->connected==0){ // Disconnected

		// Actualize labels
		GtkLabel *lbl_robot;
		lbl_robot=GTK_LABEL( gtk_builder_get_object(builderG, "label54") );
		gtk_label_set_text(lbl_robot,"--------");

		GtkLabel *lbl=GTK_LABEL( gtk_builder_get_object(builderG, "label55") );
		gtk_label_set_text(lbl,"Status : Disconnected");

		// change buttons sensitive
		gtk_widget_set_sensitive(b1,TRUE);
		gtk_widget_set_sensitive(b2,FALSE);
		gtk_widget_set_sensitive(b3,FALSE);
		gtk_widget_set_sensitive(b4,FALSE);
		gtk_widget_set_sensitive(b5,FALSE);
		gtk_widget_set_sensitive(b6,FALSE);
		
		// Stop spinner
		gtk_spinner_stop (s1);		
		
	}else{ // Connecting
		// Start spinner
		gtk_spinner_start (s1);
	}
	return TRUE; //to keep timer repeating. FALSE would stop the timer
}

/**
 *     @brief  Callback chamada aquando premido o botão Connect, responsável por biforcar os processos
 * e fazer atach da estrutura datam à memória partilhada
 *     @param  widget - ponteiro para o objeto que gerou o evento que irá chamar a callback, user_data - objeto definido pelo utilizador que também dá entrada na callback
 *     @return Vazio
 */ 
void on_button6_clicked(GtkWidget *widget, gpointer user_data)
{
	if(datagtk->connected==0) // if connected
	{
		//from this point process can check where does it come from: parent or child!
		int pid = fork();
		
		if(pid == -1)
		{
		        printf("Could not fork(). Exiting\n");  //still the parent but failed to fork
		        //return -1;
		}
		if(pid == 0)  /* This is the child */
		{
		        printf("I am the child\n");
				
			// Create datastruct to associate with the shared memory
			datastruct data;
			datastruct *datam = & data;

			int shm_id, nbae;
			shm_id = GetSharedMem();
			if(shm_id == -1) error=1;;     //failure

			// Attach to the memory segment to get a pointer to it 
			datam = (datastruct *)shmat(shm_id, (void *) 0, 0);
			if(datam == (datastruct *) (-1))
			{
				perror("shmat");
				exit(1);
			}

			char *ip_adrs;
			int i;
			struct sockaddr_in server;
			fd_set fds;
			struct timeval tv;
			unsigned char obuf[261];
			unsigned char ibuf[261];
			
			// Read IP and Port entry
			GtkEntry *ts1 = GTK_ENTRY(gtk_builder_get_object(builderG, "entry4"));
			const gchar* ipgtk = gtk_entry_get_text(ts1);
			ip_adrs = (char *) ipgtk;


			GtkEntry *ts2 = GTK_ENTRY(gtk_builder_get_object(builderG, "entry5"));
			const gchar* tss2 = gtk_entry_get_text(ts2);
			char **portc=malloc(100);
			int porti = strtol(tss2,portc,10);


			free(portc);

			printf("ip_adrs = %s |  port = %d \n", ip_adrs, porti);

			// Establish connection to gateway on ASA standard port 502
			s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
			server.sin_family = AF_INET;
			server.sin_port = htons(porti); /* ASA standard port */
			server.sin_addr.s_addr = inet_addr(ip_adrs);
			
			datam->connected=3;
			i = connect(s, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
			
			// Inform Connection Status in the terminal
			if (i<0)
			{
				printf("connect - error %d\n",i);
				close(s);
				datam->connected=0;
			}else{
				datam->connected=1;
				printf("Success - Conected\n");
			}

			FD_ZERO(&fds);
			tv.tv_sec = 5;
			tv.tv_usec = 0;

			/* check ready to send */
			FD_SET(s, &fds);
			i = select(32, NULL, &fds, NULL, &tv);
			if(0)if (i<=0)
			{
				printf("select - error %d\n",i);
				close(s);
				datam->connected=0;
			}

			datam->ready=1;
			short step=1;

			while(datam->connected==1 && error == 0 )
			{
				unsigned char *obuff;

				if(datam->moving==1 && datam->ready==1)
				{				
					if(datam->mvjoints==1){

						if(step==1){
							obuff=VS_Modbus_WrJOI(datam);
							nbae=25;
						}else{
							obuff=VS_Modbus_MvJOI( );
							nbae=15;
						}

						step=1-step;
					
					}else if(datam->mvabs==1){
						if(step==1){
							obuff=VS_Modbus_WrABS(datam);
							nbae=19;
						}else{
							obuff=VS_Modbus_MvABS( );
							nbae=15; 
						}

						step=1-step;
					
					}else if(datam->mvprg==1){
						obuff=VS_Modbus_MvPRG(datam->prg);
						nbae=15;
					}else if(datam->gtjoints==1){

						if(step==1){
							obuff=VS_Modbus_GtJOI( );
							nbae=15;
						}else{
							obuff=VS_Modbus_GtValues(datam);
							nbae=15;
						}

						step=1-step;

					}else if(datam->gtabs==1){

						if(step==1){
							obuff=VS_Modbus_GtABS(datam);
							nbae=15;
						}else{
							obuff=VS_Modbus_GtValues(datam);
							nbae=15;
						}

						step=1-step;

					};

					if(step==1){
					datam->moving=0;
					datam->ready=0;
					}

				}else if ( datam->signal ==1 ){

					obuff = VS_Modbus_MSwitch( datam );
					nbae=12;
					datam->signal =0;
				
				}else{
					obuff = VS_Modbus_Idle( datam );
					nbae=12;
				}

				// send request
				i = send(s, obuff, nbae, 0);

				//VS_PrintMessages( obuff , i , "Output" );

				free(obuff); // Free memory

				if (i<nbae)
				{
					printf("failed to send all %d chars\n",nbae);
				}

				/* wait for response */
				FD_SET(s, &fds);
				i = select(32, &fds, NULL, NULL, &tv);
				if (i<=0)
				{
					printf("no TCP response received\n");
					close(s);
				}

				/* receive and analyze response */
				i = recv(s, ibuf, 261, 0);
				if (i<9)
				{

					printf("response was too short - %d chars\n", i);

				}
				else if (ibuf[7] & 0x80)
				{
					printf("MODBUS exception response - type %d\n", ibuf[8]);
				}
				else
				{

					//VS_PrintMessages( ibuf , i , "Input" );


					if( i>25 && ibuf[5]==21 && ibuf[1]==3)
					{
						
						VS_ReadJoints( datam , ibuf);

						VS_ReadCoords( datam , ibuf);


					}
					else if(ibuf[5]==4 && ibuf[1]==4 ) // Read 1st coil - ready
					{
						datam->ready = 1-ibuf[9];
					}
				}
			
				// Pause between messsages sent
				usleep(800000);

			}

			puts("Connection Lost!");

			// detach od the memory
			if(  shmdt(datagtk) == -1 )
			{
				perror("shmdt");
				exit(1);
			}

			exit(1);


		}else{  /* This is the parent */
		
		       printf("I am the parent\n");
		}

	}else{ // Case already connected
		  GtkWidget *w = GTK_WIDGET(gtk_builder_get_object(builderG, "messagedialog4"));
		  gtk_widget_show(w);  
	}
}

/**
 *     @brief  Callback chamada aquando premido o botão Disconnect
 *     @param  widget - ponteiro para o objeto que gerou o evento que irá chamar a callback, user_data - objeto definido pelo utilizador que também dá entrada na callback
 *     @return Vazio
 */ 
void on_button7_clicked(GtkWidget *widget, gpointer user_data) //Disconnect
{
	if(datagtk->connected==1)
	{
		//close the socket before exiting
		puts("Disconected\n");
		datagtk->connected=0;

		// Close down connection
		close(s);
	
	}else{ // Case already disconnected
		GtkWidget *w = GTK_WIDGET(gtk_builder_get_object(builderG, "messagedialog5"));
		gtk_widget_show(w);	
	}
}

/**
 *     @brief  Callback chamada aquando premido o botão Move
 *     @param  widget - ponteiro para o objeto que gerou o evento que irá chamar a callback, user_data - objeto definido pelo utilizador que também dá entrada na callback
 *     @return Vazio
 */ 
void on_button5_clicked(GtkWidget *widget, gpointer user_data) //Move
{
	if(datagtk->connected==1)
	{
		// verify if the robot is running some operation
		GtkSwitch *swit= GTK_SWITCH( gtk_builder_get_object(builderG, "switch9") );
		gboolean a=gtk_switch_get_active(swit);

		  if(datagtk->ready==1)
		  {		  
			  if (a == FALSE){
		
				printf("The Robot will move in Joints.\n");

				// create variables for the objects
				GtkAdjustment *a1=GTK_ADJUSTMENT( gtk_builder_get_object(builderG, "adjustment1") );
				int J1 = gtk_adjustment_get_value(a1);
				datagtk->J[0]=J1;

				GtkAdjustment *a2=GTK_ADJUSTMENT( gtk_builder_get_object(builderG, "adjustment2") );
				int J2 = gtk_adjustment_get_value(a2);
				datagtk->J[1]=J2;

				GtkAdjustment *a3=GTK_ADJUSTMENT( gtk_builder_get_object(builderG, "adjustment3") );
				int J3 = gtk_adjustment_get_value(a3);
				datagtk->J[2]=J3;

				GtkAdjustment *a4=GTK_ADJUSTMENT( gtk_builder_get_object(builderG, "adjustment4") );
				int J4 = gtk_adjustment_get_value(a4);
				datagtk->J[3]=J4;

				GtkAdjustment *a5=GTK_ADJUSTMENT( gtk_builder_get_object(builderG, "adjustment5") );
				int J5 = gtk_adjustment_get_value(a5);
				datagtk->J[4]=J5;

				GtkAdjustment *a6=GTK_ADJUSTMENT( gtk_builder_get_object(builderG, "adjustment6") );
				int J6 = gtk_adjustment_get_value(a6);
				datagtk->J[5]=J6;
			
				// Actualize datastruct to processe in children process	
				datagtk->moving=1;
				datagtk->mvjoints=1;
				datagtk->mvabs=0;
				datagtk->mvprg=0;
				datagtk->gtjoints=0;
				datagtk->gtabs=0;			

			  }else{
				// create variables for the objects
				GtkEntry *ts1 = GTK_ENTRY(gtk_builder_get_object(builderG, "entry1"));
				const gchar* tss1=gtk_entry_get_text(ts1);

				GtkEntry *ts2 = GTK_ENTRY(gtk_builder_get_object(builderG, "entry2"));
				const gchar* tss2=gtk_entry_get_text(ts2);

				GtkEntry *ts3 = GTK_ENTRY(gtk_builder_get_object(builderG, "entry3"));
				const gchar* tss3=gtk_entry_get_text(ts3);

				// verify if values are valid
				char **teste1=malloc(100);
				char **teste2=malloc(100);
				char **teste3=malloc(100);
				int posx = strtol(tss1,teste1,10);
				int posy = strtol(tss2,teste2,10);
				int posz = strtol(tss3,teste3,10);

				if (strcmp(tss1,*teste1)!=0 && strcmp(tss2,*teste2)!=0 && strcmp(tss3,*teste3)!=0)
				{
					// Actualize datastruct to processe in children process	
					printf("The Robot will move in Abs. Coordinates.\n");
					datagtk->C[0]=posx;
					datagtk->C[1]=posy;
					datagtk->C[2]=posz;
					datagtk->moving=1;
					datagtk->mvjoints=0;
					datagtk->mvabs=1;
					datagtk->mvprg=0;
					datagtk->gtjoints=0;
					datagtk->gtabs=0;
				}else{ // if values are not valid
					  GtkWidget *w = GTK_WIDGET(gtk_builder_get_object(builderG, "messagedialog3"));
					  gtk_widget_show(w);  
				}

				// free memory
				free(teste1);
				free(teste2);
				free(teste3);
			  }

		  }else{ // not ready

			  GtkWidget *w = GTK_WIDGET(gtk_builder_get_object(builderG, "messagedialog1"));
			  gtk_widget_show(w);  
		  }
	}else{ // Case not connected
		GtkWidget *w = GTK_WIDGET(gtk_builder_get_object(builderG, "messagedialog6"));
		gtk_widget_show(w); 
	}
}

/**
 *     @brief  Callback chamada aquando premido o botão About
 *     @param  widget - ponteiro para o objeto que gerou o evento que irá chamar a callback, user_data - objeto definido pelo utilizador que também dá entrada na callback
 *     @return Vazio
 */ 
void on_button20_clicked(GtkWidget *widget, gpointer user_data) //About
{
	// show about window
	GtkWidget *w = GTK_WIDGET(gtk_builder_get_object(builderG, "aboutdialog1"));
	gtk_widget_show(w);  
}

/**
 *     @brief  Callback chamada aquando premido o botão Get Coords, user_data - objeto definido pelo utilizador que também dá entrada na callback
 *     @param  widget - ponteiro para o objeto que gerou o evento que irá chamar a callback
 *     @return Vazio
 */ 
void on_button4_clicked(GtkWidget *widget, gpointer user_data) //Get Coords
{
	if(datagtk->connected==1)
	{
		if(datagtk->ready==1)
		{	
			// Actualize datastruct to processe in children process	  
			datagtk->moving=1;
			datagtk->mvjoints=0;
			datagtk->mvabs=0;
			datagtk->mvprg=0;
			datagtk->gtjoints=0;
			datagtk->gtabs=1;

		}else{ // not ready

			GtkWidget *w = GTK_WIDGET(gtk_builder_get_object(builderG, "messagedialog1"));
			gtk_widget_show(w);  

		}
	}else{ // Case not connected
		GtkWidget *w = GTK_WIDGET(gtk_builder_get_object(builderG, "messagedialog6"));
		gtk_widget_show(w); 
	}
}

/**
 *     @brief  Callback chamada aquando premido o botão Get Joints
 *     @param  widget - ponteiro para o objeto que gerou o evento que irá chamar a callback, user_data - objeto definido pelo utilizador que também dá entrada na callback
 *     @return Vazio
 */ 
void on_button8_clicked(GtkWidget *widget, gpointer user_data) //Get Joints
{
	if(datagtk->connected==1)
	{
		if(datagtk->ready==1)
		{	// Actualize datastruct to processe in children process  
			datagtk->moving=1;
			datagtk->mvjoints=0;
			datagtk->mvabs=0;
			datagtk->mvprg=0;
			datagtk->gtjoints=1;
			datagtk->gtabs=0;

		}else{ // not ready
			GtkWidget *w = GTK_WIDGET(gtk_builder_get_object(builderG, "messagedialog1"));
			gtk_widget_show(w);  

		}
	}else{ // Case not connected
		GtkWidget *w = GTK_WIDGET(gtk_builder_get_object(builderG, "messagedialog6"));
		gtk_widget_show(w); 
	}
}

/**
 *     @brief  Callback chamada aquando premido o botão Start
 *     @param  widget - ponteiro para o objeto que gerou o evento que irá chamar a callback, user_data - objeto definido pelo utilizador que também dá entrada na callback
 *     @return Vazio
 */ 
void on_button10_clicked(GtkWidget *widget, gpointer user_data) //Start 
{
	if(datagtk->connected) // If connected
	{
		short ncb=0;

		GtkWidget *CB1= GTK_WIDGET(gtk_builder_get_object (builderG, "checkbutton1"));
		gboolean CB11 = gtk_toggle_button_get_active((GtkToggleButton *)CB1);
		if(CB11==TRUE) {ncb++;}
		GtkWidget *CB2= GTK_WIDGET(gtk_builder_get_object (builderG, "checkbutton2"));
		gboolean CB22 = gtk_toggle_button_get_active((GtkToggleButton *)CB2);
		if(CB22==TRUE) {ncb++;}
		GtkWidget *CB3= GTK_WIDGET(gtk_builder_get_object (builderG, "checkbutton3"));
		gboolean CB33 = gtk_toggle_button_get_active((GtkToggleButton *)CB3);
		if(CB33==TRUE) {ncb++;}
		if(ncb!=1) // Case not progs sellected
		{
			  GtkWidget *w = GTK_WIDGET(gtk_builder_get_object(builderG, "messagedialog2"));
			  gtk_widget_show(w); 
		}else{	

			if(datagtk->ready==1)
			  {		  
				  // Actualize datastruct to processe in children process
				  if (CB11 == TRUE){		
					printf("Executing Program 1\n");				
					datagtk->moving=1;
					datagtk->mvjoints=0;
					datagtk->mvabs=0;
					datagtk->mvprg=1;
					datagtk->prg=1;
					datagtk->gtjoints=0;
					datagtk->gtabs=0;

				  }else if(CB22 == TRUE){
					printf("Executing Program 2\n");
					datagtk->moving=1;
					datagtk->mvjoints=0;
					datagtk->mvabs=0;
					datagtk->mvprg=1;
					datagtk->prg=2;
					datagtk->gtjoints=0;
					datagtk->gtabs=0;
				  }else{
					printf("Executing Program 3\n");
					datagtk->moving=1;
					datagtk->mvjoints=0;
					datagtk->mvabs=0;
					datagtk->mvprg=1;
					datagtk->prg=3;
					datagtk->gtjoints=0;
					datagtk->gtabs=0;
				  }
			  }else{ // Case not ready

				  GtkWidget *w = GTK_WIDGET(gtk_builder_get_object(builderG, "messagedialog1"));
				  gtk_widget_show(w);
			  }
		}
	}else{ // Case Disconnected
		GtkWidget *w = GTK_WIDGET(gtk_builder_get_object(builderG, "messagedialog6"));
		gtk_widget_show(w); 
	}
}

/**
 *     @brief  Callback chamada aquando premido o botão Quit, irá matar ambos os processos
 *     @param  widget - ponteiro para o objeto que gerou o evento que irá chamar a callback, user_data - objeto definido pelo utilizador que também dá entrada na callback
 *     @return Vazio
 */ 
void on_button3_clicked(GtkWidget *widget, gpointer user_data) //Quit
{

	// Close the socket before exiting
	if (datagtk->connected==3) puts("Danger: Closing the aplication while connecting!\n");
	else puts("Disconected\n");

	// close down connection 
	close(s);

	//Actualize shared memory to kill the children
	datagtk->connected=0;

	// time to children leave
	usleep(900000);

	// Close down connection 
	gtk_main_quit();
}

/**
 *     @brief  Callback chamada aquando premido o botão de fechar da janela, irá matar ambos os processos
 *     @param  widget - ponteiro para o objeto que gerou o evento que irá chamar a callback, user_data - objeto definido pelo utilizador que também dá entrada na callback
 *     @return Vazio
 */ 
void on_window1_delete_event(GtkWidget *widget, gpointer user_data) //Cruz
{

	// Close the socket before exiting
	if (datagtk->connected==3) puts("Danger: Closing the aplication while connecting!\n");
	else puts("Disconected\n");

	/* close down connection */
	close(s);

	//Actualize shared memory to kill the children
	datagtk->connected=0;

	// time to children leave
	usleep(900000);

	// Close down connection
	gtk_main_quit();
}

/**
 *     @brief  Callback chamada aquando mudado o estado do switch BUSY
 *     @param  widget - ponteiro para o objeto que gerou o evento que irá chamar a callback, user_data - objeto definido pelo utilizador que também dá entrada na callback
 *     @return Vazio
 */ 
void on_switch1_state_set(GtkWidget *widget, gpointer user_data) //Switch BUSY
{
	if(datagtk->connected==1) // if connected
	{
		// Change switch status
		GtkSwitch *swit= GTK_SWITCH( gtk_builder_get_object(builderG, "switch1") );
		gboolean a=gtk_switch_get_active(swit);

		if(a==FALSE){ // Aplication ready to the next operation
			datagtk->ready=1;
			datagtk->signal=1;
		}
	}else{ //  Case Diconnected
		GtkWidget *w = GTK_WIDGET(gtk_builder_get_object(builderG, "messagedialog6"));
		gtk_widget_show(w); 
	}
}

/**
 *     @brief  Callback para intercetar CTRL-C
 *     @param  a - inutilizado
 *     @return Vazio
 */ 
void InterceptCTRL_C(int a)
{
	g_print("\rSair por CTRL-C\n");

	// Close the socket before exiting
	if (datagtk->connected==3) puts("Danger: Closing the aplication while connecting!\n");
	else puts("Disconected\n");

	// Close down connection
	close(s);

	//Actualize shared memory to kill the children
	datagtk->connected=0;

	// time to children leave
	usleep(900000);

	// Leave GTK main loop
	gtk_main_quit();
}

