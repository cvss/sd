/*---------------------------------------------
# AUTOR: Cecilia Virginia e Jessica Terto      
# DATA:  11/10/2019
# DESCRICAO: 
#        Pratica 3 - Parte 1 2 e 3 -Sistemas Digitais 1 - 2019.2
#        Codigo funcional para o projeto descrito no arquivo pratica_3.pdf
#        O intuito e mostrar um codigo que usa maquina de estados para 
#        implementar 
#---------------------------------------------- */

// ----------- DEFINES---------------------------------------------
//PINOS SETESEGMENTOS
#define D4 9  //Pino de enable do digito 4 do display de 7 segmentos
#define D3 10 //Pino de enable do digito 3 do display de 7 segmentos
#define D2 11 //Pino de enable do digito 2 do display de 7 segmentos
#define D1 12 //Pino de enable do digito 1 do display de 7 segmentos

//LEDS
#define LED LED_BUILTIN //LED da placa

//BOTOES
#define BOTAO_1 18 // Entrada digital
#define BOTAO_2 19 // Entrada digital

//ESTADOS DA 'MAQUINA DE ESTADOS'
#define ESTADO_NORMAL           0 //Estado em que uma borda de subida incrementa ou decrementa o valor de 'contador'
#define ESTADO_3S_INCREMENTA    1 //Ocorreu uma borda de subida do botao que incrementa 'contador' e conta 3s
#define ESTADO_3S_DECREMENTA    2 //Ocorreu uma borda de subida do botao que decrementa 'contador' e conta 3s
#define ESTADO_100MS_INCREMENTA 3 //Incrementa 'contador' a cada 100ms 
#define ESTADO_100MS_DECREMENTA 4 //Decrementa 'contador' a cada 100ms 
#define ESTADO_NORMAL2          5 //Estado normal para a maquina de estados 2.
#define ESTADO_CONFIG           6 //Estado de configuracao.
#define ESTADO_PISCA3x          7 //Pisca 3 vezes os displays.
#define ESTADO_DIGITO_CONFIG    8


//-------------FIM DEFINES---------------------------------------------//
//~~~~~~~~~~~~~ VARIAVEIS GLOBAIS E CONSTANTES~~~~~~~~~~~~~~~~~~~~~~~~~// 
 const bool seven_seg_digits[16][7] = { 
 { 1,1,1,1,1,1,0 },  //DIGITO 0
 { 0,1,1,0,0,0,0 },  //DIGITO 1
 { 1,1,0,1,1,0,1 },  //DIGITO 2
 { 1,1,1,1,0,0,1 },  //DIGITO 3
 { 0,1,1,0,0,1,1 },  //DIGITO 4
 { 1,0,1,1,0,1,1 },  //DIGITO 5
 { 1,0,1,1,1,1,1 },  //DIGITO 6
 { 1,1,1,0,0,0,0 },  //DIGITO 7
 { 1,1,1,1,1,1,1 },  //DIGITO 8
 { 1,1,1,0,0,1,1 },  //DIGITO 9
 { 1,1,1,0,1,1,1 },  //DIGITO A
 { 0,0,1,1,1,1,1 },  //DIGITO B
 { 1,0,0,1,1,1,0 },  //DIGITO C
 { 0,1,1,1,1,0,1 },  //DIGITO D
 { 1,0,0,1,1,1,1 },  //DIGITO E
 { 1,0,0,0,1,1,1 }   //DIGITO F
};

// Guarda o valor atual de cara um dos digitos
// valor_digito[0] guarda do valor de  D4
// valor_digito[1] guarda do valor de  D3
// valor_digito[2] guarda do valor de  D2
// valor_digito[3] guarda do valor de  D1
byte valor_digito[4] = {0,0,0,0};

//Variaveis utilizadas para funcao microVivo()
bool ledState = LOW;   //indica se o LED esta ligado ou desligado

