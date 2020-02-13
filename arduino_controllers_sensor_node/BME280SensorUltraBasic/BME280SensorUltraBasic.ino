#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1018.0)

Adafruit_BME280 bme;

float temperature, humidity, pressure, altitude;        
 
void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  Serial.println();

  bme.begin(0x60);   

}
void loop() {
  delay(500);
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F;
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  Serial.println("NEW MEASURE");
  Serial.print("Temperatura: ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("Pressure: ");
  Serial.println(pressure);
  Serial.print("Altitude: ");
  Serial.println(altitude);
}
