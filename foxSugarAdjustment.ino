#include <VarSpeedServo.h>

VarSpeedServo bottomServo;
VarSpeedServo frontbackServo;
VarSpeedServo updownServo;

#define vib 8
#define sol1 9
#define sol2 10
#define sol3 11
#define sol4 12

char inputKey;  //キーボード入力用の変数
char buf[20];   //Serial通信用バッファの変数
int candyColor;   //砂糖の色(0~3の4通り、0:white,1:red,2:green,3.blue)
int solKnockCount;   //solKnock()でSolenoidが叩く回数
int j = 0;    //Serial通信用のバッファ数

//キャリブレーション用数値
int cali0 = 3.0;
int cali1 = 3.0;
int cali2 = 1.3;
int cali3 = 1.7;

void setup() {
  //RobotArmのServoのピン番号指定
  bottomServo.attach(3);
  frontbackServo.attach(5);
  updownServo.attach(6);

  //frontbackServoは角度固定のためsetup内で角度指定
  frontbackServo.write(80);
  frontbackServo.wait();

  //SolenoidとVibrationのピン設定
  pinMode(vib, OUTPUT);
  pinMode(sol1, OUTPUT);
  pinMode(sol2, OUTPUT);
  pinMode(sol3, OUTPUT);
  pinMode(sol4, OUTPUT);
  pinMode(0,OUTPUT);
  pinMode(1,OUTPUT);

  digitalWrite(0,LOW);
  digitalWrite(1,HIGH);

  Serial.begin(9600);
  moveWaitPos();
}

void loop() {
  if (Serial.available()) {
    buf[j] = Serial.read();
    if (buf[j] == 'e') {
      buf[j] = '\0';

      candyColor = atoi(strtok(buf, ","));
      solKnockCount = atoi(strtok(NULL, ","));

      Serial.println(candyColor);
      Serial.println(solKnockCount);

      switch (candyColor) {
        case 0 :
          candyOFF();
          moveFillPos();
          delay(1000);
          solKnockCount = solKnockCount*cali0;
          solKnock(9);
          delay(3000);
          moveWaitPos();
          Serial.write(0);
          delay(4000);
          candyON();
          break;
        case 1 :
          candyOFF();
          moveFillPos();
          delay(1000);
          solKnockCount = solKnockCount*cali1;
          solKnock(10);
          delay(3000);
          moveWaitPos();
          Serial.write(0);
          delay(4000);
          candyON();
          break;
        case 2 :
          candyOFF();
          moveFillPos();
          delay(1000);
          solKnockCount = solKnockCount*cali2;
          solKnock(11);
          delay(3000);
          moveWaitPos();
          Serial.write(0);
          delay(4000);
          candyON();
          break;
        case 3 :
          candyOFF();
          moveFillPos();
          delay(1000);
          solKnock(12);
          solKnockCount = solKnockCount*cali3;
          delay(3000);
          moveWaitPos();
          Serial.write(10);
          delay(4000);
          candyON();
          break;
        default:
          break;
      }
      j = 0;
    }
    else {
      j++;
    }
  }
}

//WaitPositionに戻る関数
void moveWaitPos() {
  updownServo.write(40, 20, true);
  bottomServo.write(0, 20, true);
  bottomServo.wait();
  updownServo.wait();
}

//砂糖FillPositionに戻る関数
void moveFillPos() {
  bottomServo.write(70, 20, true);
  updownServo.write(62, 20, true);
  bottomServo.wait();
  updownServo.wait();
}

//SolenoidとVibrationで砂糖を送り出す関数
void solKnock(int x) {
  digitalWrite(vib, HIGH);  //Vibration回転開始
  delay(500);   //Vibrationに電流を流してから振動し始めるまでの待ち時間
  for (int i = 0; i < solKnockCount; i++) {
    digitalWrite(x, HIGH);
    delay(200);
    digitalWrite(x, LOW);
    delay(10);
  }
  digitalWrite(vib, LOW);   //Vibration回転終了
}

void candyOFF(){
  digitalWrite(1,LOW);
}

void candyON(){
  digitalWrite(1,HIGH);
}

