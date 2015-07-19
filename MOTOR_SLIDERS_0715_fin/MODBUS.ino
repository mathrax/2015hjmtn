void send_position(unsigned int pos) {
  for (int i = 0; i < 6; i++) {
    myCommand[i] = CommandBASIC[i];
  }
  if (pos >= 7) pos = 7;
  myCommand[5] = 0b00001000 + pos;

  send_modbus(myCommand, sizeof(myCommand));
  
}

void send_id_position(unsigned int id, unsigned int pos) {
  for (int i = 0; i < 6; i++) {
    myCommand[i] = CommandBASIC[i];
  }
  if(id>=0 && id<=7){
    myCommand[0] = id;
  }
  if (pos >= 7) pos = 7;
  myCommand[5] = 0b00001000 + pos;

  send_modbus(myCommand, sizeof(myCommand));
  
}

void send_modbus(unsigned char *Data_Array, int len){

  
  //SEND MODE
  pinMode(BREAK, INPUT_PULLUP);
  digitalWrite(RS485pin,HIGH);
  delay(3);

  for(int i=0; i<len; i++){
    mySerial.write(Data_Array[i]& 0xFF);
  }
  //CRC
  CRC = KIRSP_cal_CRC16(&Data_Array[0],len);
  mySerial.write(CRC & 0xFF);  //CRC-16
  mySerial.write((CRC >> 8)&0xFF);  //CRC-16

  delay(1);
  
  //RECEIVE
  digitalWrite(RS485pin,LOW);
  delay(5);

  int rcv_cnt=0;

  while(mySerial.available()>0){
    RcvCommand[rcv_cnt]=mySerial.read()& 0xFF;
    Serial.print(RcvCommand[rcv_cnt]);
    Serial.print(" ");
    rcv_cnt++;
  }
  Serial.println("");

  //C3.5
  delay(1);
  
  
  mySerial.flush();
  Serial.flush();
}









