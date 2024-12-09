#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <AFMotor.h>
#include "Servo.h"

Servo servo;
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

int lateral;
int axial;
int yaw1;
int rightFront;
int leftFront;
int rightBack;
int leftBack;
int Gat2;

LiquidCrystal_I2C lcd(0x27,16,2);
 
String SerialRd;
int verticalPos = 0;
int atualPos = 0;
int etapaMenu = 1;
int horizontalPos = 0;

//-------------------------------
//INICIO DAS FUNCOES DO CONTROLADO
//-------------------------------


void Reciver(){
String received = Serial.readStringUntil('\n');
    
rightFront = axial - lateral + yaw1;
leftFront = axial + lateral - yaw1;
rightBack = axial + lateral + yaw1;
leftBack = axial - lateral - yaw1; 

    // Verifica e armazena a variável correspondente
    if (received.startsWith("  Axial: ")) {
      axial = received.substring(8).toInt(); // Extrai o valor e converte para inteiro
     // Serial.print("  Axial: ");
     // Serial.print(axial);
    }
    if (received.startsWith("  Later: ")) {
      lateral = received.substring(8).toInt(); // Extrai o valor e converte para inteiro
     // Serial.print("  Lateral: ");
     // Serial.print(lateral);
    }
    if (received.startsWith("  Yaw1: ")) {
      yaw1 = received.substring(8).toInt(); // Extrai o valor e converte para inteiro
     // Serial.print("  Yaw1: ");
     // Serial.print(yaw1);
    }
    if (received.startsWith("  Gat2: ")) {
      Gat2 = received.substring(8).toInt(); // Extrai o valor e converte para inteiro
      servo.write(map(Gat2, 0, 1023, 0, 180));
    }
Serial.print(leftFront / 2);
Serial.print('\t');
Serial.print(rightFront / 2);
Serial.print('\t');
Serial.print(leftBack / 2);
Serial.print('\t');
Serial.print(rightBack / 2);
Serial.println('\t');

 power();
 direction();

}

void power(){
if(rightFront < 0){
    motor2.setSpeed(rightFront * -1);  
  } else {
    motor2.setSpeed(rightFront);
  }
  if(leftFront < 0){
    motor3.setSpeed(leftFront * -1);  
  } else{
    motor3.setSpeed(leftFront);
  }
  if(leftBack < 0){
    motor4.setSpeed(leftBack * -1);
  }else{
    motor4.setSpeed(leftBack);  
  }
  if(rightBack < 0){
    motor1.setSpeed(rightBack * -1);  
  } else{
    motor1.setSpeed(rightBack);
  }  
}

void direction(){
  if(leftFront > 0){
motor3.run(BACKWARD);
} else{
  motor3.run(FORWARD);
}

if(leftBack > 0){
motor4.run(BACKWARD);
} else{
  motor4.run(FORWARD);
}

if(rightFront > 0){
motor2.run(BACKWARD);
} else{
  motor2.run(FORWARD);
}

if(rightBack > 0){
motor1.run(BACKWARD);
} else{
  motor1.run(FORWARD);
}

//-------------------------------
//FINAL DAS FUNCOES DO CONTROLADO
//-------------------------------

}

void setup()
{
 Serial.begin(115200);
 servo.attach(A0);
 lcd.init();
 lcd.backlight();
 menuFase(etapaMenu);
 selects("4");
}
 
void loop()
{
if(Serial.available() > 0){
  SerialRd = Serial.readString();                  
  SerialRd.trim();  
  lcd.clear();
  selects(SerialRd);
  menuFase(etapaMenu);
 }

}

