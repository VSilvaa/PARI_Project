/**
 *       @file  myf.c
 *      @brief  Funções contidas na biblioteca VS_Modbus
 *
 * A biblioteca VS_Modbus foi criada de forma a consolidar os conhecimentos apreendidos nas aulas de PARI do ano letivo 2016/2017 sobre criação de bibliotecas dinâmicas. Foi ainda usada como forma de simplificação do código fonte usado no Projeto Final da Unidade Curricular. De entre as funções presentes na biblioteca destacam-se as funções de geração de mensagens modbus que desempenham um determinada função quando enviadas para o Servidos
 * A referida biblioteca inclui outras bibliotecas que são necessárias para que as funções sejam executadas sem erros, para se estabelecer a ligação TCP/IP e ainda para manipulação de strigs e sua impressão na linha de comandos.
 *
 *     @author  Vitor Silva, vans@ua.pt
 *
 *   @internal
 *     Created  31-Jan-2017
 *     Company  University of Aveiro
 *   Copyright  Copyright (c) 2016, Vitor Silva
 *
 * =====================================================================================
 */

#include "VS_Modbus.h"

/**
 * 	@brief  Obté uma memória partilhada e conecta-se a ela. A função 
 * primeiro cria uma chave baseada num diretório ou ficheiro para identificar 
 * a memória e conecta-se a ela com permissões específicas.
 * 	@return int ID da memória partilhada. Retorna -1 caso falhe.
 */
int GetSharedMem()
{
        key_t key;                       // long int
        int shm_id;
        key = ftok("/", 'X');       	 // gera uma chave random baseada no nome de um ficheiro
        if(key == -1) { perror("ftok"); return -1; }

        // Conecta-se ( ou cria) a memória com a permissão 644 
        shm_id = shmget(key, SHM_SIZE, 0644 | IPC_CREAT);
        if(shm_id == -1) { perror("shmget"); return -1; }

        return shm_id;
}


/**
 *     @brief  Gera a mensagem ModBus TCP/IP que altera o estado de duas saídas
 * digitais do servidor que fazem com que o robot se mova para o valor de juntas pré definido
 *     @param  Vazio
 *     @return obuf - char array com a mensagem referida
 */ 
unsigned char *VS_Modbus_MvJOI( void )
{
	unsigned char *obuf=malloc(15*sizeof(unsigned char));

	obuf[0] = 0; //Transaction Identifier
	obuf[1] = 2;

	obuf[2] = 0; //Protocol Identifier
	obuf[3] = 0;

	obuf[4] = 0; //Message length
	obuf[5] = 9;

	obuf[6] = 1; //The Unit Identifier

	obuf[7] = 0x0f; //Function Code

	obuf[8] = 0; //First Register Identifier
	obuf[9] = 0;

	obuf[10]= 0; //Total Number
	obuf[11]= 9;

	obuf[12]= 2; //N bytes

	obuf[13]= 3; //Values
	obuf[14]= 0;

	return obuf;
}

/**
 *     @brief  Gera a mensagem ModBus TCP/IP que altera o estado de duas saídas
 * digitais do servidor que fazem com que o robot se mova para as coordenadas absolutas
 * pré definidas
 *     @param  Vazio
 *     @return obuf - char array com a mensagem referida
 */ 
unsigned char *VS_Modbus_MvABS( void )
{
	unsigned char *obuf=malloc(15*sizeof(unsigned char));

	obuf[0] = 0; //Transaction Identifier
	obuf[1] = 2;

	obuf[2] = 0; //Protocol Identifier
	obuf[3] = 0;

	obuf[4] = 0; //Message length
	obuf[5] = 9;

	obuf[6] = 1; //The Unit Identifier

	obuf[7] = 0x0f; //Function Code

	obuf[8] = 0; //First Register Identifier
	obuf[9] = 0;

	obuf[10]= 0; //Total Number
	obuf[11]= 9;

	obuf[12]= 2; //N bytes

	obuf[13]= 5; //Values
	obuf[14]= 0;

	return obuf;
}

