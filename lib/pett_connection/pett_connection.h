#ifndef pett_connection_h
#define pett_connection_h

void task_connection(void * parameters);

void connectionServer();

void setup_wifi();

void paginaconf();

void modoconf();

void guardar_conf();

void grabar(int addr, String a);

String leer(int addr);

void escanear();

void setupInitialIoT();

#endif