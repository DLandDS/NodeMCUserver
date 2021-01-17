#include <WiFiManager.h>
#include <ESPAsyncTCP.h>
#include <DNSServer.h>
#include <vector>

#include "wifiSetup.h"

static DNSServer DNS;

static std::vector<AsyncClient*> clients; // a list to hold all clients

 /* clients events */
static void handleError(void* arg, AsyncClient* client, int8_t error) {
	Serial.printf("\n connection error %s from client %s \n", client->errorToString(error), client->remoteIP().toString().c_str());
}

static void handleData(void* arg, AsyncClient* client, void *data, size_t len) {
	Serial.printf("\n data received from client %s \n", client->remoteIP().toString().c_str());
  uint8_t *message = (uint8_t*)data;
	Serial.write(message, len);
  if(strcmp((const char *)message, "0")==0){

  } else if (strcmp((const char *)message, "1")==0){
    
  } else {
    Serial.println("error reading data");
  }
  
  

	// reply to client
	if (client->space() > 32 && client->canSend()) {
    int status = digitalRead(BUILTIN_LED);
		char reply[32];
		sprintf(reply, "%d", status);
		client->add(reply, strlen(reply));
		client->send();
	}
}

static void handleDisconnect(void* arg, AsyncClient* client) {
	Serial.printf("\n client %s disconnected \n", client->remoteIP().toString().c_str());
}

static void handleTimeOut(void* arg, AsyncClient* client, uint32_t time) {
	Serial.printf("\n client ACK timeout ip: %s \n", client->remoteIP().toString().c_str());
}


/* server events */
static void handleNewClient(void* arg, AsyncClient* client) {
	Serial.printf("\n new client has been connected to server, ip: %s", client->remoteIP().toString().c_str());

	// add to list
	clients.push_back(client);
	
	// register events
	client->onData(&handleData, NULL);
	client->onError(&handleError, NULL);
	client->onDisconnect(&handleDisconnect, NULL);
	client->onTimeout(&handleTimeOut, NULL);
}

void setup() {
	Serial.begin(115200);
  Serial.println("Serial Started");
  pinMode(BUILTIN_LED, OUTPUT);
	delay(20);
	
	// create access point
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

	// start dns server
	if (!DNS.start(DNS_PORT, SERVER_HOST_NAME, WiFi.softAPIP()))
		Serial.printf("\n failed to start dns service \n");

	AsyncServer* server = new AsyncServer(TCP_PORT); // start listening on tcp port 7050
	server->onClient(&handleNewClient, server);
	server->begin();
}

void loop() {
	DNS.processNextRequest();
}