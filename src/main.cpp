//Код платы принимателя (принимает через цифровой пин информацию, записывает в структуру и корректрует подключенныйе серво);
#define BOARD_ADDR 2 //адрес платы принимателя начинается с 2
#define NET_PIN 42 //цифровой пин который подключен подключен к сети GBUS  


#include <Arduino.h>
#include <GBUSmini.h>

struct BoardInfo{
  uint8_t board_addr;
  uint8_t servo_num;
  uint8_t rotate_val;
} bufferToReadInfo; 

uint8_t byteBufferToReadInfo[sizeof(bufferToReadInfo)];

void setup() {
  pinMode(NET_PIN, INPUT_PULLUP);
  digitalWrite(NET_PIN, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  //delay(750);
  //sett
}

void loop() {
  // if(digitalRead(NET_PIN) == 0){
  //   digitalWrite(LED_BUILTIN, HIGH);
  // } else {
  //   digitalWrite(LED_BUILTIN, LOW);
  // }
  //Serial.println(digitalRead(NET_PIN));
  if(GBUS_read(NET_PIN, BOARD_ADDR, byteBufferToReadInfo, sizeof(byteBufferToReadInfo))){
    digitalWrite(LED_BUILTIN, HIGH);
    unpackDataBytes(byteBufferToReadInfo, bufferToReadInfo);
    Serial.print(bufferToReadInfo.board_addr);
    Serial.print(", ");
    Serial.print(bufferToReadInfo.servo_num);
    Serial.print(", ");
    Serial.print(bufferToReadInfo.rotate_val);
    Serial.println();
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }
}