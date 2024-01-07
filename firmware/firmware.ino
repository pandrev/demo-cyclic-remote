#include <WiFi.h>
#include <ArduinoJson.h>       // https://arduinojson.org/
#include <WebSocketsClient.h>  // download and install from https://github.com/Links2004/arduinoWebSockets
#include <SocketIOclient.h>

const char* SSID = "Vodafone-30B1D7_EXT";
const char* PASSWORD = "763AHFHG33MHDX6T";
//#define SERVER "esp-socket-switch.heSrokuapp.com"  // Server URL (without https://www)
const char* SERVER = "wss://demo-cyclic-remote.onrender.com";

#define LED 2

SocketIOclient socketIO;


void messageHandler(uint8_t* payload) {
  StaticJsonDocument<64> doc;

  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.println(error.f_str());
    return;
  }

  String messageKey = doc[0];
  bool value = doc[1];

  if (messageKey == "buttonState") {
    digitalWrite(LED, value);
  }
}

void socketIOEvent(socketIOmessageType_t type, uint8_t* payload, size_t length) {
  if (type ==  sIOtype_DISCONNECT) {

      Serial.println("Disconnected!");
  }else if(type ==  sIOtype_CONNECT) {

      Serial.printf("Connected to url: %s%s\n", SERVER, payload);

      // join default namespace (no auto join in Socket.IO V3)
      socketIO.send(sIOtype_CONNECT, "/");

  }else if(type == sIOtype_EVENT){

      messageHandler(payload);
      Serial.println("ligou ou nao ya socio");
  }else {
    Serial.println("OPPPPPPPPPPPPPPPPPS");
  }
}

void setupWiFi() {
  Serial.println("\nConnecting...");

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nConnected : ");
  Serial.println(WiFi.localIP());
}


void setup() {
  pinMode(LED, OUTPUT);

  Serial.begin(9600);

  setupWiFi();

  // server address, port and URL
 socketIO.begin(SERVER, 443, "/socket.io/?transport=websocket");

  socketIO.onEvent(socketIOEvent);
}

void loop() {
  socketIO.loop();
}