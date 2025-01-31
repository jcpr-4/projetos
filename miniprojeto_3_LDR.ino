#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int LDR=A0; //define o pino analógico para leitura do LDR
int rele=2; //define o pino digital 2 para acionar o relé quea aciona a carga

int sensorLDR; //Armazena o valor analógico lido do LDR
const int valor_ligar= 950; //valor máximo de limiar
const int valor_desligar= 250; // valor mínimo de limiar 

void setup()
{
  Serial.begin(9600);

  //inicializa o display
  lcd.begin(16, 2);

  pinMode(LDR, INPUT);
  pinMode(rele,OUTPUT);
  pinMode(13,OUTPUT);

}

void loop()
{
  sensorLDR=analogRead(LDR); //Leitura do LDR sendo armazenada na variável sensorLDR
  Serial.print("Leitura do LDR: ");
  Serial.println(sensorLDR);
	
  //Bloco de código para exibir as informações no display LCD
  lcd.setCursor(0,0);
  lcd.print("Brilho: ");
  lcd.setCursor(8,0);
  lcd.print(sensorLDR);
  lcd.setCursor(0,1);
  lcd.print("Estado: ");
  
  //Bloco de código para realizar acionamento da carga por meio dos valores de limiar
  if(sensorLDR>valor_desligar && sensorLDR<valor_ligar)
  {
    //Bloco de código que será executado quando o valor 
    //lido do LDR estiver na faixa dos valores de limiar mínimo e máximo
    digitalWrite(rele,HIGH);
    digitalWrite(13,HIGH);
    lcd.setCursor(8,1);
    lcd.print("Ligado");
    
  }else{
    digitalWrite(rele,LOW);
    digitalWrite(13,LOW);
    lcd.setCursor(7,1);
    lcd.print("Desligado");
    
  }
  delay(1000);
  lcd.clear();
}