//Variaveis usadas para contar tempo
unsigned long previousMillis          = 0;  //guarda variavel para ver se passou 1s
unsigned long previous3sIncrementa    = 0;  //guarda se ocorreu 3s, vindo de um incremento
unsigned long previous3sDecrementa    = 0;  //guarda se ocorreu 3s, vindo de um decremento
unsigned long previous100msIncrementa = 0;  //guarda se ocorreu 100ms, vindo de um incremento
unsigned long previous100msDecrementa = 0;  //guarda se ocorreu 100ms, vindo de um decremento
const long interval3s    = 3000;            //Intervalo de 3s.
const long interval1s    = 1000;            //Intervalo de 1s.
const long interval500ms = 500;             //Intervalo de 500ms.
const long interval100ms = 100;             //Intervalo de 100ms.
const long interval50ms  = 50;
//Variaveis usadas para contar tempo
int segundos = 0;                    //guarda os segundos do contador (0 - 59)
int minutos  = 0;                    //guarda os minutos do contador

//Variavel usada para contar variacoes dos botoes de entrada
unsigned int contador = 0; //Conta entradas

//Variaveis usadas para pegar a borda de subida
int lastBOTAO_1 = LOW;   // ultima leitura do BOTAO_1
int lastBOTAO_2 = LOW;   // ultima leitura do BOTAO_2

//Variavies relacionadas a maquina de estados
byte estadoAtual  = ESTADO_NORMAL; //guarda o estado (modo) atual do sistema
byte estadoFuturo = ESTADO_NORMAL; //guarda o estado (modo) futuro do sistema: depende
                                   //do estado atual e das entradas em um dado instante
byte estadoAtual2  = ESTADO_NORMAL2;
byte estadoAnterior = ESTADO_NORMAL2;
byte digitoConfigurando = 0;

//Variaveis usadas em modo config
unsigned long previous3s   = 0; //guarda se ocorreu 3s.
unsigned long previous3ss  = 0;
unsigned long previous50ms = 0;
unsigned long count100ms   = 0;

unsigned long contadorNormal2 = 0; //conta o tempo dentro do modo normal2

//Variaveis do display
int apagueiDisplay = 0;
unsigned long contadorAlternaDisplay = 0;
bool acendeDisplay = true;
bool startCount = true;

// ~~~~~~~~~~~~ FIM VARIAVEIS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

// ++++++++++++FUNCOES ++++++++++++++++++++++++++++++++++++++++++++++++//
/*Funcao que configura os pinos digitais como saida para serem usados
pelos displays de 7 segmentos*/
void configuraSegmentosDisplay()
{
  pinMode(2, OUTPUT);  //PINO 2 -> SEGMENTO A  
  pinMode(3, OUTPUT);  //PINO 3 -> SEGMENTO B
  pinMode(4, OUTPUT);  //PINO 4 -> SEGMENTO C
  pinMode(5, OUTPUT);  //PINO 5 -> SEGMENTO D
  pinMode(6, OUTPUT);  //PINO 6 -> SEGMENTO E
  pinMode(7, OUTPUT);  //PINO 7 -> SEGMENTO F
  pinMode(8, OUTPUT);  //PINO 8 -> SEGMENTO G
  pinMode(9, OUTPUT);  //PINO 9 -> D4
  pinMode(10, OUTPUT); //PINO 10 -> D3
  pinMode(11, OUTPUT); //PINO 11 -> D2
  pinMode(12, OUTPUT); //PINO 12 -> D1
  //Inicializa todos os digitos desligados:
  //os 7 seguimentos ligados em zero, logo:
  digitalWrite(D4,HIGH); // Deliga digito 4: menos significativo
  digitalWrite(D3,HIGH); // Deliga digito 3
  digitalWrite(D2,HIGH); // Deliga digito 2
  digitalWrite(D1,HIGH); // Deliga digito 1: mais significativo
}


/*Funcao que configura os A5 e A4 como entradas digitais*/
void configuraEntradas()
{
  pinMode(BOTAO_1,INPUT); //PINO 18 - ENTRADA
  pinMode(BOTAO_2,INPUT); //PINO 19 - ENTRADA
}


