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
CAppControl     oAppControl;


#ifdef DEBUGINFOS
/**
 * @brief runDebugTests
 * Function to run some debug tests on startup
 */
void runDebugTests() {
  JsonDocument oCfg;
  JsonObject oCfgNode = oCfg.to<JsonObject>();
  Appl.writeConfigTo(oCfgNode,false);
  DEBUG_INFO("Current configuration:");
  DEBUG_JSON_OBJ(oCfgNode);
}
#endif

/**
 * @brief registerModules
 * Register all modules to the application - config handlers, status handlers, message receivers
 */
void registerModules() {

  Appl.registerModule("wifi",&oWiFiController);
  Appl.registerModule("mqtt",&oMqttController);
  Appl.registerModule("web", &oWebServer);
  // Register the configuration and status handlers
  
  // Register the message receivers...
  // - React on Appl.dispatch() calls (MSG_APPL_LOOP message)
  // - offer module commands on the message bus for other modules
  Appl.MsgBus.registerEventReceiver(&oAppControl);
  Appl.MsgBus.registerEventReceiver(&oWebSocket);     

}

void setup() {
  Serial.begin(115200);
  DEBUG_FUNC_START();
  DEBUG_INFOS("\nInitializing application: \"%s\" Version: %s\n",APP_NAME,APP_VERSION);

  registerModules();

    // Initialize the application - this will send a MSG_APPL_INITIALIZED message
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
  DEBUG_FUNC_END();
}

void loop() {
  Appl.dispatch(); // will send a MSG_APPL_LOOP message to all registered receivers
  // put your main code here, to run repeatedly:
}
