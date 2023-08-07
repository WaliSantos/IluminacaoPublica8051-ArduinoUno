#include <LiquidCrystal.h>
LiquidCrystal lcd(10,9,8,7,6,5);

//DEFINE PORTAS DOS SENSORES/LED/BOTAO:
#define ldr A0
#define temp A2
#define amp A3
#define volt A5
#define led 13
   

//DEFINE SAIDA DOS SENSORES
int saida_ldr = 12;
int saidaSensores = 4;



//VARIAVEIS AUXILIARES:
int valor_temp, valor_ldr, valor_amp, valor_volt; // Valores dos sensores
bool status_led=1; // status do led inicia em 1 (led ligado)
bool status_botao_mestre = 1; // status do botao mestre inicia em 1 (led ligado)
char c; //
char buf[64];


void setup() {
  //LCD:
  lcd.begin(16,2);
 // INTERRUPÇÕES:
  attachInterrupt(0,intSensoresBotao,CHANGE); // Configurando interrupção botao mudança de borda
  attachInterrupt(1,recebeInformacao,CHANGE);
  //PINOS: 
  pinMode(led, OUTPUT);//Pino 13 como saida do led
  pinMode(volt,INPUT);// Sensor de voltagem como entrada
  pinMode(ldr, INPUT); // Sensor de luz como entrada
  pinMode(saida_ldr, OUTPUT); // Sensor ldr como saída
  pinMode(saidaSensores, OUTPUT); // Saída sensores como saída
  pinMode(3,OUTPUT);
  pinMode(A4,OUTPUT); // 0 PARA RECEBENDO 1 PARA ENVIANDO


  // SERIAL:
  Serial.begin(9600);  
}

void ligaLed(){
  digitalWrite(led, HIGH);  
}
void apagaLed(){
  digitalWrite(led, LOW); 
}

//FUNÇÕES DAS INTERRUPÇÕES:
void intSensoresBotao(){
  if(status_led == 1){
    apagaLed(); // estava ligado, vou desligar
    status_led = 0;
  }
  else{
    ligaLed();
    status_led= 1; //estava desligado, vou ligar
  }
}

void recebeInformacao(){
  c=Serial.read();
  if(c == 'F'){
      if(status_botao_mestre == 1){
        apagaLed();  
        status_botao_mestre = 0;
      }else{
        apagaLed();  
        status_botao_mestre = 1;
      }
  }else if(c == 'C'){;
    Serial.write('Z');
    sprintf(buf," T3: %d V3: %d A3: %d \r", valor_temp,valor_volt,valor_amp);
    Serial.write(buf);
    Serial.flush();
  }
  
  digitalWrite(3,LOW); // Deixa em low para ser acionada interrupção dnv dps
}

//LOOP:
void loop() { 
  //Recebendo e convertendo valores analogicos:
  valor_temp = ((analogRead(temp))/2-1); //Convertendo valor temp
  valor_volt = (analogRead(volt)/2); // convertendo voltagem, n ficou exato
  valor_amp = (analogRead(amp)/2); // não converti amper
  valor_ldr = analogRead(ldr);

  
  //PLOTANDO SENSORES NO LCD:
  lcd.setCursor(0,0);
  lcd.print("T=");
  lcd.setCursor(2,0);
  lcd.print(valor_temp);
  lcd.setCursor(5,0);
  lcd.print("V=");
  lcd.setCursor(7,0);
  lcd.print(valor_volt);
  lcd.setCursor(11,0);
  lcd.print("I=");
  lcd.setCursor(13,0);
  lcd.print(valor_amp);
  

  
  //Serial:
  delay(100);

  //Rotina principal:
  if(status_led == 1 and status_botao_mestre == 1){ //SE SENSORES E LDR/BOTAO ESTAO NA FAIXA
    ligaLed(); //Led ligado por default
  }
  while(Serial.available()>0){
    digitalWrite(3,HIGH); //Aciona interrupção 1
  }
  if((valor_volt > 297) or (valor_amp > 360) or (valor_temp > 60)){ // SE OS SENSORES ULTRAPASSAREM O LIMITE
    digitalWrite(saidaSensores, HIGH); //ACIONA INTERRUPÇÃO DOS SENSORES
  }else{ //SE OS SENSORES ESTIVEREM NO LIMITE, TESTAR LUZ E BOTÃO:
      digitalWrite(saidaSensores, LOW); //Mantém ou alterna para LOW CASO SENSORES ESTEJAM NA FAIXA
      if(valor_ldr > 49){ // SE O LDR ULTRAPASSAR A FAIXA
      digitalWrite(saida_ldr, HIGH); //ACIONA A INTERRUPÇÃO DO LDR
      }else {
      digitalWrite(saida_ldr, LOW); //Mantém ou alterna para LOW CASO LDR ESTEJA NA FAIXA
    }   
  } 
}
