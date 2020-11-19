#include <PubSubClient.h>
#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic


const char* mqtt_server = "mqtt.blackspektro.com";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
char message_buff[100];



void callback(char* topic, byte* payload, unsigned int length) 
{
  int i = 0;
  Serial.println("Message arrived:  topic: " + String(topic));
  Serial.println("Length: " + String(length,DEC));
  // create character buffer with ending null terminator (string)
  for(i=0; i<length; i++) 
  {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  String msgString = String(message_buff);
  Serial.println("Payload: " + msgString);
}


void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID or use MAC address
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),"uid","password")) 
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("tester", "hello world");
      // ... and resubscribe
      client.subscribe("accon");
    }
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() 
{
  Serial.begin(9600);
  WiFiManager wifiManager;
  //1wifiManager.resetSettings(); 
  wifiManager.autoConnect("NodeMCU");
  Serial.println("Connected to network!!!");
  Serial.println();
  Serial.println();  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void loop() 
{
  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();
}
