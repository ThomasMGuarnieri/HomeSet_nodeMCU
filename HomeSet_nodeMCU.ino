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

/* Variaveis de controle */
unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
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

  /* Credenciais de login */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Configurações API Firebase */
  config.api_key = api_key;
  config.database_url = database_url;
  config.token_status_callback = tokenStatusCallback;
  config.max_token_generation_retry = 5;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}
void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    FirebaseJson json;
    json.add("devices_id", 1);

    Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, "device_log/devices_id", &json) ? "ok" : fbdo.errorReason().c_str());

    /*
      Firebase.RTDB.setInt(&fbdo, "device_log/devices_id", testCount)
      Firebase.RTDB.setInt(&fbdo, "device_log/power", testCount)
      Firebase.RTDB.setInt(&fbdo, "device_log/requested_power_state", testCount)
      Firebase.RTDB.setInt(&fbdo, "device_log/change", testCount)
    */
  }
}
