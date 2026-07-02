#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

#define DHTPIN D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// WiFi Details
const char* ssid = "pavi";
const char* password = "veeralak";

// ThingSpeak Details
unsigned long channelID = 3417238;      // Example: 3416575
const char* writeAPIKey = "099PC9B9R3ALTP4Y";

WiFiClient client;

void setup()
{
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);
}

void loop()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity))
  {
    Serial.println("DHT11 Error");
    delay(2000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  int response = ThingSpeak.writeFields(channelID, writeAPIKey);

  if (response == 200)
  {
    Serial.println("Data uploaded successfully.");
  }
  else
  {
    Serial.print("Upload Failed. Error Code: ");
    Serial.println(response);
  }

  Serial.println("-----------------------------");

  delay(20000);
}