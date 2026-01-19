/**
 * @file       main.cpp
 * @author     LSC Labs - P. Liebl
 * @version    1.0.2
 * @date       2025-12-05
 * @brief      Main application file
 * 
 * -> this file is a template from LSC Labs for IoT applications based on ESP8266/ESP32 (ESP-ProjectTemplate)
 * -> Modify for your own application as needed.
 * 
 * Template contains:
 * - WiFi management (Station + Access Point mode)
 * - MQTT client with Home Assistant auto discovery support
 * - Web Server with WebSocket support
 * 
 */
#include <Arduino.h>
#include <AppConfig.h>
#include <AppControl.h>
#include <FileSystem.h>
#include <Appl.h>
#include <WiFiController.h>
#include <MQTTController.h>
#include <WebSocket.h>
#include <WebServer.h>
// #include <WebRoutes.h>   - if you have custom web routes, include your WebRoutes.h here
#include <DevelopmentHelper.h>


CFS oFS;
CWiFiController oWiFiController;
CMQTTController oMqttController;
CWebSocket      oWebSocket("/ws");
CWebServer      oWebServer(80);

// Insert an application control instance to handle the application coordination and logic
// CAppControl oAppControl;


#ifdef DEBUGINFOS
void runDebugTests() {
  JsonDocument oCfg;
  JsonObject oCfgNode = oCfg.to<JsonObject>();
  Appl.writeConfigTo(oCfgNode,false);
  DEBUG_INFO("Current configuration:");
  DEBUG_JSON_OBJ(oCfgNode);
}
#endif

void setup() {
  Serial.begin(115200);
  DEBUG_INFOS("\nInitializing application: \"%s\" Version: %s\n",APP_NAME,APP_VERSION);
  
  Appl.addConfigHandler("wifi",&oWiFiController);
  Appl.addConfigHandler("mqtt",&oMqttController);

  Appl.addStatusHandler("wifi",&oWiFiController);
  Appl.addStatusHandler("mqtt",&oMqttController);

  // Connect the message bus to the application control implementation
  // Appl.MsgBus.addEventHandler(&oAppControl);

  Appl.init(APP_NAME,APP_VERSION);
  Appl.sayHello();
  
  bool bUseConfigData = Appl.readConfigFrom(CONFIG_FILE);
  Appl.printDiag();
  Appl.Log.logInfo(F("Initializing services..."));
 
  // Initialize and start the WiFi
  oWiFiController.startWiFi(bUseConfigData);
  oMqttController.setup();
  
  // Prepare and start the Web Server
  // enable registerWebRoutes if you have custom web routes (WebRoutes.cpp/h)
  oWebServer.addHandler(&oWebSocket);
  // registerWebRoutes(oWebServer);  
  oWebServer.begin();
  

  Appl.Log.logInfo(F("Hello world... - let's start the show!"));
  delay(200);

  #ifdef DEBUGINFOS
    runDebugTests();
  #endif
}

void loop() {
  oWebSocket.dispatchMessageQueue();
  // put your main code here, to run repeatedly:
}