/**
 *     @brief  Gera a mensagem ModBus TCP/IP que altera o estado de várias saídas
 * digitais do servidor que fazem com que o robot execute um determinado programa
 *     @param  Vazio
 *     @return obuf - char array com a mensagem referida
 */ 
unsigned char *VS_Modbus_MvPRG( char prg)
{
	unsigned char *obuf=malloc(15*sizeof(unsigned char));

	obuf[0] = 0; //Transaction Identifier
	obuf[1] = 2;

	obuf[2] = 0; //Protocol Identifier
	obuf[3] = 0;

	obuf[4] = 0; //Message length
	obuf[5] = 9;

	obuf[6] = 1; //The Unit Identifier

	obuf[7] = 0x0f; //Function Code

	obuf[8] = 0; //First Register Identifier
	obuf[9] = 0;

	obuf[10]= 0; //Total Number
	obuf[11]= 9;

	obuf[12]= 2; //N bytes

	switch(prg){
		case 1:
			obuf[13]= 0x49; //Values
			obuf[14]= 0;
			break;
		case 2:
			obuf[13]= 0x89; //Values
			obuf[14]= 0;
			break;
		default:
			obuf[13]= 9; //Values
			obuf[14]= 1;
	}

	return obuf;
}

/**
 *     @brief  Gera a mensagem ModBus TCP/IP que altera o estado de várias saídas
 * digitais do servidor que fazem com que o robot execute um determinado programa
 * leia a posição actual das juntas e escreva esses valores em registos que posteriormente
 * serão lidos
 *     @param  Vazio
 *     @return obuf - char array com a mensagem referida
 */ 
unsigned char *VS_Modbus_GtJOI( void )
{
	unsigned char *obuf=malloc(15*sizeof(unsigned char));

	obuf[0] = 0; //Transaction Identifier
	obuf[1] = 2;

	obuf[2] = 0; //Protocol Identifier
	obuf[3] = 0;

	obuf[4] = 0; //Message length
	obuf[5] = 9;

	obuf[6] = 1; //The Unit Identifier

	obuf[7] = 0x0f; //Function Code

	obuf[8] = 0; //First Register Identifier
	obuf[9] = 0;

	obuf[10]= 0; //Total Number
	obuf[11]= 9;

	obuf[12]= 2; //N bytes

	obuf[13]= 0x11; //Values
	obuf[14]= 0;

	return obuf;
}

/**
 *     @brief  Gera a mensagem ModBus TCP/IP que altera o estado de várias saídas
 * digitais do servidor que fazem com que o robot execute um determinado programa
 * leia a posição actual da ponta em coordenadas absolutas e escreva esses valores 
 * em registos que posteriormente serão lidos
 *     @param  Vazio
 *     @return obuf - char array com a mensagem referida
 */ 
unsigned char *VS_Modbus_GtABS( )
{
	unsigned char *obuf=malloc(15*sizeof(unsigned char));

	obuf[0] = 0; //Transaction Identifier
	obuf[1] = 2;

	obuf[2] = 0; //Protocol Identifier
	obuf[3] = 0;

	obuf[4] = 0; //Message length
	obuf[5] = 9;

	obuf[6] = 1; //The Unit Identifier

	obuf[7] = 0x0f; //Function Code

	obuf[8] = 0; //First Register Identifier
	obuf[9] = 0;

	obuf[10]= 0; //Total Number
	obuf[11]= 9;

	obuf[12]= 2; //N bytes

	obuf[13]= 0x21; //Values
	obuf[14]= 0;

	return obuf;
}

/**
 *     @brief  Gera a mensagem ModBus TCP/IP que escreve no sregistos do servidor o valor
 * das juntas para o qual o robot se pretende mover (posições 3 a 9)
 *     @param  modbus - estrutura de dados com todas as informações acerca 
 * do estado actual do servidor ModBus, da ligação e contém valores de juntas
 * e coordenadas absolutas
 *     @return obuf - char array com a mensagem referida
 */ 
