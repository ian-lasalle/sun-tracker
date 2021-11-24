#include <LiquidCrystal.h>
#include <Servo.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Servo servohori;
Servo servoverti; 
int servoh = 0;
int servohLimitHigh = 180;
int servohLimitLow = 10;
int servov = 0; 
int servovLimitHigh = 180;
int servovLimitLow = 10;
//LDRs
int ldrtopl = A0; //top left
int ldrtopr = A3; //top right
int ldrbotl = A1; // bottom left 
int ldrbotr = A2; // bottom right

float i = 0.0;
float v = 0.0;
float sensibilidad = 0.1; //100 mv/A = 0.1 V/A

void imprimirResultados();
//float calcularCorriente(int);

void setup() {
  lcd.begin(16, 2);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  servohori.attach(10);
  servohori.write(0);
  servoverti.attach(9);
  servoverti.write(0);
  delay(10);
  Serial.begin(9600);
}

void loop() {
  servoh = servohori.read();
  servov = servoverti.read();
  //Valores analogicos por cada LDR
  int topl = analogRead(ldrtopl);
  int topr = analogRead(ldrtopr);
  int botl = analogRead(ldrbotl);
  int botr = analogRead(ldrbotr);
  //Promedios
  int avgtop = (topl + topr)/2; 
  int avgbot = (botl + botr)/2; 
  int avgleft = (topl + botl)/2; 
  int avgright = (topr + botr)/2;

  //top & bottom
  if(avgtop < avgbot){
    servoverti.write(servov +1);
    if (servov > servovLimitHigh) { 
      servov = servovLimitHigh;
    }
    delay(5);
  }else if(avgbot < avgtop){
    servoverti.write(servov -1);
    if(servov < servovLimitLow){
      servov = servovLimitLow;
    }
    delay(5);
  }else{
    servoverti.write(servov);
  }

  //left & right
  if(avgleft > avgright){
    servohori.write(servoh +1);
    if(servoh > servohLimitHigh){
      servoh = servohLimitHigh;
    }
    delay(5);
  }else if(avgright > avgleft){
    servohori.write(servoh -1);
    if (servoh < servohLimitLow){
     servoh = servohLimitLow;
    }
    delay(5);
  }else{
    servohori.write(servoh);
  }
  
  delay(3);

//  v = analogRead(A4)*(5.0/1023.0);
//    i = (v - 2.5 )/sensibilidad;
// // i = calcularCorriente(500);
//  Serial.print("v = ");
//  Serial.println(v);
//  imprimirResultados();
  delay(300);
}

float calcularCorriente(int iteraciones){
  float v = 0.0;
  float corriente = 0.0;
  for(int j = 0; j < iteraciones; j++){
      v = analogRead(A4)*(5.0/1023.0);
      corriente += (v - 2.5)/sensibilidad;
  }
  corriente = corriente/iteraciones; //corriente promedio de n-iteraciones
  return (corriente); 
}

void imprimirResultados(){
  delay(1);
  lcd.setCursor(0, 0);
  lcd.print("i = ");
  lcd.print(i*1000);
  lcd.setCursor(14, 0);
  lcd.print("mA");
  
  lcd.setCursor(0, 1);
  lcd.print("W = ");
  lcd.print(v*i);
  lcd.setCursor(15, 1);
  lcd.print("W");
}