/*Funcao liga um display (D1, D2, D3, D4, indicado pela variavel de entrada 'digito_7') por um milisegundo
com o numero (0-15) passado para funcao (variavel valor)*/
void ligaSegmentosDisplay(byte valor, byte digito_7)
{ 
  byte pino = 2; //PINO INICIAL DOS LEDS (ja que A = 2, G = 9)
  byte contadorSegmentos = 0; // variavel para ser usada no loop 
  //
  digitalWrite(digito_7,LOW); //HABILITA QUAL DOS DISPLAYS IRA SER LIGADO:
  
  for (contadorSegmentos = 0; contadorSegmentos < 7; ++contadorSegmentos)
  { //PARA "contadorSegmentos"
    //IGUAL A 0, ENQUANTO "contadorSegmentos" MENOR QUE 7, INCREMENTA "contadorSegmentos"
    digitalWrite(pino + contadorSegmentos, seven_seg_digits[valor][contadorSegmentos]); //PERCORRE O ARRAY E LIGA OS
                                                                                        //SEGMENTOS CORRESPONDENTES AO DIGITO
  }
  delay(1);                    // Espera 1 ms
  digitalWrite(digito_7,HIGH); // DESABILITA OS DISPLAYS QUE ESTAVA LIGADO:
}


/*Funcao varre os quatro digitos do display de 7 segmentos deixando cada um deles ligado por 1ms
o valor de cada um deles e guardado em um vetor 'valor_digito' definido como global*/
void atualizaDiplays(int v = -1)
{
  int i; // variavel de iteracao
  for (i = 0; i < 4; i++)
  { //PARA "contador"
    if(i != v)
     ligaSegmentosDisplay(valor_digito[i],i + D4); //Contador + D4, pq os enables comecam a partir do pino 9 = D4;
  }
}

/*Funcao conta 1 segundo e inverte o valor LED da placa fazendo ele piscar*/
void microVivo()
{
  unsigned long currentMillis = millis(); //salva o valor atual do contador de tempo da placa

  if (currentMillis - previousMillis >= interval1s) // verifica se passou 1 segundo
  {

    //Isso e usando em outra função - atualizaVetorDisplay( )
    segundos++; // Atualiza a contagem dos segundos. A atualizacao do valor_digito apenas ocorrera na funcao atualizaVetorDisplay()

    //Atualiza o estado atual do contador de milisegundos
    previousMillis = currentMillis;


    ledState = !ledState;       //inverte o valor do estado atual do LED;
    digitalWrite(LED,ledState); //Atualiza o LED
  }
}

/*Funcao atualiza o valor que sera escrito dos digitos do display de 7 segmentos*/
void atualizaVetorDisplay()
{
  if(segundos > 59)
  {
    segundos = segundos - 60;
    minutos++;
  }
  if(minutos > 59)
  {
    minutos = 0;  
  }
  valor_digito[0] = segundos%10;
  valor_digito[1] = segundos/10;
  valor_digito[2] = minutos%10;
  valor_digito[3] = minutos/10;
}

void modoNormal2()
{
  bool lerBOTAO_1; //guarda o valor lido de BOTAO_1
  bool lerBOTAO_2; //guarda o valor lido de BOTAO_1
  
  atualizaVetorDisplay();

  lerBOTAO_1 = bordaBOTAO_1(); //le BOTAO_1 e verifica se ocorreu uma borda de subida
  lerBOTAO_2 = bordaBOTAO_2(); //le BOTAO_2 e verifica se ocorreu uma borda de subida

  if(startCount && (lerBOTAO_1 || lerBOTAO_2))
  {
    contadorNormal2 = millis();
    startCount = false;

    estadoFuturo = ESTADO_NORMAL2;
  }
  else if((lerBOTAO_1 || lerBOTAO_2) && ((millis() - contadorNormal2) >= interval3s))
  {
    contadorNormal2 = millis();
    startCount = true;

    contadorAlternaDisplay = millis();
    acendeDisplay = false;
    apagueiDisplay = 0;
    estadoFuturo = ESTADO_PISCA3x;
    estadoAnterior = ESTADO_NORMAL2;
  }
  else if(!lerBOTAO_1 && !lerBOTAO_2)
  {
    contadorNormal2 = millis();
    startCount = true;

    estadoFuturo = ESTADO_NORMAL2;
  }
}

