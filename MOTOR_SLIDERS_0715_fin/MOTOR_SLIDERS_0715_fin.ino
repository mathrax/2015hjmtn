//暫定採用版

#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 5); // RX, TX

unsigned char CommandRESET[] = {
  0x00, 0x06, 0x00, 0x7D, 0b00000000, 0b00000000
};
unsigned char CommandBASIC[] = {
  0x00, 0x06, 0x00, 0x7D, 0b00000000, 0b00000000
};

unsigned char CommandHOME[] = {
  0x00, 0x06, 0x00, 0x7D, 0b00000000, 0b00010000
};
unsigned char CommandSTART[] = {
  0x00, 0x06, 0x00, 0x7D, 0b00000000, 0b00001000
};
unsigned char myCommand[] = {
  0, 0, 0, 0, 0, 0
};
unsigned char RcvCommand[32];

int CRC;

#define RS485pin 3
#define BREAK 4

#define SW1 12
#define SW2 9

int state1, onoff1;
int state2, onoff2;

#define FastUp        0
#define FastMiddle    1
#define FastDown      2
#define SlowUp        3
#define SlowMiddle    4
#define SlowDown      5

int timeline;

int timeList[] = {
  30, 30, 30, 30, 30, 30, 40, 10, 10, 40, 10, 30, 
};

int cmdList[] = {
  0, 2, 0, 2, 0, 2, 3, 1, 0, 5, 1, 2, 
};

int timeCount[13];
int scene[13];

int lastx[13];
int lasty[13];
int now_y[13];

int DEBUGMODE = 0;

int delaylist[] = {
  0, 0, 0, 0, 0, 0, 0,          //1
  0, 0, 0, 0, 0, 0, 0,          //2
  0, 0, 0, 0, 0, 0, 0,          //3
  0, 0, 0, 0, 0, 0, 0,          //4
  0, 0, 0, 0, 0, 0, 0,          //5
  80, 40, 0, 60, 100, 20, 120,  //6
  0, 0, 0, 0, 0, 0, 0,          //7
  0, 0, 0, 0, 0, 0, 0,          //8
  0, 0, 0, 0, 0, 0, 0,          //9
  0, 0, 0, 0, 0, 0, 0,          //10
  0, 0, 0, 0, 0, 0, 0,          //11
  0, 0, 0, 0, 0, 0, 0,          //12
  0, 0, 0, 0, 0, 0, 0,          //end
};

int ledlist[] = {2, 4, 5, 7, 10, 12, 13};

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  mySerial.begin(57600);

  pinMode(RS485pin, OUTPUT);
  pinMode(BREAK, INPUT_PULLUP);

  pinMode(13, OUTPUT);

  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);

  send_modbus(CommandRESET, sizeof(CommandRESET));

  for (int i = 0; i < 7; i++) {
    timeCount[i] = 0;
    scene[i] = 0;
    lasty[i] = -1;
  }
  if (DEBUGMODE == 1) {
    for (int i = 0; i < 7; i++) {
      pinMode(ledlist[i], OUTPUT);
      digitalWrite(ledlist[i], LOW);
    }
    digitalWrite(9, 0);
  }
  delay(4000);
}

void loop() // run over and over
{
  timeline++;
  if (timeline > 460) {    //All timeList + Max delayList
    timeline = 0;
    for (int i = 0; i < 7; i++) {
      timeCount[i] = 0;
      scene[i] = 0;
      lasty[i] = -1;
    }
  }

  for (int i = 0; i < 7; i++) {
    if (timeline > delaylist[i + (scene[i] * 7)]) {
      timeCount[i]++;
    }
    if (timeCount[i] > timeList[scene[i]] + delaylist[i + (scene[i] * 7)]) {
      timeCount[i] = 0;
      scene[i]++;
      if (scene[i] > 12)scene[i] = 12;
    }

    now_y[i] = cmdList[scene[i]];

    if (now_y[i] != lasty[i]) {
     if(cmdList[scene[i]]>=0) send_motor(i, cmdList[scene[i]]);
    }

    lasty[i] = now_y[i];
  }
  delay(100);
}


void send_motor(int id, int pos) {
  send_id_position(id, pos);
  send_modbus(CommandRESET, sizeof(CommandRESET));
}







