#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

/* Auxilia nas questões referentes ao token */
#include "addons/TokenHelper.h"
/* Provide the RTDB payload printing info and other helper functions. */
#include "addons/RTDBHelper.h"

#define USER_EMAIL "tester@tester.com"
#define USER_PASSWORD "tester"

/* Credenciais de acesso a rede Wifi */
#define WIFI_SSID "MG_Pedras"
#define WIFI_PASSWORD "99787568"

/* Firebase API Key */
#define API_KEY "AIzaSyDLp3EJm1xFVZhOVM4_9o3c6w1zqNa3QRo"

/* Firebase url do banco de dados */
#define DATABASE_URL "https://homeset-b4916-default-rtdb.firebaseio.com/"

/* Define o pino de saida */
const int IN_PIN = D4;

/* Firebase data object */
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseJsonData result;

/* Variaveis de controle */
unsigned long sendDataPrevMillis = 0;

void setup() {
  Serial.begin(115200);

  pinMode(IN_PIN, OUTPUT);

  connectToInternet(WIFI_SSID, WIFI_PASSWORD);

  /* Credenciais de login */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Configurações API Firebase */
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;
  config.max_token_generation_retry = 5;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void connectToInternet(char *ssid, char *password) {
  WiFi.begin(ssid, password);
  Serial.print("Conectando a rede Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  
  /* Mostra o IP local no console */
  Serial.println();
  Serial.print("Conectado com o IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    FirebaseJson searchJson;

    if (Firebase.RTDB.getJSON(&fbdo, "devices/", &searchJson)) {
      
      searchJson.get(result, "-MkIPpNw2AR8JDbgCsi9/power");

      if (result.type == "boolean" && result.to<bool>() == true) {
        Serial.println("LIGADO (O dispositivo está ligado)");
        digitalWrite(IN_PIN, LOW);
      } else {
        Serial.println("DESLIGADO (O dispositivo está desligado)");  
        digitalWrite(IN_PIN, HIGH);
      }
    }

    /*
    json.set("-MkA1cAXZuQW5ITG7Hhl/power", true);
    json.set("-MkA1cAXZuQW5ITG7Hhl/time", sendDataPrevMillis);
    json.set("-MkIPpNw2AR8JDbgCsi9/power", true);
    json.set("-MkIPpNw2AR8JDbgCsi9/time", sendDataPrevMillis);

    Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, "devices_log", &json) ? "Data sent" : fbdo.errorReason().c_str());
    */
  }
}