//Funcao que implementa o sistema funcionando no modo normal
void estadoNormal()
{
  bool lerBOTAO_1; //guarda o valor lido de BOTAO_1
  bool lerBOTAO_2; //guarda o valor lido de BOTAO_1
  lerBOTAO_1 = bordaBOTAO_1(); //le BOTAO_1 e verifica se ocorreu uma borda de subida
  lerBOTAO_2 = bordaBOTAO_2(); //le BOTAO_2 e verifica se ocorreu uma borda de subida

  if(lerBOTAO_1)
  {
    // SAIDAS
    contador++;
    // INICIALIZA O PROXIMO ESTADO
    previous3sIncrementa = millis();
    estadoFuturo = ESTADO_3S_INCREMENTA;
  }
  if(lerBOTAO_2)
  {
    //SAIDAS
     contador--;
    // INICIALIZA O PROXIMO ESTADO
    previous3sDecrementa = millis();
    estadoFuturo = ESTADO_3S_DECREMENTA;
  }
   
  valor_digito[0] = (contador%10);        //unidade
  valor_digito[1] = ((contador/10)%10);   //dezena
  valor_digito[2] = ((contador/100)%10);  //centana
  valor_digito[3] = ((contador/1000)%10); //milhar
  
}

// Pega a borda de subida do BOTAO_1, retorna 1 quando ocorreu uma borda
int bordaBOTAO_1()
{
  //le o estado atual da entrada
  bool reading = digitalRead(BOTAO_1);

  if ((reading==1)&&(lastBOTAO_1==0)) // Pega a borda de subida do BOTAO_1
  {
    lastBOTAO_1 = reading; //atualiza o valor da ultima leitura
    return 1;
  }
  else
  {
    lastBOTAO_1 = reading; //atualiza o valor da ultima leitura
    return 0;
  }
}

// Pega a borda de subida do BOTAO_2, retorna 1 quando ocorrou uma borda
int bordaBOTAO_2()
{
  //le o estado atual da entrada
  bool reading = digitalRead(BOTAO_2);

  if ((reading==1)&&(lastBOTAO_2==0)) // Pega a borda de subida do BOTAO_2
  {
    lastBOTAO_2 = reading; //atualiza o valor da ultima leitura
    return 1;
  }
  else
  {
    lastBOTAO_2 = reading; //atualiza o valor da ultima leitura
    return 0;
  }
}

//Funcao que implementa o estado intermediario que espera 3s com o botao abertado
//caso tenha vindo de um incremento (BOTAO1)
void estado3sIncrementa()
{
  unsigned long currentMillis3s = millis(); //salva o valor atual do contador de tempo da placa
  // le o estado atual do BOTAO_1
  bool reading = digitalRead(BOTAO_1);
  
  if((reading==1)&&(lastBOTAO_1==1)) //Se o BOTAO_1 continua apertado
  {
    if (currentMillis3s - previous3sIncrementa >= interval3s) // verifica se passou 3 segundo
    {
      //Atualiza o estado atual do contador de milisegundos
       previous3sIncrementa = currentMillis3s;
      //INICIALIZA O NOVO ESTADO
       previous100msIncrementa = currentMillis3s;
       estadoFuturo = ESTADO_100MS_INCREMENTA;
    }   
  }
  else // O botao foi solto antes dos 3s
  {
    estadoFuturo = ESTADO_NORMAL;
  }
  lastBOTAO_1 = reading; //Atualiza a ultima leitura do botao
}

//Funcao que implementa o estado que espera 100ms com o botao abertado
// e incrementa 'contador'
void estado100msIncrementa()
{
  unsigned long currentMillis100ms = millis(); //salva o valor atual do contador de tempo da placa
  // le o estado atual do BOTAO_1
  bool reading = digitalRead(BOTAO_1);
  
  if((reading==1)&&(lastBOTAO_1==1)) //Se o botao continua apertado
  {
    if (currentMillis100ms - previous100msIncrementa >= interval100ms) // verifica se passou 100ms
    {
      //Atualiza o estado atual do contador de milisegundos
      previous100msIncrementa = currentMillis100ms;
      //ATUALIZA SAIDA
      contador++;
    }   
  }
  else //se o botao foi solto
  {
    estadoFuturo = ESTADO_NORMAL;
  }

  lastBOTAO_1 = reading;
  
  valor_digito[0] = (contador%10);
  valor_digito[1] = ((contador/10)%10);
  valor_digito[2] = ((contador/100)%10);
  valor_digito[3] = ((contador/1000)%10);
}

