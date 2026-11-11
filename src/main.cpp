/**
 * @file       main.cpp
 * @author     LSC Labs - P. Liebl
 * @version    1.1.0
 * @date       2026-11-11
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
 * Web GUI (embedded pages and routes):
 * The device can be built without the web GUI. To enable it:
 * 1. "npm run sync-RuntimeFiles" - copies WebRoutes.cpp / WebRoutes.h and index.html
 *    from the library (always the current version of PLibESPV1 - do not edit them,
 *    add your own routes in a separate file).
 * 2. "npm run create-WebPages" - builds the pages into include/web/*.gz.h
 * 3. Uncomment "#include <WebRoutes.h>" and "registerWebRoutes(oWebServer);" below.
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
// #include <WebRoutes.h>     // Web GUI - see header (sync-RuntimeFiles + create-WebPages)
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
  JsonNode oCfgNode;
  DEBUG_INFO("Current configuration:");
  Appl.writeConfigTo(oCfgNode,false);
  DEBUG_JSON_OBJ(oCfgNode);
}
#endif

/**
 * @brief registerModules
 * Register all modules to the application - config handlers, status handlers, message receivers
 */
void registerModules() {

  // Modules with configuration and status (wifi, mqtt, web)
  Appl.registerModule("wifi",&oWiFiController);
  Appl.registerModule("mqtt",&oMqttController);
  Appl.registerModule("web", &oWebServer);

  // Register your own configuration and status handlers
  // Appl.addConfigHandler("app", &oMyModule);
  // Appl.addStatusHandler("app", &oMyModule);

  // Register the message receivers...
  // - React on Appl.dispatch() calls (MSG_APPL_LOOP message)
  // - offer module commands on the message bus for other modules
  Appl.MsgBus.registerEventReceiver(&oAppControl);
  // WebSocket: dispatches its message queue on MSG_APPL_LOOP and sends
  // bus messages (i.e. WiFi scan results) to the clients
  Appl.MsgBus.registerEventReceiver(&oWebSocket,"WebSocket");

  // Home Assistant auto discovery - register your components
  // oMqttController.registerHomeAssistantComponent("app", &oMyModule);
  oMqttController.registerHomeAssistantComponent("wifi", &oWiFiController);
}

void setup() {
  // Serial.begin(115200); // Moved to CAppl constructor
  DEBUG_FUNC_START();
  DEBUG_INFOS("\nInitializing application: \"%s\" Version: %s\n",APP_NAME,APP_VERSION);

  registerModules();

  // Initialize the application - this will send a MSG_APPL_INITIALIZED message
  Appl.init(APP_NAME,APP_VERSION);
  Appl.sayHello();

  bool bUseConfigData = Appl.readConfigFrom(CONFIG_FILE);
  Appl.printDiag();
  ApplLogInfo(F("Initializing services..."));

  // Register the web routes BEFORE the WiFi is started: the server is started on
  // MSG_WIFI_CONNECTED - requests (i.e. network scanners) must not hit a server,
  // whose routes are still being registered.
  ApplLogInfo(F("..initializing web"));
  // registerWebRoutes(oWebServer);   // Web GUI - see header (sync-RuntimeFiles + create-WebPages)
  oWebServer.addHandler(&oWebSocket);

  // Start the WiFi and the web server
  oWiFiController.startWiFi(bUseConfigData);
  oWebServer.begin();         // ignored, if already started by MSG_WIFI_CONNECTED

  ApplLogInfo(F("..initializing mqtt"));
  oMqttController.setup();

  ApplLogInfo(F("Hello world... - let's start the show!"));
  delay(200);

  #ifdef DEBUGINFOS
    runDebugTests();
  #endif
  // Sends MSG_APPL_STARTED - i.e. MQTT publishes the Home Assistant discovery
  Appl.start();
  DEBUG_FUNC_END();
}

void loop() {
  // Sends MSG_APPL_LOOP to all registered receivers - the library modules dispatch
  // themselves there: WebSocket (message queue), MQTT, WiFi (scan, reconnect),
  // WebServer (reboot after OTA)
  Appl.dispatch();
  // put your main code here, to run repeatedly:
}
