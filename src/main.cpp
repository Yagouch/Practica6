#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9 //Pin 9 para el reset del RC522
#define SS_PIN 10 //Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); //Creamos el objeto para el RC522

void handle_root();

// SSID & Password
const char* ssid = "*****";  // Enter your SSID here
const char* password = "*****";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)



void setup() {
  Serial.begin(115200);

SPI.begin(); //Iniciamos el Bus SPI
mfrc522.PCD_Init(); // Iniciamos el MFRC522
Serial.println("Lectura del UID");

  Serial.println("Try Connecting to ");
  Serial.println(ssid);

  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected successfully");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/", handle_root);

  server.begin();
  Serial.println("HTTP server started");
  delay(100); 
}

void loop() {
  // Revisamos si hay nuevas tarjetas presentes
if ( mfrc522.PICC_IsNewCardPresent())
{
//Seleccionamos una tarjeta
if ( mfrc522.PICC_ReadCardSerial())
{
// Enviamos serialemente su UID
Serial.print("Card UID:");
for (byte i = 0; i < mfrc522.uid.size; i++) {
Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
Serial.print(mfrc522.uid.uidByte[i], HEX);
}
Serial.println();
// Terminamos la lectura de la tarjeta actual
mfrc522.PICC_HaltA();
}
}

  server.handleClient();
}

// HTML & CSS contents which display on web server
String HTML = "<!DOCTYPE html> "
"<html> "
"<body> "
"<h1>My Primera Pagina con ESP32 - Station Mode &#128522;</h1>\ "
"<p> Codigo RFID: "  mfrc522.uid.uidByte[0]  " </p> "
"</body> "
"</html> ";

// Handle root url (/)
void handle_root() {
  server.send(200, "text/html", HTML);
}