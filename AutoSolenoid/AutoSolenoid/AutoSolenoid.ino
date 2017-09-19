#include <Wire.h>
#include <MyBase64.h>
#include <WiFiEsp.h>
#include <WiFiEspUdp.h>
#include <WiFiEspServer.h>
#include <EEPROMex.h>
#include "Common.h"
#include "Debug.h"
#include "CString.h"
#include "SerialManager.h"



uint8_t nRelayPin = 10, nStation1Pin = 2, nStation2Pin = 3, nStation3Pin = 4, nStation4Pin = 5, nStation5Pin = 6, nStation6Pin = 7, nStation7Pin = 8, nStation8Pin = 9, nStationNum = 0;
#define serialESP8266 Serial1
#define serialHC05 Serial2
WiFiEspUDP UDPServer;




uint8_t getStationNum()
{
  uint8_t nStationNum = 0;

  if (digitalRead(nStation1Pin) == HIGH)
    nStationNum = 1;
  else if (digitalRead(nStation2Pin) == HIGH)
    nStationNum = 2;
  else if (digitalRead(nStation3Pin) == HIGH)
    nStationNum = 3;
  else if (digitalRead(nStation4Pin) == HIGH)
    nStationNum = 4;
  else if (digitalRead(nStation5Pin) == HIGH)
    nStationNum = 5;
  else if (digitalRead(nStation6Pin) == HIGH)
    nStationNum = 6;
  else if (digitalRead(nStation7Pin) == HIGH)
    nStationNum = 7;
  else if (digitalRead(nStation8Pin) == HIGH)
    nStationNum = 8;
    
  return nStationNum;
}

bool intiWifiShield()
{
  bool bResult = false;
  uint8_t nRetries = 0;
  uint8_t nStatus = WL_IDLE_STATUS;

  // Initialize ESP module
  WiFi.init(&serialESP8266);

  // Check for the presence of the shield
  if ((nStatus = WiFi.status()) == WL_NO_SHIELD)
  {
    debug.log(F("WiFi shield not present!"));
    WiFi.reset();
    resetArd();
  }
  else
  {
    CSerialManager serialManager(&UDPServer, &UDPServer, &serialHC05, nRelayPin, nStationNum);

    // Attempt to connect to WiFi network
    while ((nStatus != WL_CONNECTED) && (nRetries < 10))
    {
      debug.log(F("Attempting to connect to WPA SSID: "), false);
      debug.log(serialManager.getNetworkName(), false);
      debug.log(F(", KEY: "), false);
      debug.log(serialManager.getNetworkKey(), false);
      debug.log(F("..."), true);

      // Connect to WPA/WPA2 network
      nStatus = WiFi.begin((char*)serialManager.getNetworkName(), (char*)serialManager.getNetworkKey());
      debug.dump(F("nStatus"), nStatus);
      nRetries++;
    }
    if (nStatus == WL_CONNECTED)
    {
      IPAddress IPAddr = WiFi.localIP();
      debug.log(F("You're connected to the network with IP address: "), false);
      debug.log(IPAddr, true);
      bResult = true;
    }
    else
    {
      debug.log(F("Could not log into local wifi network - SSID & KEY are incorrect!"));
      debug.log(F("Please upload your SSID & KEY via bluetooth."));
    }
  }
  return bResult;
}

//*************************************************************************************************
// Do all the setup tasks.
//*************************************************************************************************
void setup() 
{
  Serial.begin(115200);
  while (!Serial);

  serialESP8266.begin(115200);
  serialHC05.begin(38400);


  digitalWrite(nRelayPin, LOW);
  nStationNum = getStationNum();

  EEPROM.setMemPool(0, EEPROMSizeUno);
  EEPROM.setMaxAllowedWrites(EEPROMSizeUno);
  
  // Wait for serial port to connect - needed for native USB port only
  delay(10);
  debug.init();
  
  if (intiWifiShield())
  {
    if (UDPServer.begin(10004))
    {
      debug.log(F("Setup() complete..."));
      CSerialManager SerialManager(&UDPServer, &UDPServer, &serialHC05, nRelayPin, nStationNum);
      SerialManager.notifyController();
    }
    else
      debug.logRuntimeError(F("WifiManager.cpp"), __LINE__);      
  }
  else
  {
    debug.logRuntimeError(F("IrrigationController.cpp"), __LINE__);
    resetArd();
  }
}
 
void loop()
{ 
  if (serialHC05.available() > 0)
  {
    CSerialManager serialManager(&serialHC05, &UDPServer, &serialHC05, nRelayPin, nStationNum);
    serialManager.processData();
  }
  if (UDPServer.parsePacket() > 0)
  {
     CSerialManager serialManager(&UDPServer, &UDPServer, &serialHC05, nRelayPin, nStationNum);
     serialManager.processData();
  }
  // Listen for incoming clients
}

