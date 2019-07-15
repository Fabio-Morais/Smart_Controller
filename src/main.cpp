#include <Arduino.h>

#include <Wire.h>
#include "SSD1306Wire.h"     
#include "images.h"
#include "functions.h"
#include <string.h>
#include <millisDelay.h>

#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Time.h>

#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

#include <NTPClient.h>
#include <WiFiUdp.h>

//for LED status
#define botaoD 14
#define botaoE 12
#define rele 13
#define led 15


// Define NTP Client to get time
const long utcOffsetInSeconds = 3600;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'
ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80



int progress = 0;

SSD1306Wire display(0x3d, 5, 4);

#define DEMO_DURATION 3000
typedef void (*Demo)(void);

int demoMode = 0;
int counter = 1;

//Define variables
const char *nameCity;
time_t dateTime;
char dateString[20];
char timeString[20];
const char *windSpeed;
const char *weather;
const char *temperatura;
const char *pressure;
const char *humidity;
const char *temp_min;
const char *temp_max;
char idCity[20]="8010483";
char userName[100]="critical";
char nameCity_aux[30];

char time_str[20];
int tempo_int=12;

int flag_username=0;

/*
0-> auto
1-> manual
*/
int switch_man_aut=0; 

millisDelay ledDelay;
millisDelay interruptDelay;
millisDelay bomDia;


int pag=0;
int flag=0;
/********/
/*      FUNÇÕES    */

void desenhaHumidity(const char *humidity){
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(10, 0, "Humidade");
    display.setFont(ArialMT_Plain_16);
    display.drawString(22, 16, humidity);
  }
 
  void desenhaWindSpeed(const char *windSpeed){
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(75, 0, "V. Vento");
    display.setFont(ArialMT_Plain_16);
    display.drawString(85, 16, windSpeed);
  }
  void desenhaPressure(const char *pressure){
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(42, 32, "Pressão");
    display.setFont(ArialMT_Plain_16);
    display.drawString(44, 47, pressure);
  }
  void desenhaTempMin(const char *tempMin){
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(10, 12, "Temp. Min");
    display.setFont(ArialMT_Plain_16);
    char tempMin_aux[10];
    strcpy(tempMin_aux, tempMin);
    char *token = strtok(tempMin_aux, ".");
    strcpy(tempMin_aux, token);
    strcat(tempMin_aux, "°C");
    display.drawString(80, 10, tempMin_aux);
  }
  void desenhaTempMax(const char *tempMax){
    display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.setFont(ArialMT_Plain_10);
      display.drawString(10, 47, "Temp. Max");
      display.setFont(ArialMT_Plain_16);
      char tempMax_aux[10];
      strcpy(tempMax_aux, tempMax);
      char *token = strtok(tempMax_aux, ".");
      strcpy(tempMax_aux, token);
      strcat(tempMax_aux, "°C");
      display.drawString(80, 45, tempMax_aux);
  }
 
  void temp(const char *temperatura){
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_24);
    display.drawString(8, 20, temperatura); 
  }

void drawProgressBar() {
      progress = (counter / 5) % 100;
      // draw the progress bar
      display.drawProgressBar(23, 40, 80, 5, progress);

      // draw the percentage as String
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(64, 48, String(progress) + "%");
}

/********/
/*      PAGINAS     */

void Pagina_Inicial(){
    if(progress<=60)
      display.drawXbm(8, 20 , smart_width, smart_height, smart_bits);
    else if(progress>60){
      display.drawXbm(8, 0 , critical_width, critical_height, critical_bits);
    }
    drawProgressBar();

}
void Pagina_Bem_Vindo(){
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  char bem_vindo[80];
  strcpy(bem_vindo, "Bem-vindo");
  display.drawString(63, 6, bem_vindo);
  if(strlen(userName) < 15)
    display.setFont(ArialMT_Plain_24);
  else
    display.setFont(ArialMT_Plain_16);

  display.drawString(66 - (strlen(userName))/2, 35, userName);
  display.drawHorizontalLine(15, 30, 100);
}

void Pagina1(const char *hour, const char *date, const char *city){
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(100, 52, hour);    //10-7-2019
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 26, city);
  
  char dia_aux[3];
  char mes_aux[3];
  char ano[5];
  char data[50];
  strcpy(dia_aux, date);
  char *token = strtok(dia_aux, "-");
  strcpy(dia_aux, token);
  display.setFont(ArialMT_Plain_10);
  token = strtok(NULL, "-");
  strcpy(mes_aux, token);
  token = strtok(NULL, "-");
  strcpy(ano,token);
  strcat(data, wd( atoi(ano) , atoi(mes_aux), atoi(dia_aux) ));
  strcat(data, ", ");
  strcat(data, dia_aux);
  strcat(data, " de ");
  strcat(data, mes[atoi(mes_aux) + 1]); 
  display.drawString(0, 8, data);

}

