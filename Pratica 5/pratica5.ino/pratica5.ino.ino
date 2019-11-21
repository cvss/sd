
/*---------------------------------------------
# AUTOR: Cecilia Virginia e Jessica Terto    
# DATA:  21/11/2019
# DESCRICAO: 
#        Pratica 5 - Parte 1 2 e 3 -Sistemas Digitais 1 - 2019.2
#         
#---------------------------------------------- */

#include <Servo.h>
#include <Ultrasonic.h>

//-------------PINOS-------------
#define trig      9
#define echo      8
#define IN1       2
#define IN2       4
#define IN3       6
#define IN4       7
#define ativaMA   3
#define ativaMB   5
#define PWMservo  10
//-------------------------------
int PWMmotor = 0;
int led = 3;
int converte =0;
int distancia = 0;
int pwm_horario=0, pwm_antihorario=0;
int sentido;

Servo carrinho;
Ultrasonic visao(trig,echo);

//FUNÇOES

void movimento(int PWMmotor, int direcao){  
// PWMmotor de 0 - 100
// se direcao=1, vai pra frente
// se direcao=0, carrinho para
// e se direcao=-1, vai pra tras

//valor = analogRead(A0);
  if(direcao==1){ 
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
    pwm_horario = map(PWMmotor,0,100,0,255);
    analogWrite(ativaMA,pwm_horario);
    analogWrite(ativaMB,pwm_horario);
    sentido=0;
    
  } else if(direcao==-1){
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
    pwm_antihorario = map(PWMmotor,0,100,0,255);
    analogWrite(ativaMA,pwm_antihorario);
    analogWrite(ativaMB,pwm_antihorario);
    sentido=1;
    
  } else if(direcao==0){
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,LOW);
    pwm_antihorario = map(PWMmotor,0,100,0,255);
    analogWrite(ativaMA,0);
    analogWrite(ativaMB,0);
    sentido=2;
  }
}

void visao(){
  
}

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
//converte = map(valor,0,1023,0,255); // conversão para variar de 0 - 180
//  carrinho.write(converte);
//  analogWrite(led,converte);
//  Serial.print(valor);
//  Serial.print("\t");
//  Serial.print(converte);
//  Serial.print("\n");

  
//distancia = visao.Ranging(CM);
//Serial.print(distancia);
//Serial.print("\n");

  delay(100);

  
}