void selects(String pos){

if(pos == "w" && verticalPos == 1){
   horizontalPos = 0;
   verticalPos = verticalPos - 1;
   atualPos = verticalPos + horizontalPos; 
   lcd.setCursor(0, verticalPos);
   lcd.print(">");
   Serial.print(SerialRd);
   Serial.print(verticalPos);
   Serial.print(horizontalPos);   
   Serial.println(atualPos);   
 return;
} 
else if(pos == "s" && verticalPos == 0){
   horizontalPos = 0;
   verticalPos = verticalPos + 1;
   atualPos = verticalPos + horizontalPos; 
   lcd.setCursor(0, verticalPos);
   lcd.print(">");
   Serial.print(SerialRd);
   Serial.print(verticalPos);
   Serial.print(horizontalPos);
   Serial.println(atualPos);
 return;
}
 else if(pos == "e" && etapaMenu == 1){

  switch(atualPos){
    case 0:
      Serial.println("confirm in --CONTROL-");
      etapaMenu = 2;
      menuFase(etapaMenu);
      teleoperado();   
     return;
    break;

    case 1:
      Serial.println("confirm in --AUTO--");
      etapaMenu = 3;
      menuFase(etapaMenu);
      autonomo();
     return;
    break;

    case 2:
      Serial.println("confirm in --TESTS--");
      etapaMenu = 4;
      menuFase(etapaMenu);
      tests();
     return;
    break;
    default:
     return;
    break;
  }
}else if(pos == "d" && etapaMenu == 1){
   horizontalPos = 2;
   verticalPos = 0;
   atualPos = verticalPos + horizontalPos; 
   lcd.setCursor(9, 0);
   lcd.print(">");
   Serial.print(SerialRd);
   Serial.print(verticalPos);
   Serial.print(horizontalPos);   
   Serial.println(atualPos);   
}else if(pos == "a" && etapaMenu == 1){
   horizontalPos = 0;
   verticalPos = 0;
   atualPos = verticalPos + horizontalPos; 
   lcd.setCursor(0, verticalPos);
   lcd.print(">");
   Serial.print(SerialRd);
   Serial.print(verticalPos);
   Serial.print(horizontalPos);   
   Serial.println(atualPos);
}

else if(pos == "q" && etapaMenu > 1){
  Serial.print("Quit Requested On: ");
  Serial.println(etapaMenu);
  etapaMenu = 1;
  menuFase(etapaMenu);
  selects("w");
}

 else{ 
   lcd.setCursor(0, atualPos);
   lcd.print(">"); 
 return;}
}

void menuFase(int fase){  
 if(fase == 1){
   lcd.setCursor(1, 0);
   lcd.print("Control");
   lcd.setCursor(1, 1);
   lcd.print("Auto");
   lcd.setCursor(10, 0);
   lcd.print("Tests");
}else if(fase == 2){
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("CONTROL");
    lcd.setCursor(4, 1);
    lcd.print("Running");
    lcd.setCursor(15, 1);
    lcd.print("<");           
}
else if(fase == 3){
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("AUTONOMO");
    lcd.setCursor(4, 1);
    lcd.print("Running");
    lcd.setCursor(15, 1);
    lcd.print("<");    
}else if(fase == 4){
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Servos");
    lcd.setCursor(5, 1);
    lcd.print("&Motors");
    lcd.setCursor(15, 1);
    lcd.print("<");  
}

}

void tests(){
motor1.run(FORWARD);
motor2.run(FORWARD);
motor3.run(FORWARD);
motor4.run(FORWARD);
 for(int i = 0; i < 255; i++){
  motor1.setSpeed(i);
  motor2.setSpeed(i);
  motor3.setSpeed(i);
  motor4.setSpeed(i);
  Serial.println(i);
  delay(10);
 }

motor1.run(BACKWARD);
motor2.run(BACKWARD);
motor3.run(BACKWARD);
motor4.run(BACKWARD);
 for(int i = 255; i > 0; i--){
  motor1.setSpeed(i);
  motor2.setSpeed(i);
  motor3.setSpeed(i);
  motor4.setSpeed(i);
  Serial.println(i);
  delay(10);
 }
motor1.run(RELEASE);
motor2.run(RELEASE);
motor3.run(RELEASE);
motor4.run(RELEASE);
for(int i = 0; i < 180; i++){
  servo.write(i);
  Serial.println(i);
  delay(5);
}
for(int i = 180; i > 0; i--){
  servo.write(i);
  Serial.println(i);
  delay(5);
}
lcd.clear();
selects("q");
}



void teleoperado(){
  if (Serial.available()) {
  Reciver();
  }
}

void autonomo(){

}
