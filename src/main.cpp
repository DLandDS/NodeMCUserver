#include <ESP8266WiFi.h>
#include <WiFiManager.h>

#define SendKey 0  //Button to send data Flash BTN on NodeMCU

int port = 8888;  //Port number
WiFiServer server(port);

//Server connect to WiFi Network
const char *ssid = "NodeMCU Server";
const char *password = "12345678";

void setup(){
	Serial.begin(115200);
	pinMode(BUILTIN_LED, OUTPUT);  //Btn to send data

	// Wait for connection  
	WiFi.mode(WIFI_STA);
	WiFiManager wm;
	bool res;
	res = wm.autoConnect(ssid,password);
	if(!res) {
		Serial.println("Failed to connect");
	} 
	else { 
		Serial.println("connected...yeey :)");
	}

	Serial.println("");
	Serial.print("Connected to ");
	Serial.println(ssid);

	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());  
	server.begin();
	Serial.print("Open Telnet and connect to IP:");
	Serial.print(WiFi.localIP());
	Serial.print(" on port ");
	Serial.println(port);
}

void loop() {
	WiFiClient client = server.available();

	if (client) {
		if(client.connected()){
			Serial.println("Client Connected");
		}

		while(client.connected()){      
			while(client.available()>0){
				// read data from the connected client
				int bufferreader = client.read();
				if(bufferreader == '0'){
					digitalWrite(BUILTIN_LED, LOW);
				} else if(bufferreader == '1'){
					digitalWrite(BUILTIN_LED, HIGH);
				} else {
					Serial.println(bufferreader);
				}
			}
			//Send Data to connected client
			char const *status;
			if(digitalRead(BUILTIN_LED)==LOW){
				status = "0";
			} else if(digitalRead(BUILTIN_LED)==HIGH){
				status = "1";
			}
			client.write(status);
		}
		client.stop();
		Serial.println("Client disconnected");    
	}
}