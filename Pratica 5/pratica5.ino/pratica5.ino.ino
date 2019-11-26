
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

#define ESTADO_NORMAL        0
#define ESTADO_DESVIO_D      1
#define ESTADO_DESVIO_E      2
#define ESTADO_RE            3


int PWMmotor = 0;
int converte =0;
int distancia = 0, anguloservo=0, i=0;
int pwm_horario=0, pwm_antihorario=0;
int sentido, CM;
int pwm_geral = 30;
int tempoCurva = 1000;
unsigned long tempopassado=0;
unsigned long tempopassado_D=0;
unsigned long tempopassado_E=0;

//Variavies relacionadas a maquina de estados
byte estadoAtual  = ESTADO_NORMAL;
byte estadoFuturo = ESTADO_NORMAL;

Servo cabeca;
Ultrasonic visao(trig,echo);

//FUNÇOES

void mov_motorB(int PWMmotor, int direcao){  
// PWMmotor de 0 - 100
// se direcao=1, vai pra frente
// se direcao=0, carrinho para
// e se direcao=-1, vai pra tras

//valor = analogRead(A0);
  if(direcao==1){ 
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
    pwm_horario = map(PWMmotor,0,100,0,255);
    analogWrite(ativaMB,pwm_horario);
    sentido=0;
    
  } else if(direcao==-1){
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
    pwm_antihorario = map(PWMmotor,0,100,0,255);
    analogWrite(ativaMB,pwm_antihorario);
    sentido=1;
    
  } else if(direcao==0){
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,LOW);
    pwm_antihorario = map(PWMmotor,0,100,0,255);
    analogWrite(ativaMB,0);
    sentido=2;
  }
}

void mov_motorA(int PWMmotor, int direcao){
// PWMmotor de 0 - 100
// se direcao=1, vai pra frente
// se direcao=0, carrinho para
// e se direcao=-1, vai pra tras

//valor = analogRead(A0);
  if(direcao==1){ 
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    pwm_horario = map(PWMmotor,0,100,0,255);
    analogWrite(ativaMA,pwm_horario);
    sentido=0;

  } else if(direcao==-1){
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    pwm_antihorario = map(PWMmotor,0,100,0,255);
    analogWrite(ativaMA,pwm_antihorario);
    sentido=1;
    
  } else if(direcao==0){
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
    pwm_antihorario = map(PWMmotor,0,100,0,255);
    analogWrite(ativaMA,0);
    sentido=2;
  }
}
  

void observar(){
  

  unsigned long tempoatual = millis();
  
 //converte = map(valor,0,1023,0,255); // conversão para variar de 0 - 180
 
 if(tempoatual - tempopassado >= 20){
 cabeca.write(i);
 distancia = visao.Ranging(CM);
 i=i+5;
 tempopassado=tempoatual;
 }
 if(i>=255){
  i=0;
  }
  
}

void estado_normal(){
  
   mov_motorA(pwm_geral,1);
   mov_motorB(pwm_geral,1);
   observar();

   if(distancia<=16 || distancia>=10){
    anguloservo = i;
      if(i<=127){
        estadoFuturo = ESTADO_DESVIO_E;
      } else if(i>127){
        estadoFuturo = ESTADO_DESVIO_D;
      }
   } else if(distancia<10){
    estadoFuturo = ESTADO_RE;
   }
}

void estado_desvioD(){
  unsigned long tempo_atual = millis();
  
  observar();
  mov_motorA(pwm_geral/4,1);
  mov_motorB(pwm_geral,1);
  if(tempo_atual - tempopassado_D >= tempoCurva){
    estadoFuturo = ESTADO_NORMAL; 
    tempopassado_D = tempo_atual; 
  }
 
}

void estado_desvioE(){
  unsigned long tempo_atual = millis();
  
  observar();
  mov_motorA(pwm_geral,1);
  mov_motorB(pwm_geral/4,1);
  if(tempo_atual - tempopassado_E >= tempoCurva){
    estadoFuturo = ESTADO_NORMAL; 
    tempopassado_E = tempo_atual; 
  }
 
}

void estado_re(){
  mov_motorA(pwm_geral,-1);
  mov_motorB(pwm_geral,-1);
  if(distancia>10){
    estadoFuturo = ESTADO_NORMAL;
  }
}

void maquina_de_estados(){
  switch(estadoAtual){
    case ESTADO_NORMAL:
      estado_normal();
      break;
    case ESTADO_DESVIO_D:
      estado_desvioD();
      break;
    case ESTADO_DESVIO_E:
      estado_desvioE();
      break;
    case ESTADO_RE:
      estado_re();
      break;
  }
//atualiza o estadoAtual;
  estadoAtual = estadoFuturo;
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
  pinMode(echo,INPUT);
  pinMode(trig,OUTPUT);
  
  cabeca.attach(PWMservo);
  
}

void loop() {

maquina_de_estados();

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

//  Serial.print(valor);
  Serial.print("\t");
  Serial.print(converte);
  Serial.print("\n");

//Serial.print(distancia);
//Serial.print("\n");
  
}