//Funcao que implementa o estado intermediario que espera 3s com o botao abertado
//caso tenha vindo de um incremento (BOTAO1)
void estado3sDecrementa()
{
  unsigned long currentMillis3s = millis(); //salva o valor atual do contador de tempo da placa
  //  le o estado atual do BOTAO_2
  bool reading = digitalRead(BOTAO_2);
  
  if((reading==1)&&(lastBOTAO_2==1)) //Verifica se o botao continua apertado
  {
    if (currentMillis3s - previous3sDecrementa >= interval3s) // verifica se passou 3 segundo
    {
      //Atualiza o estado atual do contador de milisegundos
       previous3sDecrementa = currentMillis3s;
      //INICIALIZA O NOVO ESTADO
       previous100msDecrementa = currentMillis3s;
       estadoFuturo = ESTADO_100MS_DECREMENTA;
    }   
  }
  else //caso o botao tenha sido solto
  {
    estadoFuturo = ESTADO_NORMAL;
  }
  lastBOTAO_2 = reading;
}

//Funcao que implementa o estado que espera 100ms com o botao abertado
// e incrementa 'contador'
void estado100msDecrementa()
{
  unsigned long currentMillis100ms = millis(); //salva o valor atual do contador de tempo da placa
  // le o estado atual do BOTAO_2
  bool reading = digitalRead(BOTAO_2);
  
  if((reading==1)&&(lastBOTAO_2==1))
  {
    if (currentMillis100ms - previous100msDecrementa >= interval100ms) // verifica se passou 100ms
    {
      //Atualiza o estado atual do contador de milisegundos
      previous100msDecrementa = currentMillis100ms;
      //ATUALIZA SAIDA
      contador--;
    }   
  }
  else 
  {
    estadoFuturo = ESTADO_NORMAL;
  }

  lastBOTAO_2 = reading;
  
  valor_digito[0] = (contador%10);
  valor_digito[1] = ((contador/10)%10);
  valor_digito[2] = ((contador/100)%10);
  valor_digito[3] = ((contador/1000)%10);
}


// Funcao que implementa a maquina de estados e chama a funcao que trata cada um dos 
// estados (modos) que o 
void maquina_de_estados()
{
  switch(estadoAtual)
  {
    case ESTADO_NORMAL:
      estadoNormal();
      break;
    case ESTADO_3S_INCREMENTA:
      estado3sIncrementa();
      break;
    case ESTADO_3S_DECREMENTA:
      estado3sDecrementa();
      break;
    case ESTADO_100MS_INCREMENTA:
      estado100msIncrementa();
      break;
    case ESTADO_100MS_DECREMENTA:
      estado100msDecrementa();
      break;
  }
  //Atualiza o estadoAtual
  estadoAtual = estadoFuturo;
}

void maquina_de_estados2()
{
  switch(estadoAtual2)
  {
    case ESTADO_NORMAL2:
      modoNormal2();
      break;
    case ESTADO_PISCA3x:
      modoPisca3x();
      break;
    case ESTADO_CONFIG:
      modoConfig();
      break;
    case ESTADO_DIGITO_CONFIG:
      modoDigitoConfig();
      break;
    case ESTADO_100MS_DECREMENTA:
      estado100msDecrementa();
      break;
  }
  //Atualiza o estadoAtual
  estadoAtual2 = estadoFuturo;
}

