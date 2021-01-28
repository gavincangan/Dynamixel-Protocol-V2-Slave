#include "NetworkController.h"

NetworkController network_controller = NetworkController();

void setup() {

  // Dynamixel baud rate
  Serial1.begin(BAUDRATE);

  // Serial for debugging
  Serial.begin(9600);
  Serial.println("                              ******************  START  ******************");
  Serial.println("--------------------------------------------------------------------------------------------------------");
  Serial.println(" ");

pinMode (2, OUTPUT);
digitalWrite(2, HIGH);
//digitalWrite(13, HIGH);

// Model Number (2 bytes) (Value Default 1020 (DEC) so -> Model_Byte_Low : 0xFC  and Model_Byte_High : 0x03
  network_controller.tabRegister[0]=0xFC;
  network_controller.tabRegister[1]=0x03;
// Version of Firmware
  network_controller.tabRegister[2]=FIRMWARE;
// ID
  network_controller.tabRegister[3]=network_controller.ID;
  // network_controller.ID=0x01;
}

void loop() {
  // put your main code here, to run repeatedly:
while (Serial1.available())
  {
   int c = Serial1.read(); 
if(c >= 0)network_controller.machineState(c);   
//if fin du message
//
//network_controller.returnStatus();
  }

}
