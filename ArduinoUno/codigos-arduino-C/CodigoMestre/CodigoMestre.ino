#include <TimerOne.h>
#define btn 2 // Botao acionando diretamente a interrupção
char valor_recebido;
String buf;
String buf2;
String buf3;
void setup() {
  //PINOS:
  pinMode(btn, INPUT);

  // INTERRUPÇÕES:
  attachInterrupt(digitalPinToInterrupt(btn),desligaTudo,CHANGE);
  Timer1.attachInterrupt(solicitaRecebimento);
  //TIMER:
  Timer1.initialize(3000000);  //Inicialização do timer 1 (auto reload)-> 3 segundos
  //SERIAL:
  Serial.begin(9600);
}

void loop() {
   while(Serial.available()>0){
      valor_recebido = Serial.read();
      if(valor_recebido == 'X'){
        buf = Serial.read();
        Serial.flush();
        
        
      }else if(valor_recebido == 'Y'){
        buf2 = Serial.read();
        Serial.flush();
             
      }else if(valor_recebido == 'Z'){
        buf3 = Serial.read();
        Serial.flush();
      }
      
   }
}

void solicitaRecebimento(){
  Serial.write('A'); //Pede informação para o MIC A
  Serial.flush();
  Serial.write('B'); //Pede informação para o MIC B
  Serial.flush();
  Serial.write('C'); //Pede informação para o MIC C
}
void desligaTudo(){
  Serial.write('F');
  Serial.flush();
  
}
