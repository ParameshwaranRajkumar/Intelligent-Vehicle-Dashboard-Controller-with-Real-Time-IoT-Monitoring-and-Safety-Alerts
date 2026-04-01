#include <WiFi.h>

const char* ssid = "vivo";
const char* password = "890890890";

String apiKey = "ETRYZGOC8AJ9LJUT";
const char* server = "api.thingspeak.com";

WiFiClient client;
HardwareSerial mySerial(2);

String data = "";

void sendToThingSpeak(String data)
{
  int first = data.indexOf(',');
  int second = data.indexOf(',', first + 1);
  int third = data.indexOf(',', second + 1);

  if (first == -1 || second == -1 || third == -1)
  {
    Serial.println("Invalid data format!");
    return;
  }

  String temp = data.substring(0, first);
  String fuel = data.substring(first + 1, second);
  String battery = data.substring(second + 1, third);
  String speed = data.substring(third + 1);

  Serial.println("Temp: " + temp);
  Serial.println("Fuel: " + fuel);
  Serial.println("Battery: " + battery);
  Serial.println("Speed: " + speed);

  if (client.connect(server, 80))
  {
    String url = "/update?api_key=" + apiKey;
    url += "&field1=" + temp;
    url += "&field2=" + fuel;
    url += "&field3=" + battery;
    url += "&field4=" + speed;

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: api.thingspeak.com\r\n" +
                 "Connection: close\r\n\r\n");

    Serial.println("Data sent to ThingSpeak!");

    // Read response
    while (client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
  }
  else
  {
    Serial.println("Connection failed!");
  }

  client.stop();
}

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600, SERIAL_8N1, 16, 17);

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
}

void loop()
{
  while (mySerial.available())
  {
    char c = mySerial.read();
    Serial.print(c); // DEBUG raw data

    if (c == '\n')
    {
      Serial.println("\nReceived: " + data);

      sendToThingSpeak(data);

      data = "";
      delay(15000); // ThingSpeak limit
    }
    else
    {
      data += c;
    }
  }
}