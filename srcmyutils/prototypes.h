/*File generated automatically in vitor-SATELLITE-L50-A by vitor on Qua Fev  1 19:14:18 WET 2017*/
#ifdef __cplusplus
extern "C" {
#endif
/* myf.c */
int GetSharedMem();
unsigned char *VS_Modbus_GtABS( );
unsigned char *VS_Modbus_GtJOI( void );
unsigned char *VS_Modbus_GtValues( datastruct *modbus);
unsigned char *VS_Modbus_Idle( );
unsigned char *VS_Modbus_MSwitch( datastruct *modbus );
unsigned char *VS_Modbus_MvABS( void );
unsigned char *VS_Modbus_MvJOI( void );
unsigned char *VS_Modbus_MvPRG( char prg);
unsigned char *VS_Modbus_WrABS( datastruct *modbus);
unsigned char *VS_Modbus_WrJOI( datastruct *modbus);
void VS_PrintMessages(char *buf , int length , char* type);
void VS_ReadCoords( datastruct *modbus , char *ibuf );
void VS_ReadJoints ( datastruct *modbus , char *ibuf );
#ifdef __cplusplus
}
#endif
