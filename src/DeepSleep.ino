#include <DHT.h>
#include <DHT_U.h>

#include <UbiConstants.h>
#include <UbidotsEsp32Mqtt.h>
#include <UbiTypes.h>

#include <TFT_eSPI.h>
#include <SPI.h>

#define DHTPIN 27
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
TFT_eSPI tft = TFT_eSPI(135, 240);

const char *UBIDOTS_TOKEN = "BBUS-kkMxuAFp6c0ZtTNrdh1yFgrKARnDev";  // Put here your Ubidots TOKEN
const char *WIFI_SSID = "FAJARDO";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "43208186";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "esp32-ssf";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL1 = "Temperature"; // Put here your Variable label to which data  will be published
const char *VARIABLE_LABEL2 = "Humedad";
const char *VARIABLE_LABEL3 = "Switch1";
const char *VARIABLE_LABEL4 = "Switch2";
int TIME_TO_SLEEP = 2;
unsigned long long uS_TO_S_FACTOR = 1000000;


const int PUBLISH_FREQUENCY = 2000; // Update rate in milliseconds

unsigned long timer;

Ubidots ubidots(UBIDOTS_TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
  // put your setup code here, to run once:
  tft.init();
  dht.begin();
  tft.fillScreen(TFT_BLACK);
  Serial.begin(115200);
  // ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  
  float hum = dht.readHumidity();
  float temp = dht.readTemperature();
  
  ubidots.add(VARIABLE_LABEL1, temp); // Insert your variable Labels and the value to be sent
  ubidots.add(VARIABLE_LABEL2, hum);
  ubidots.publish(DEVICE_LABEL);
  /*
  tft.fillRect(0,0,135,166,TFT_BLACK);
  tft.drawString("Temp. [ C]", 10, 10, 4);
  tft.drawString("o", 100, 5);
  tft.setTextColor(TFT_GREENYELLOW);
  tft.drawString(String(temp,1), 10, 40, 6);

  tft.setTextColor(TFT_WHITE);
  tft.drawString("Humedad", 10, 90, 4);
  tft.setTextColor(TFT_GREENYELLOW);
  tft.drawString(String(hum,1), 10, 120, 6);

  tft.setTextColor(TFT_WHITE);
  */
  ubidots.loop();
  
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  esp_deep_sleep_start();
}

void loop() {

}
