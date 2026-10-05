#ifndef WEB_H
#define WEB_H

#include <WiFi.h>
#include <WebServer.h>




// ============================================================
// Definiciones
// ============================================================

const char* ssid = "DronESP";
const char* password = "12345678";

WebServer server(80);


// ============================================================
// Funciones
// ============================================================


void handleRoot();
void handleDatos(); 
void handleControl();
void handleEncenderMotores();



#endif