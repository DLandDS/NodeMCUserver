#include <WiFiManager.h>
#include "clienthendler.cpp"

#define SendKey 0  //Button to send data Flash BTN on NodeMCU

//Server connect to WiFi Network
const char *ssid = "NodeMCU Server";
const char *password = "12345678";
int i;

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
	if(server.hasClient()){
		clients.push_back(server.available());
		printCountClient();
		printStatus(clients[clients.size()-1]);
		server.begin();
	}
	
	for(size_t i = 0; i < clients.size(); i++){
		if(clients[i].connected()){
			actionHandler(clients[i]);
		} else {
			printStatus(clients[i]);
			clients[i].stop();
			clients.erase(clients.begin()+i);
			printCountClient();
			i--;
		}
	}
}