void Pagina2(const char *weather, const char *temperatura){

  if(tempo_int > 20 || (tempo_int>= 0 && tempo_int <6) )
    display.drawXbm(70, 10, moon_width, moon_height, moon_bits);
  else if((!strcmp(weather, "Clouds") || !strcmp(weather, "Few clouds") || !strcmp(weather, "Atmosphere")) && tempo_int < 20)
    display.drawXbm(70, 20, cloud_width, cloud_height, cloud_bits);
  else if(!strcmp(weather, "Clear"))
    display.drawXbm(70, 8, sun_width, sun_height, sun_bits);
  else if(!strcmp(weather, "Rain"))
    display.drawXbm(70, 14, rain_width, rain_height, rain_bits);
  else if(!strcmp(weather, "Snow"))
    display.drawXbm(75, 8, rain_width, rain_height, snow_bits);
  else 
    display.drawXbm(70, 10, moon_width, moon_height, moon_bits);
  Serial.println(weather);
  /**/
  char temperatura_aux[10];
  strcpy(temperatura_aux, temperatura);
  char *token = strtok(temperatura_aux, ".");
  strcpy(temperatura_aux, token);
  strcat(temperatura_aux, "°C");
  /******/
  temp(temperatura_aux);
}

void Pagina3(const char *tempMin, const char *tempMax){
  desenhaTempMax(tempMax);
  desenhaTempMin(tempMin);
}
 
void Pagina4(const char *humidity, const char *windSpeed, const char *pressure){
  desenhaHumidity(humidity);
  desenhaWindSpeed(windSpeed);
  desenhaPressure(pressure);
}

void displayPag(){
  if(pag==5){
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    Pagina_Bem_Vindo();
    display.display();
    if(bomDia.justFinished())
      {
        pag=1;
        flag_username=0;
        flag=1;
      }
  }
  else if(pag==1){
    Pagina1(timeString, dateString, nameCity_aux);
    //pag=2;
  }
  else if(pag==2){
    Pagina2(weather,temperatura);
    //pag=3;
  }
  else if(pag==3){
    Pagina3(temp_min, temp_max);
    //pag=4;
  }else if(pag==4){
    Pagina4(humidity, windSpeed, pressure);
    //pag=1;
  }
}


ICACHE_RAM_ATTR void pagE(){
  if(interruptDelay.justFinished() && flag_username==0) {
     if(pag>1)
      pag--;
    else
      pag=4;
  interruptDelay.start(200);
  flag=1;
  Serial.println("Esquerda");
   }

    Serial.println("Esquerda sem entrar");
  

}

ICACHE_RAM_ATTR void pagD(){
if(interruptDelay.justFinished() && flag_username==0) {
    if(pag<4)
      pag++;
    else
      pag=1;  
 interruptDelay.start(200);
 flag=1;
 Serial.println("Direita");
   }
Serial.println("Direita sem entrar");
}


void handleRoot();              // function prototypes for HTTP handlers
void handleNotFound();

void handleRoot() {
  server.send(200, "text/plain", "Hello world!");   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

/*    IMPRIMIR BOM DIA     */
void handleOptions() {
  server.arg("idcity").toCharArray(idCity, server.arg("idcity").length() + 1);
  Serial.println("idcity: "); Serial.println(idCity);
  server.arg("username").toCharArray(userName, server.arg("username").length() + 1);
  Serial.println("username: "); 
  Serial.println(userName);
  if(!strcmp(userName, ""))
    strcpy(userName,"critical");
  flag_username=1;
  pag=5;
  bomDia.start(1000);//5s
  server.send(200, "text/plain", "Recebido cidade e username");       //Response to the HTTP request
}
int flagVentoinha=0;
int flagLed=0;
/*ventoinha*/
void handleVentoinha() {
  if(digitalRead(rele) == LOW && switch_man_aut==1)
    digitalWrite(rele, HIGH);
  else if(digitalRead(rele) == HIGH && switch_man_aut==1)
    digitalWrite(rele, LOW);
  
  Serial.println("ar condicionado ok");
  server.send(200, "text/plain", "Recebido ventoinha");       //Response to the HTTP request
}

/*leds*/
void handleLights() {
  if(digitalRead(led) == LOW && switch_man_aut==1)
    digitalWrite(led, HIGH);
  else if(digitalRead(led) == HIGH && switch_man_aut==1)
    digitalWrite(led, LOW);
  Serial.println("leds ok");
  server.send(200, "text/plain", "Recebido leds");       //Response to the HTTP request
}

void handleSwitch() {
  if(switch_man_aut)
    switch_man_aut=0;
  else
    switch_man_aut=1;
  Serial.println("switch");
  Serial.print("Switch modo: ");
  Serial.print(switch_man_aut);
  Serial.println();

}



void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}


