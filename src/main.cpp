//Код платы контроллера (принимает и парсит команду через COM порт, отправляет по цифровому пину команды на другие платы);
#define BOARD_ADDR 1 //адрес платы контроллера всегда равен одному
#define NET_PIN 4 //цифровой пин который подключен подключен к сети GBUS  


#include <Arduino.h>
#include <GyverBus.h>
#include <GBUS.h>
#include <GBUSmini.h>

struct BoardInfo{
  uint8_t board_addr;
  uint8_t servo_num;
  uint8_t rotate_val;
} bufferToSendInfo; 

uint8_t byteBufferToSendInfo[sizeof(bufferToSendInfo)];

void getCommand(String portStr);
void sendCommand();
void testSend();

void setup() {
  pinMode(NET_PIN, INPUT_PULLUP);
  digitalWrite(NET_PIN, HIGH);
  pinMode(LED_BUILTIN, OUTPUT); 
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()){
    getCommand(Serial.readString());
  }
  //delay(1500);
  //testSend();
}

void getCommand(String portStr){
  if (portStr.length() != 20){
      Serial.println("Invalid com");
      return;
  }
  if (portStr.substring(0, 9) != "GET_COM::" || portStr[12] != ':' || portStr[16] != ':'){
      Serial.println("Invalid com");
      return;
  }
  String tmpArray[3] = {portStr.substring(9, 12), portStr.substring(13, 16), portStr.substring(17, 20)};
  if(tmpArray[0].toInt() == 0 || tmpArray[1].toInt() == 0 || tmpArray[2].toInt() == 0){
    Serial.println("Invalid com");
      return;
  }
  bufferToSendInfo.board_addr = (uint8_t)tmpArray[0].toInt();
  bufferToSendInfo.servo_num = (uint8_t)tmpArray[1].toInt();
  bufferToSendInfo.rotate_val = (uint8_t)tmpArray[2].toInt();
  Serial.println("Succes com");
  sendCommand();
  //Serial.println(bufferToSendInfo.board_addr);
  //Serial.println(bufferToSendInfo.servo_num);
  //Serial.println(bufferToSendInfo.rotate_val);
}

void sendCommand(){
  packDataBytes(byteBufferToSendInfo, bufferToSendInfo);
  //Serial.println(bufferToSendInfo.board_addr + 1);
  GBUS_send(NET_PIN, bufferToSendInfo.board_addr + 1, BOARD_ADDR, byteBufferToSendInfo, sizeof(byteBufferToSendInfo));
}

void testSend(){
  bufferToSendInfo.board_addr = 1;
  bufferToSendInfo.servo_num = 2;
  bufferToSendInfo.rotate_val = 130;
  packDataBytes(byteBufferToSendInfo, bufferToSendInfo);
  GBUS_send(NET_PIN, bufferToSendInfo.board_addr + 1, BOARD_ADDR, byteBufferToSendInfo, sizeof(byteBufferToSendInfo));
}