unsigned char *VS_Modbus_WrJOI( datastruct *modbus)
{
	unsigned char *obuf=malloc(26*sizeof(unsigned char));

	int j1 = modbus->J[0];
	int j2 = modbus->J[1];
	int j3 = modbus->J[2];
	int j4 = modbus->J[3];
	int j5 = modbus->J[4];
	int j6 = modbus->J[5];

	obuf[0] = 0; //Transaction Identifier
	obuf[1] = 2;

	obuf[2] = 0; //Protocol Identifier
	obuf[3] = 0;

	obuf[4] = 0; //Message length
	obuf[5] = 0x13;

	obuf[6] = 1; //The Unit Identifier

	obuf[7] = 0x10; //Function Code

	obuf[8] = 0; //First Register Identifier
	obuf[9] = 3;

	obuf[10]= 0; //Total Number
	obuf[11]= 6;

	obuf[12]= 0x0c; //Number Values

	obuf[13]= (j1 >> 8) & 0xFF; //Value 1
	obuf[14]=  j1 & 0xFF;

	obuf[15]= (j2 >> 8) & 0xFF; //Value 2
	obuf[16]=  j2 & 0xFF;

	obuf[17]= (j3 >> 8) & 0xFF; //Value 3
	obuf[18]=  j3 & 0xFF;

	obuf[19]= (j4 >> 8) & 0xFF; //Value 4
	obuf[20]=  j4 & 0xFF;

	obuf[21]= (j5 >> 8) & 0xFF; //Value 5
	obuf[22]=  j5 & 0xFF;

	obuf[23]= (j6 >> 8) & 0xFF; //Value 6
	obuf[24]=  j6 & 0xFF;

	return obuf;
}

/**
 *     @brief  Gera a mensagem ModBus TCP/IP que escreve no sregistos do servidor o valor
 * das coordenadas absolutas para o qual o robot se pretende mover (posições 1 a 3)
 *     @param  modbus - estrutura de dados com todas as informações acerca 
 * do estado actual do servidor ModBus, da ligação e contém valores de juntas
 * e coordenadas absolutas
 *     @return obuf - char array com a mensagem referida
 */ 
unsigned char *VS_Modbus_WrABS( datastruct *modbus)
{
	unsigned char *obuf=malloc(22*sizeof(unsigned char));

	int xx = modbus->C[0];
	int yy = modbus->C[1];
	int zz = modbus->C[2];

	obuf[0] = 0; //Transaction Identifier
	obuf[1] = 2;

	obuf[2] = 0; //Protocol Identifier
	obuf[3] = 0;

	obuf[4] = 0; //Message length
	obuf[5] = 0x0d;

	obuf[6] = 1; //The Unit Identifier

	obuf[7] = 0x10; //Function Code

	obuf[8] = 0; //First Register Identifier
	obuf[9] = 0;

	obuf[10]= 0; //Total Number
	obuf[11]= 3;

	obuf[12]= 0; //Number Values


	obuf[13]= (xx >> 8) & 0xFF; //Value 1
	obuf[14]=  xx & 0xFF;

	obuf[15]= (yy >> 8) & 0xFF; //Value 2
	obuf[16]=  yy & 0xFF;

	obuf[17]= (zz >> 8) & 0xFF; //Value 3
	obuf[18]=  zz & 0xFF;

	return obuf;
}

/**
 *     @brief  Gera a mensagem ModBus TCP/IP que pede ao Servidor Modbus para lhe enviar
 * o valor da primeira saída digital que informa se o robot está a executal alguma tarefa espeçífica
 *     @param  Vazio
 *     @return obuf - char array com a mensagem referida
 */ 
unsigned char *VS_Modbus_Idle( )
{
	unsigned char *obuf=malloc(14*sizeof(unsigned char));

	obuf[0] = 0; //Transaction Identifier
	obuf[1] = 4;

	obuf[2] = 0; //Protocol Identifier
	obuf[3] = 0;

	obuf[4] = 0; //Message length
	obuf[5] = 6;

	obuf[6] = 1; //The Unit Identifier

	obuf[7] = 1; //Function Code

	obuf[8] = 0; //First Register Identifier
	obuf[9] = 0;

	obuf[10]= 0; //Total Number
	obuf[11]= 1;

	return obuf;
}