void setup() {
  Serial.begin(115200);

// put your setup code here, to run once:
  Serial.begin(115200);
  // Hours
  timeClient.begin();
  wifiMulti.addAP("CSW-Guest", "Critical98");   // add Wi-Fi networks you want to connect to

  Serial.println("Connecting ...");

  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

  if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }
 
  server.on("/", HTTP_POST, handleOptions);       // Call the 'handleConfiguration' function when a client requests URI "/"
  server.on("/ventoinha", HTTP_GET, handleVentoinha);       // Call the 'handleVentoinha' function when a client requests URI "/"
  server.on("/led", HTTP_GET, handleLights);       // Call the 'handleLights' function when a client requests URI "/"
  server.on("/switch", HTTP_GET, handleSwitch);       // Call the 'handleVentoinha' function when a client requests URI "/"
  

  server.onNotFound(handleNotFound);              // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");

  //set led pin as output
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(botaoE, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(botaoE), pagE, RISING);
  pinMode(botaoD, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(botaoD), pagD, RISING);
  /*OUTPUT*/
  pinMode(led, OUTPUT);
  pinMode(rele, OUTPUT);

  digitalWrite(rele, HIGH);
  digitalWrite(led, LOW);




    //keep LED on
  digitalWrite(BUILTIN_LED, LOW);
    // Inicialização do Display
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  
  ledDelay.start(2000);  // start a 10sec delay
  interruptDelay.start(10);
  
}
int temp_ventoinha=0;
void loop()
{
  server.handleClient();                    // Listen for HTTP requests from clients
  timeClient.update(); 

  if(temp_ventoinha >= 22 && switch_man_aut==0)
    digitalWrite(rele, LOW);
  else if(temp_ventoinha < 22 && switch_man_aut==0)
    digitalWrite(rele, HIGH);
  
  if((tempo_int >= 20 || (tempo_int>=0 && tempo_int <=9)) && switch_man_aut==0)
    digitalWrite(led, HIGH);
  else if(tempo_int < 20 && tempo_int >9 && switch_man_aut==0)
    digitalWrite(led, LOW);
  

  if (pag == 0)
  {
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    Pagina_Inicial();
    display.display();
    counter++;
    delay(10);
    if (progress == 99)
      {
        pag = 5;
        bomDia.start(3000);
        ledDelay.start(30);
      }
    //pag=1;
  }
  else
  {
    if (ledDelay.justFinished() || flag == 1 || flag_username==1)
    { // only returns TRUE once.
    Serial.println(tempo_int);
      flag = 0;
      ledDelay.start(10000); //10s

      if (WiFi.status() == WL_CONNECTED)
      {
        HTTPClient http; //Object of class HTTPClient

        char endpoint[100];
        sprintf(endpoint, "http://openweathermap.org/data/2.5/weather?id=%d&appid=b6907d289e10d714a6e88b30761fae22",atoi(idCity));
        Serial.println(endpoint);
        http.begin(endpoint);
        int httpCode = http.GET();
        //Check the returning code
        if (httpCode > 0)
        {
          // Get the request response payload
          String payload = http.getString();
          // Parsing
          const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
          DynamicJsonBuffer jsonBuffer(bufferSize);
          JsonObject &root = jsonBuffer.parseObject(http.getString());
          // Get parameters from weather API
          nameCity = root["name"];
          strcpy(nameCity_aux, nameCity); 
          if(!strcmp(nameCity_aux, "Lisbon"))
            strcpy(nameCity_aux, "Lisboa");
          time_t utcCalc = root["dt"];  
          sprintf(dateString,"%d-%d-%d",day(utcCalc),month(utcCalc), year(utcCalc));  
          sprintf(timeString,"%s",timeClient.getFormattedTime().c_str());
          strncpy(time_str, timeString, 2);
          time_str[2] = '\0';
          tempo_int= atoi(time_str);
          windSpeed = root["wind"]["speed"];
          weather = root["weather"][0]["main"];
          temperatura = root["main"]["temp"];
          temp_ventoinha= atoi(temperatura);
          pressure = root["main"]["pressure"];
          humidity = root["main"]["humidity"];
          temp_min = root["main"]["temp_min"];
          temp_max = root["main"]["temp_max"];
          Serial.println(weather);
          http.end(); //Close connection
        
          // clear the display
          display.clear();
          display.setTextAlignment(TEXT_ALIGN_RIGHT);

          displayPag();
          display.display();
        }
      }
    }
  }
}