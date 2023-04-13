#include <SPI.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

const int BME_CS = 20;
const int BME_SCK = 8;
const int BME_MOSI = 10;
const int BME_MISO = 9;

const char ssid[] = "Home";
const char pass[] = "Masuda_House_";
const IPAddress ip(192, 168, 10, 31);
const IPAddress gateway(192, 168, 10, 1);
const IPAddress subnet(255, 255, 255, 0);
WebServer server(80);

Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);

void setup() {
  Serial.begin(115200);
  bme.begin();

  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());

  server.on("/", handleGET);
  server.begin();
  Serial.println("Server start!");
}

void loop() {
  server.handleClient();
}

void handleGET() {
  String metrics;
  metrics = "# HELP kokoa_home_bme280_temp templature of kokoa home bme280\n";
  metrics += "# TYPE kokoa_home_bme280_temp gauge\n";
  metrics += "kokoa_home_bme280_temp " + String(bme.readTemperature()) + "\n";
  metrics += "# HELP kokoa_home_bme280_humid Humidity of kokoa home bme280\n";
  metrics += "# TYPE kokoa_home_bme280_humid gauge\n";
  metrics += "kokoa_home_bme280_humid " + String(bme.readHumidity()) + "\n";
  metrics += "# HELP kokoa_home_bme280_pres pressure of kokoa home bme280\n";
  metrics += "# TYPE kokoa_home_bme280_pres gauge\n";
  metrics += "kokoa_home_bme280_pres " + String(bme.readPressure() / 100.0) + "\n";
  server.send(200, "text/plain", metrics);
}
