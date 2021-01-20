#include <ESP8266WiFi.h>
#include <WiFiManager.h>

#define SendKey 0  //Button to send data Flash BTN on NodeMCU

int port = 8888;  //Port number
WiFiServer server(port);

//Server connect to WiFi Network
const char *ssid = "---------";  //Enter your wifi SSID
const char *password = "--------";  //Enter your wifi Password

int count=0;
//=======================================================================
//                    Power on setup
//=======================================================================
void setup(){
	Serial.begin(115200);
	pinMode(SendKey,INPUT_PULLUP);  //Btn to send data
	Serial.println();

	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password); //Connect to wifi

	// Wait for connection  
	WiFi.mode(WIFI_STA);
	WiFiManager wm;
	bool res;
	res = wm.autoConnect("AutoConnectAP","password");
	if(!res) {
		Serial.println("Failed to connect");
		// ESP.restart();
	} 
	else {
		//if you get here you have connected to the WiFi    
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
//=======================================================================
//                    Loop
//=======================================================================

void loop() {
	WiFiClient client = server.available();

	if (client) {
		if(client.connected()){
			Serial.println("Client Connected");
		}

		while(client.connected()){      
			while(client.available()>0){
				// read data from the connected client
				Serial.write(client.read());
			}
			//Send Data to connected client
			while(Serial.available()>0){
				client.write(Serial.read());
			}
		}
		client.stop();
		Serial.println("Client disconnected");    
	}
}
//=======================================================================