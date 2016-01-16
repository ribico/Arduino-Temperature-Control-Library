// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 5
#define TEMPERATURE_PRECISION 11

#define DALLAS_1_ADDRESS  { 0x28, 0x12, 0x0F, 0x29, 0x07, 0x00, 0x00, 0xDD }
#define DALLAS_2_ADDRESS  { 0x28, 0xA1, 0x29, 0x2A, 0x07, 0x00, 0x00, 0x6F }
#define DALLAS_3_ADDRESS  { 0x28, 0x4F, 0x7F, 0x29, 0x07, 0x00, 0x00, 0xD4 }
#define DALLAS_4_ADDRESS  { 0x28, 0x9F, 0x1E, 0x29, 0x07, 0x00, 0x00, 0xCC }
#define DALLAS_5_ADDRESS  { 0x28, 0x57, 0xDC, 0x29, 0x07, 0x00, 0x00, 0x53 }
#define DALLAS_6_ADDRESS  { 0x28, 0xBB, 0xFC, 0x28, 0x07, 0x00, 0x00, 0x70 }
#define DALLAS_7_ADDRESS  { 0x28, 0x42, 0xF0, 0x28, 0x07, 0x00, 0x00, 0xB5 }
#define DALLAS_8_ADDRESS  { 0x28, 0xE1, 0x06, 0x29, 0x07, 0x00, 0x00, 0x05 }
#define DALLAS_9_ADDRESS  { 0x28, 0x2D, 0x35, 0x2A, 0x07, 0x00, 0x00, 0xA5 }
#define DALLAS_10_ADDRESS  { 0x28, 0xBA, 0xA2, 0x2A, 0x07, 0x00, 0x00, 0x77 }
#define DALLAS_11_ADDRESS  { 0x28, 0xEB, 0x32, 0x03, 0x07, 0x00, 0x00, 0x3A }
#define DALLAS_12_ADDRESS  { 0x28, 0x81, 0x22, 0x17, 0x07, 0x00, 0x00, 0x7C }
#define DALLAS_13_ADDRESS  { 0x28, 0xA3, 0xFE, 0x50, 0x07, 0x00, 0x00, 0xBD }
#define DALLAS_14_ADDRESS  { 0x28, 0xC8, 0xC8, 0x51, 0x07, 0x00, 0x00, 0x11 }


// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);


void setup(void)
{
  // start serial port
  Serial.begin(9600);
  while (!Serial)
  {
    ;
  }
  Serial.println("Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin();

  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  for(uint8_t i=0; i<sensors.getDeviceCount(); i++)
  {
    DeviceAddress tempAddress;
    if( !sensors.getAddress(tempAddress, i) )
    {
      Serial.print("Unable to find address for Device ");
      Serial.println(i);
    }
    else
    {
      Serial.print("Device ");
      Serial.print(i);
      Serial.print(" Address: ");
      printAddress(tempAddress);
      Serial.println();

      sensors.setResolution(tempAddress, TEMPERATURE_PRECISION);
      Serial.print("Device Resolution: ");
      Serial.print(sensors.getResolution(tempAddress), DEC);
      Serial.println();
    }
  }

  Serial.println();
  Serial.println();
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
}

// function to print a device's resolution
void printResolution(DeviceAddress deviceAddress)
{
  Serial.print("Resolution: ");
  Serial.print(sensors.getResolution(deviceAddress));
  Serial.println();
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
  Serial.print("Device Address: ");
  printAddress(deviceAddress);
  Serial.print(" ");
  printTemperature(deviceAddress);
  Serial.println();
}

/*
 * Main function, calls the temperatures in a loop.
 */
void loop(void)
{
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");


  for(uint8_t i=0; i<sensors.getDeviceCount(); i++)
  {
    DeviceAddress tempAddress;
    if( !sensors.getAddress( tempAddress, i) )
    {
      Serial.print("Unable to find address for Device ");
      Serial.println(i);
    }
    else
    {
      printData(tempAddress);
    }
  }

}