void modoConfig()
{
  unsigned long currentMillis3s = millis(); //salva o valor atual do contador de tempo da placa
  // le o estado atual do BOTAO_1
  bool reading = digitalRead(BOTAO_1);
  bool reading2 = digitalRead(BOTAO_2);
  
  if(((reading2==1)||(reading==1))&&(lastBOTAO_1==1)) //Se o BOTAO_1 continua apertado
  {
    if (currentMillis3s - previous3s >= interval3s) // verifica se passou 3 segundo
    {
      
      //Atualiza o estado atual do contador de milisegundos
       previous3s = currentMillis3s;
      //INICIALIZA O NOVO ESTADO
       estadoFuturo = ESTADO_PISCA3x;
    }   
  }
  else // O botao foi solto antes dos 3s
  {
    estadoFuturo = ESTADO_NORMAL2;
  }
  lastBOTAO_1 = reading; //Atualiza a ultima leitura do botao
  lastBOTAO_2 = reading2;
}

void modoPisca3x()
{
  if((millis() - contadorAlternaDisplay >= interval500ms) && !acendeDisplay)
  {
    acendeDisplay = true;
    contadorAlternaDisplay = millis();

    apagueiDisplay += 1;
    if(apagueiDisplay == 2)
    {
      if(estadoAnterior == ESTADO_DIGITO_CONFIG)
      {
        estadoFuturo = ESTADO_NORMAL2;
      }
      else if(estadoAnterior == ESTADO_NORMAL2)
      {
        estadoFuturo = ESTADO_DIGITO_CONFIG;
        count100ms = millis();
        digitoConfigurando = 0;
      }
    }
  }

  if(( (millis() - contadorAlternaDisplay) < interval500ms) && acendeDisplay)
  {
    atualizaDiplays();
  }
  else if(( (millis() - contadorAlternaDisplay) >= interval500ms) && acendeDisplay)
  {
    acendeDisplay = false;
    contadorAlternaDisplay = millis();
  }
   
}

void modoDigitoConfig()
{
  unsigned long currentMillis3s = millis(); //salva o valor atual do contador de tempo da placa
  // le o estado atual do BOTAO_1
  bool reading = digitalRead(BOTAO_1);
  bool reading2 = digitalRead(BOTAO_2);
  
  if(currentMillis3s - count100ms > 100){
    atualizaDiplays(digitoConfigurando);
    if(currentMillis3s - count100ms > 200){
      count100ms = millis();
    }
  } else {
    atualizaDiplays();
  }

  if((reading==1) || (reading2==1)) //Se BOTAO for apertado
  {
      if(reading2){
        if(digitoConfigurando = 0){
          segundos++;
        } else if (digitoConfigurando == 1){
          segundos += 10;
        } else if (digitoConfigurando == 2){
          minutos += 1;
        } else if (digitoConfigurando == 3){
          minutos += 10;
        }
        atualizaVetorDisplay();
      }
      if(reading){
        digitoConfigurando++;
        if(digitoConfigurando >= 4){
          digitoConfigurando = 0;
        }
      }
      //Atualiza o estado atual do contador de milisegundos
       previous3ss = currentMillis3s;    
  }

  if (currentMillis3s - previous3ss >= interval3s) // verifica se passou 3 segundo
    {
      estadoFuturo = ESTADO_PISCA3x;
      estadoAnterior = ESTADO_DIGITO_CONFIG;
      apagueiDisplay = 0;
  }

  lastBOTAO_1 = reading; //Atualiza a ultima leitura do botao 
}

// ++++++++++++ FIM FUNCOES ++++++++++++++++++++++++++++++++++++++++++++++++//
//##############CONFIGURACAO DAS ENTRADAS E SAIDAS#####################//
void setup()
{
  configuraSegmentosDisplay(); // Configura os pinos ligados ao display de sete segmentos
  configuraEntradas();         // Configura os pinos ligados aos botoes como entrada
  pinMode(LED, OUTPUT);        // Configura o pino do LED da placa como saida
}

//##############LOOP PRINCIPAL#####################//
void loop() {
  microVivo();                 // togglar o led 
  //atualizaVetorDisplay();    // atualiza minutos e segundos
  //atualizaVetorContador();   // Verifica se algum botao foi apertado
  maquina_de_estados2();        // gerencia as maquinas de estados
 atualizaDiplays();           // escreve nos displays
  //modoPisca3x();
}
