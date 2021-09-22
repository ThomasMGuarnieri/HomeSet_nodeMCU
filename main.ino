#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>

/* Auxilia nas questões referentes ao token */
#include "addons/TokenHelper.h"
/* Provide the RTDB payload printing info and other helper functions. */
#include "addons/RTDBHelper.h"

#define USER_EMAIL "tester@tester.com"
#define USER_PASSWORD "tester"

/* Credenciais de acesso a rede Wifi */
const char *ssid = "MG_Pedras";
const char *password = "99787568";

/* Firebase API Key */
const char *api_key = "AIzaSyDLp3EJm1xFVZhOVM4_9o3c6w1zqNa3QRo";

/* Firebase url do banco de dados */
const char *database_url = "https://homeset-b4916-default-rtdb.firebaseio.com/";

/* Firebase data object */
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
int testCount = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Conectando a rede Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }

  /* Mostra o IP local no console */
  Serial.println();
  Serial.print("Conectado com o IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Credenciais de login */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Configurações API Firebase */
  config.api_key = api_key;
  config.database_url = database_url;
  config.token_status_callback = tokenStatusCallback;
  config.max_token_generation_retry = 5;

  /* Sign up */
  /*if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("Firebase singUp OK");
    signupOK = true;
  } else{
    Serial.printf("SingUp failed");
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }*/

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}
void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    
    /* Escreve um numero do tipo Int no banco de dados, no caminho test/int */
    if (Firebase.RTDB.setInt(&fbdo, "test/int", testCount)){
      Serial.println("INSERTED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    } else {
      Serial.println("FAILED");
      Serial.println("ERROR: " + fbdo.errorReason());
    }
    testCount++;
  }
} 
