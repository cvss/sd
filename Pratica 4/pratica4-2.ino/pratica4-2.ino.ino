

/*---------------------------------------------
# AUTOR: Cecilia Virginia e Jessica Terto    
# DATA:  07/11/2019
# DESCRICAO: 
#        Pratica 4 - Parte 1 2 e 3 -Sistemas Digitais 1 - 2019.2
#         
#---------------------------------------------- */

#include <Servo.h>
#include <Ultrasonic.h>

int valor = 0, trig = 4, echo = 5;
int led = 3;
int converte =0;
int distancia = 0;
int IN1 = 2, IN2 = 4, IN3 = 6, IN4 = 7, ativaMA = 3, ativaMB = 5;
int pwm_horario=0, pwm_antihorario=0;
int sentido;

Servo carrinho;
Ultrasonic visao(trig,echo);

void setup() {
  Serial.begin(9600);
  pinMode(A0,INPUT);
  //pinMode(led,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(ativaMA,OUTPUT);
  pinMode(ativaMB,OUTPUT);
  
  //carrinho.attach(led);
  
}

void loop() {

valor = analogRead(A0);
  if(valor>=512){
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
    pwm_horario = map(valor,512,1023,0,255);
    analogWrite(ativaMA,pwm_horario);
    analogWrite(ativaMB,pwm_horario);
    sentido=0;
    
  } else if(valor<=511){
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
    pwm_antihorario = map(valor,511,0,0,255);
    analogWrite(ativaMA,pwm_antihorario);
    analogWrite(ativaMB,pwm_antihorario);
    sentido=1;
    
  }
if(sentido==0){
  Serial.print("Sentido Horario");
  Serial.print("\n");
  Serial.print(digitalRead(IN1));
  Serial.print("\t");
  Serial.print(digitalRead(IN2));
  Serial.print("\t");
  Serial.print(digitalRead(IN3));
  Serial.print("\t");
  Serial.print(digitalRead(IN4));
  Serial.print("\t");
  Serial.print("\n");
  Serial.print("PWM:");
  Serial.print("\t");
  Serial.print(pwm_horario);
  Serial.print("\n");
  
} else if(sentido==1){
  Serial.print("Sentido Antihorario");
  Serial.print("\n");
  Serial.print(digitalRead(IN1));
  Serial.print("\t");
  Serial.print(digitalRead(IN2));
  Serial.print("\t");
  Serial.print(digitalRead(IN3));
  Serial.print("\t");
  Serial.print(digitalRead(IN4));
  Serial.print("\t");
  Serial.print("\n");
  Serial.print("PWM:");
  Serial.print("\t");
  Serial.print(pwm_antihorario);
  Serial.print("\n");
}
//converte = map(valor,0,1023,0,255);
//  carrinho.write(converte);
//  analogWrite(led,converte);
//  Serial.print(valor);
//  Serial.print("\t");
//  Serial.print(converte);
//  Serial.print("\n");

  
distancia = visao.Ranging(CM);
//Serial.print(distancia);
//Serial.print("\n");

  delay(100);

  
}
