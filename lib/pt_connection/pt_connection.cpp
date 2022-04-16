#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "pt_connection.h"

//-------------------VARIABLES GLOBALES-----------------------
static int contconexion = 0;

static char ssid[50];      
static char pass[50];

const char *ssidConf = "PetTrainer";
const char *passConf = "";

String mensaje = "";

//-----------CODIGO HTML PAGINA DE CONFIGURACION---------------
String pagina = 
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
        "<title>PetTrainer WiFi</title>"
        "<meta charset='UTF-8'>"
        "<style>"
        "body{"
        "background-color:rgb(61, 69, 73);"
        "font-size: 20px;"
        "font-family:'Noto Sans',sans-serif; "
        "border: 0px solid black ;   "
        "}"
        "h1{color: whitesmoke;"
        "font-family: 'Noto Sans',sans-serif;}"
        "h2{color: whitesmoke;"
        "font-family: 'Noto Sans',sans-serif;}"
        "</style>"
        "</head>"
        "<body>"
        "<center><h1>PeTraining WiFI</h1></center>"
        "</form>"
        "<form action='guardar_conf' method='get'>"
        "<center><h2>SSID:<br><br></h2></center>"
        "<center><input class='input1' name='ssid' type='text'><br></center>"
        "<center><h2>PASSWORD:<br><br></h2></center>"
        "<center><input class='input1' name='pass' type='password'><br><br></center>"
        "<center><input class='boton' type='submit' value='GUARDAR'/><br><br></center>"
        "</form>"
        "<center><a href='escanear'><button class='boton'>ESCANEAR</button></a><br><br></center>";

String paginafin = "</body>"
"</html>";

void task_connection(void * pvParameters)
{
        while (1) {
                Serial.print("Task 2 counter:");
                vTaskDelay(1000/portTICK_PERIOD_MS);
        }
}


//------------------------SETUP WIFI-----------------------------

void connectionServer(){
        delay(10000);
        HTTPClient client;
        client.begin("https://server-pettrainer.herokuapp.com/api/petTraining/6191c18991c653d618c46f58");
        int httpCode = client.GET();
        if(httpCode >0){
                String payload=client.getString();
                Serial.println("\nStatuscode: "+String(httpCode));
                Serial.println(payload);

                StaticJsonDocument<0> filter;
                filter.set(true);
                
                StaticJsonDocument<512> doc;
                
                DeserializationError error = deserializeJson(doc, payload, DeserializationOption::Filter(filter));
                
                if (error) {
                        Serial.print(F("deserializeJson() failed: "));
                        Serial.println(error.f_str());
                        return;
                }
                int FoodDiary = doc["FoodDiary"]; // 4
                int RationsDiary = doc["RationsDiary"]; // 90
                int hour = doc["hour"]; // 22
                int minute = doc["minute"]; // 37
                int second = doc["second"]; // 43

                Serial.println("Cantidad de comida/dia: "+ String(FoodDiary));
                Serial.println("Raciones al dia: "+ String(RationsDiary));
                Serial.println("Hora exacta: "+String(hour)+":"+String(minute)+":"+String(second));
         

                client.end();
                
        }
        else{
                Serial.println("Error on HTTP request");
        }
}


void setup_wifi() {
// Conexión WIFI
        WiFi.mode(WIFI_STA); //para que no inicie el SoftAP en el modo normal
        WiFi.begin(ssid, pass);
        while (WiFi.status() != WL_CONNECTED and contconexion <10) { //Cuenta hasta 10 si no se puede conectar lo cancela
                ++contconexion;
                delay(250);
                Serial.print(".");
                delay(250);
                
        }
        if (contconexion <10) {   
                        Serial.println("");
                        Serial.println("WiFi conectado");
                        Serial.println(WiFi.localIP());
                        connectionServer();
                        
                                
        }
        else { 
                        Serial.println("");
                        Serial.println("Error de conexion");
                        modoconf();
                        
                        
        }
}

//--------------------------------------------------------------
WiFiClient espClient;
WebServer server(80);
//--------------------------------------------------------------


//-------------------PAGINA DE CONFIGURACION--------------------
void paginaconf() {
        server.send(200, "text/html", pagina + mensaje + paginafin); 
}

//--------------------MODO_CONFIGURACION------------------------
void modoconf() {
         
        WiFi.softAP(ssidConf);
        IPAddress myIP = WiFi.softAPIP(); 
        Serial.print("IP del acces point: ");
        Serial.println(myIP);
        Serial.println("WebServer iniciado...");

        server.on("/", paginaconf); //esta es la pagina de configuracion

        server.on("/guardar_conf", guardar_conf); //Graba en la eeprom la configuracion

        server.on("/escanear", escanear); //Escanean las redes wifi disponibles
        
        server.begin();

        while (true) {
                        server.handleClient();
        }
}

//---------------------GUARDAR CONFIGURACION-------------------------
void guardar_conf() {
        
        Serial.println(server.arg("ssid"));//Recibimos los valores que envia por GET el formulario web
        grabar(0,server.arg("ssid"));
        //Serial.println(server.arg("pass"));
        grabar(50,server.arg("pass"));

        mensaje = "Configuracion Guardada...";
        paginaconf();
}

//----------------Función para grabar en la EEPROM-------------------
void grabar(int addr, String a) {
        int tamano = a.length(); 
        char inchar[50]; 
        a.toCharArray(inchar, tamano+1);
        for (int i = 0; i < tamano; i++) {
                EEPROM.write(addr+i, inchar[i]);
        }
        for (int i = tamano; i < 50; i++) {
                EEPROM.write(addr+i, 255);
        }
        EEPROM.commit();
}

//-----------------Función para leer la EEPROM------------------------
String leer(int addr) {
         byte lectura;
         String strlectura;
         for (int i = addr; i < addr+50; i++) {
                        lectura = EEPROM.read(i);
                        if (lectura != 255) {
                                strlectura += (char)lectura;
                        }
         }
         return strlectura;
}

//---------------------------ESCANEAR----------------------------
void escanear() {  
        int n = WiFi.scanNetworks(); //devuelve el número de redes encontradas
        Serial.println("escaneo terminado");
        if (n == 0) { //si no encuentra ninguna red
                Serial.println("no se encontraron redes");
                mensaje = "no se encontraron redes";
        }  
        else
        {
                Serial.print(n);
                Serial.println(" redes encontradas");
                mensaje = "";
                for (int i = 0; i < n; ++i)
                {
                        // agrega al STRING "mensaje" la información de las redes encontradas 
                        mensaje = (mensaje) + "<p>" + String(i + 1) + ": " + WiFi.SSID(i) +
                                " (" + WiFi.RSSI(i) + ") Ch: " + WiFi.channel(i) + " Enc: " +
                                WiFi.encryptionType(i) + " </p>\r\n";
                        //WiFi.encryptionType 5:WEP 2:WPA/PSK 4:WPA2/PSK 7:open network 8:WPA/WPA2/PSK
                        delay(10);
                }
                Serial.println(mensaje);
                //();
        }
}

//------------------------SETUP-----------------------------
void setupInitialIoT() {
        
        // Inicia Serial
        Serial.begin(115200);
        Serial.println("");

        EEPROM.begin(512);

        leer(0).toCharArray(ssid, 50);
        leer(50).toCharArray(pass, 50);

        setup_wifi();
        
}
