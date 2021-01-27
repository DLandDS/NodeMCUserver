#include <ESP8266WiFi.h>
#include <vector>
#define port 8888
WiFiServer server(port);
std::vector<WiFiClient> clients;

void actionHandler(WiFiClient &client){
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
	}
}

void printCountClient(){
	Serial.printf("Connected : %d\n", clients.size());
}

void printStatus(WiFiClient &client){
	Serial.printf("(%s = %s)\n",
	client.remoteIP().toString().c_str(),
	client.connected()?"Connected":"Disconnected");
}