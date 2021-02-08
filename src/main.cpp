#include <TheThingsNetwork.h>
#include <SoftwareSerial.h>
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

SoftwareSerial loraSerial = SoftwareSerial(7,6);
#define debugSerial Serial

#define freqPlan TTN_FP_EU868

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);



//the pin you connect the ds18b20 to
#define DS18B20 10
OneWire ourWire(DS18B20);
DallasTemperature sensors(&ourWire);


//set AppEUI and key here through OTAA
const char *appEui = "70B3D57ED003D28B";
const char *appKey = "CFE578E05B57B82C5DCC9FAB6E4EBBC3";


#define LED 3
//#define LED2 4
const int time = 100;

void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  //wait a maximum of 10s for serial monitor
  while (!debugSerial && millis() < 10000);
  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);

  //initialise status LED
  pinMode(LED,OUTPUT);
  //start reading
 sensors.begin();
}

void loop()
{

    debugSerial.println("-- LOOP");

    //wait for 2 seconds before measurements
    delay(2000);

      //read temperature and output via serial
    sensors.requestTemperatures();
    


    //parse the bytes for transmission

    uint16_t temp = sensors.getTempCByIndex(0)*100;

    //split the data into bytes for transmission
    byte payload[2];
    payload[0] = highByte(temp);
    payload[1] = lowByte(temp);


   
   
   debugSerial.print(sensors.getTempCByIndex(0));
   debugSerial.println("°C");

  if(sensors.getTempCByIndex(0) > 23){
    digitalWrite(LED, HIGH);
    delay(time);
  }else{
    digitalWrite(LED, LOW);
  }
    delay(time);

   ttn.sendBytes(payload, sizeof(payload));
   delay(10000);
}
