#include <ESP8266WiFi.h>
#include <vector>
#define port 8888
WiFiServer server(port);
std::vector<WiFiClient> clients;

void actionHandler(WiFiClient &client){
	client = server.available();
	if (client) {
		if(client.connected()){
			Serial.println("Client Connected");
		}
		while(client.connected()){      
			while(client.available()>0){
				// read data from the connected client
				int bufferreader = client.read();
				if(bufferreader == '0'){
					digitalWrite(BUILTIN_LED, HIGH);
				} else if(bufferreader == '1'){
					digitalWrite(BUILTIN_LED, LOW);
				} else if(bufferreader == 's'){
					char const *status;
					if(digitalRead(BUILTIN_LED)==LOW){
						status = "1\n";
					} else if(digitalRead(BUILTIN_LED)==HIGH){
						status = "0\n";
					}
					client.write(status);
					Serial.println("Data Requested");
				} 
				Serial.println(bufferreader);	
			}
		}
		client.stop();
		Serial.println("Client disconnected");    
	}
}