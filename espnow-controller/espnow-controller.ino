#include <Arduino.h>
#include <ESP8266WiFi.h>
extern "C" {
#include <espnow.h>
#include <user_interface.h>
}

#define WIFI_DEFAULT_CHANNEL 10

//uint8_t no1[] = {0x1A, 0xFE, 0x34, 0xEE, 0xC9, 0x40}; // esp number 1 - master 0x1A,0xFE,0x34,0xDA,0xF0,0x99
uint8_t no2[] = {0x1A, 0xFE, 0x34, 0xF1, 0xD2, 0x9}; // esp number 2 - slave
//uint8_t no2[] = {0x18, 0xFE, 0x34, 0xF1, 0xD2, 0x9};
//uint8_t no3[] = {0x1A, 0xFE, 0x34, 0xEE, 0xCD, 0x15}; // esp number 3 - slave
//uint8_t no4[] = {0x5E, 0xCF, 0x7F, 0x9, 0x9A, 0xFE}; // esp number 4 - slave


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

  //  wifi_get_macaddr(SOFTAP_IF, macaddr);
  //  Serial.print("mac address (SOFTAP_IF): ");
  //  printMacAddress(macaddr);

  if (esp_now_init() == 0) {
    Serial.println("init");
  } else {
    Serial.println("init failed");
    ESP.restart();
    return;
  }

  Serial.println("ESP_NOW_ROLE_CONTROLLER");
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  //  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb([](uint8_t *macaddr, uint8_t *data, uint8_t len) {
    Serial.println("recv_cb");
    Serial.print("mac address: ");
    printMacAddress(macaddr);
    Serial.print("data: ");
    for (int i = 0; i < len; i++) {
      Serial.print(data[i], HEX);
    }
    Serial.print("  ");

    for(int _i=0; _i<6; _i++) {
      Serial.print(macaddr[_i]);
      Serial.print(",");
    }
    Serial.print("Data = ");
    Serial.println(data[0]);
  });

  esp_now_register_send_cb([](uint8_t* macaddr, uint8_t status) {
    //    Serial.print("send_cb");
    //
    //    Serial.print("mac address: ");
    //    printMacAddress(macaddr);

    //    Serial.print(" status = "); Serial.println(status);
  });

  //  int res = esp_now_add_peer(no2, (uint8_t)ESP_NOW_ROLE_CONTROLLER, (uint8_t)WIFI_DEFAULT_CHANNEL, NULL, 0);
  int res2 = esp_now_add_peer(no2, (uint8_t)ESP_NOW_ROLE_SLAVE, (uint8_t)WIFI_DEFAULT_CHANNEL, NULL, 0);

  //  int res3 = esp_now_add_peer(no3, (uint8_t)ESP_NOW_ROLE_SLAVE, (uint8_t)WIFI_DEFAULT_CHANNEL, NULL, 0);
  //  int res4 = esp_now_add_peer(no4, (uint8_t)ESP_NOW_ROLE_SLAVE, (uint8_t)WIFI_DEFAULT_CHANNEL, NULL, 0);

  // ESP.deepSleep(2.5e6, WAKE_RF_DEFAULT);


  //  esp_now_unregister_recv_cb();
  //  esp_now_deinit();
}

uint8_t message[] = { 0x00, 0x01 };
void loop() {
  //  if (digitalRead(0) == 0)  {
  //    delay(200);
  //    message = 1;
  //    esp_now_send(NULL, &message[0], 1);
  //    Serial.println("Button press");
  //  } else {
  //    delay(200);
  //    message = 0;
  //  esp_now_send(no2, &message[1], 1);
  //    Serial.println("Button not press");
  //  }

  esp_now_send(no2, &message[0], 1);
  delay(1000);
}

