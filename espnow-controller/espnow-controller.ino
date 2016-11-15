#include <Arduino.h>
#include <ESP8266WiFi.h>
extern "C" {
#include <espnow.h>
#include <user_interface.h>
}

#define WIFI_DEFAULT_CHANNEL 10

uint8_t no1[] = {0x1A, 0xFE, 0x34, 0xF1, 0xD2, 0x9}; // esp slave 1
uint8_t no2[] = {0x1A, 0xFE, 0x34, 0xEE, 0xC9, 0x40}; // esp slave 2


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
  pinMode(0, INPUT_PULLUP);

  Serial.begin(115200);
  Serial.println("Initializing... CONTROLLER");

  WiFi.mode(WIFI_STA);

  uint8_t macaddr[6];
  wifi_get_macaddr(STATION_IF, macaddr);
  Serial.print("mac address (STATION_IF): ");
  printMacAddress(macaddr);

  if (esp_now_init() == 0) {
    Serial.println("init");
  } else {
    Serial.println("init failed");
    ESP.restart();
    return;
  }

  Serial.println("ESP_NOW_ROLE_CONTROLLER");
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_recv_cb([](uint8_t *macaddr, uint8_t *data, uint8_t len) {
    Serial.println("recv_cb");
    Serial.print("mac address: ");
    printMacAddress(macaddr);

    Serial.print("data: ");
    for (int i = 0; i < len; i++) {
      Serial.print(data[i], HEX);
    }
    Serial.print("  ");

    for (int _i = 0; _i < 6; _i++) {
      Serial.print(macaddr[_i]);
      if (i < 5) Serial.print(',');
    }
  });

  esp_now_register_send_cb([](uint8_t* macaddr, uint8_t status) {
    //    Serial.print("send_cb");
    //    Serial.print("mac address: ");
    //    printMacAddress(macaddr);
    //    Serial.print(" status = "); Serial.println(status);
  });

  int res2 = esp_now_add_peer(no1, (uint8_t)ESP_NOW_ROLE_SLAVE, (uint8_t)WIFI_DEFAULT_CHANNEL, NULL, 0);
  int res3 = esp_now_add_peer(no2, (uint8_t)ESP_NOW_ROLE_SLAVE, (uint8_t)WIFI_DEFAULT_CHANNEL, NULL, 0);


  // ESP.deepSleep(2.5e6, WAKE_RF_DEFAULT);
  //  esp_now_unregister_recv_cb();
  //  esp_now_deinit();
}

void loop() {

  //  esp_now_send(no2, &message[0], 1);
  delay(1000);
}

