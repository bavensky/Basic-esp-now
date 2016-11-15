#include <Arduino.h>
#include <ESP8266WiFi.h>
extern "C" {
#include <espnow.h>
#include <user_interface.h>
}

#define WIFI_DEFAULT_CHANNEL 10

uint8_t no1[] = {0x18, 0xFE, 0x34, 0xDB, 0x3D, 0x22};


void printMacAddress(uint8_t* macaddr) {
  Serial.print("{");
  for (int i = 0; i < 6; i++) {
    Serial.print("0x");
    Serial.print(macaddr[i], HEX);
    if (i < 5) Serial.print(',');
  }
  Serial.println("}");
}

void setup() {
  WiFi.disconnect();
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("Initializing... SLAVE");

  WiFi.mode(WIFI_AP_STA);
  //  WiFi.mode(WIFI_AP);
  //  WiFi.mode(WIFI_STA);

  uint8_t macaddr[6];
  //    wifi_get_macaddr(STATION_IF, macaddr);
  //    Serial.print("mac address (STATION_IF): ");
  //    printMacAddress(macaddr);

  wifi_get_macaddr(SOFTAP_IF, macaddr);
  Serial.print("mac address (SOFTAP_IF): ");
  printMacAddress(macaddr);

  if (esp_now_init() == 0) {
    Serial.println("init");
  } else {
    Serial.println("init failed");
    ESP.restart();
    return;
  }

  Serial.println("ESP_NOW_ROLE_SLAVE");
  //  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb([](uint8_t *macaddr, uint8_t *data, uint8_t len) {

    //    if (data[0] == 1)  {
    //      digitalWrite(LED_BUILTIN, LOW);
    //      digitalWrite(RELAY, !digitalRead(RELAY));
    //    } else {
    //      digitalWrite(LED_BUILTIN, HIGH);
    //    }
    //    digitalWrite(LED_BUILTIN, !data[0]);

    Serial.print("Data = ");
    Serial.println(data[0]);
  });

  esp_now_register_send_cb([](uint8_t* macaddr, uint8_t status) {
    Serial.println("send_cb");

    Serial.print("mac address: ");
    printMacAddress(macaddr);

    Serial.print("status = "); Serial.println(status);
  });

  int res = esp_now_add_peer(no1, (uint8_t)ESP_NOW_ROLE_CONTROLLER, (uint8_t)WIFI_DEFAULT_CHANNEL, NULL, 0);
  //  int res2 = esp_now_add_peer(no1, (uint8_t)ESP_NOW_ROLE_SLAVE, (uint8_t)WIFI_DEFAULT_CHANNEL, NULL, 0);


  //  esp_now_unregister_recv_cb();
  //  esp_now_deinit();
}

uint8_t message[] = { 0x00, 0x01 };
void loop() {
  //  yield();
  esp_now_send(no1, &message[0], 1);
  delay(1000);
}