/**
 *     @brief  Gera a mensagem ModBus TCP/IP que pede ao Servidor Modbus para lhe enviar
 * os valores dos 9 registos coorespondentes aos valores das juntas e coordenadas absolutas
 *     @param  Vazio
 *     @return obuf - char array com a mensagem referida
 */ 
unsigned char *VS_Modbus_GtValues( datastruct *modbus)
{
	unsigned char *obuf=malloc(14*sizeof(unsigned char));

	obuf[0] = 0; //Transaction Identifier
	obuf[1] = 3;

	obuf[2] = 0; //Protocol Identifier
	obuf[3] = 0;

	obuf[4] = 0; //Message length
	obuf[5] = 6;

	obuf[6] = 1; //The Unit Identifier

	obuf[7] = 3; //Function Code

	obuf[8] = 0; //First Register Identifier
	obuf[9] = 0;

	obuf[10]= 0; //Total Number
	obuf[11]= 9;

	return obuf;
}

/**
 *     @brief  Gera a mensagem ModBus TCP/IP que altera o valor de uma saída digital
 * que informa se o robot está a executar alguma tarefa ou não
 *     @param  Vazio
 *     @return obuf - char array com a mensagem referida
 */ 
unsigned char *VS_Modbus_MSwitch( datastruct *modbus )
{
	unsigned char *obuf=malloc(15*sizeof(unsigned char));

	obuf[0] = 0; //Transaction Identifier
	obuf[1] = 2;

	obuf[2] = 0; //Protocol Identifier
	obuf[3] = 0;

	obuf[4] = 0; //Message length
	obuf[5] = 6;

	obuf[6] = 1; //The Unit Identifier

	obuf[7] = 5; //Function Code

	obuf[8] = 0; //First Register Identifier
	obuf[9] = 0;

	obuf[10]= 0; //Total Number
	obuf[11]= 0;

	return obuf;
}

/**
 *     @brief  Lê na mensagem recebida o valor dos 9 registos corespondentes aos 
 * valores das juntas e actualiza a estrutura com esses novos valores
 *     @param  modbus - estrutura de dados com todas as informações acerca 
 * do estado actual do servidor ModBus, da ligação e contém valores de juntas
 * e coordenadas absolutas
 *     @return obuf - char array com a mensagem referida
 */ 
void VS_ReadJoints ( datastruct *modbus , char *ibuf )
{

	// Read Holding Registers - Joints
	for( int t=1;t<7;t++ ){
		int i = *(signed char *)(&ibuf[13+2*t]);
		i *= 1 << CHAR_BIT;					
		i |= ibuf[14+2*t];
		modbus->J[t-1]=i;
	}

}

/**
 *     @brief  Lê na mensagem recebida o valor dos 9 registos corespondentes aos 
 * valores das coordenadas absolutas e actualiza a estrutura com esses novos valores
 *     @param  modbus - estrutura de dados com todas as informações acerca 
 * do estado actual do servidor ModBus, da ligação e contém valores de juntas
 * e coordenadas absolutas; ibuf - char array com a mensagem a ler
 *     @return obuf - char array com a mensagem referida
 */ 
void VS_ReadCoords( datastruct *modbus , char *ibuf )
{

	// Read Holding Registers - Abs. Coordinates
	for( int t=1;t<4;t++ ){
		int i = *(signed char *)(&ibuf[7+2*t]);
		i *= 1 << CHAR_BIT;					
		i |= ibuf[8+2*t];
		modbus->C[t-1]=i;
	}						

}

/**
 *     @brief  Gera a mensagem ModBus TCP/IP que escreve
 *     @param  buf - mensagem a imprimir, length - tamanho da 
 * mensagem; type - tipo da mensagem Input/Output
 */ 
void VS_PrintMessages(char *buf , int length , char* type)
{

	// Print the messages in hexadecimal values
	for (int j=0;j<length;j++) 
	{
	      unsigned short ww = buf[j];
	      printf("%s %02x = %02x\n", type, j, ww);
	}

}

