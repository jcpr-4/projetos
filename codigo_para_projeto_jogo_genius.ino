// --- Mapeamento de Hardware ---
#define  led1    10             //LED1 ligado no pino digital 10
#define  led2    11             //LED2 ligado no pino digital 11
#define  led3    12             //LED3 ligado no pino digital 12
#define  led4    13             //LED4 ligado no pino digital 13
#define  botao1   5             //Botão 1 ligado no pino digital 5
#define  botao2   6             //Botão 2 ligado no pino digital 6
#define  botao3   7             //Botão 3 ligado no pino digital 7
#define  botao4   4             //Botão 4 ligado no pino digital 4
#define  botao_reinicio A4		  //Botão para reiniciar o jogo

#define  piezo    8             //Transdutor ligado no pino digital 8

#define pino_a 9 // pinos para acender os leds do display
#define pino_b A0
#define pino_c A1
#define pino_d A2
#define pino_e A3
#define pino_f 3
#define pino_g 2


// --- Constantes Auxiliares ---
#define  nota_C4  262           //Frequência nota Dó  262Hz
#define  nota_E4  330           //Frequência nota Mí  330Hz
#define  nota_G4  392           //Frequência nota Sol 392Hz
#define  nota_A4  440           //Frequência nota lá 440Hz


// --- Protótipo das Funções Auxiliares ---
void output1();   				  //função para ativar led1
void output2();					    //função para ativar led2
void output3();					    //função para ativar led3	
void output4();					    //função para ativar led4
void memoryClear();				  //função para limpar o vetor memory
void start();					      //função para iniciar o jogo
void gameOver();				    //função para indicar derrota do usuário e reiniciar o jogo
void complete();				    //função para indicar que o nível atual foi completado
void check(byte number);    //função para verificar se o botão pressionado é o correto
void contadorLevel(int);		//função para contar o número de níveis


// --- Variáveis Globais ---
byte memory[15];                  //vetor que determina as posições de memória
byte count = 0x00;                //variável para contagem dos botões pressionados
int level = 1;                    //variável para indicar os níveis 
boolean espera = 0; 			        //variável para aguardar o pressionar de um botão


// --- Configurações Iniciais ---
void setup()
{  
    
    pinMode(led1, OUTPUT);            //LED1 é saída digital
    pinMode(led2, OUTPUT);            //LED2 é saída digital
    pinMode(led3, OUTPUT);            //LED3 é saída digital
  	pinMode(led4, OUTPUT);            //LED4 é saída digital
  	pinMode(pino_a,OUTPUT);           //pino a display saída digital
 	  pinMode(pino_b,OUTPUT);           //pino b display saída digital
  	pinMode(pino_c,OUTPUT);           //pino c display saída digital
  	pinMode(pino_d,OUTPUT);           //pino d display saída digital  
  	pinMode(pino_e,OUTPUT);           //pino e display saída digital
  	pinMode(pino_f,OUTPUT);           //pino f display saída digital    
  	pinMode(pino_g,OUTPUT);           //pino g display saída digital
    pinMode(botao1, INPUT_PULLUP);    //Botão 1 é entrada com resistor de pull-up interno
    pinMode(botao2, INPUT_PULLUP);    //Botão 2 é entrada com resistor de pull-up interno
    pinMode(botao3, INPUT_PULLUP);    //Botão 3 é entrada com resistor de pull-up interno
  	pinMode(botao4, INPUT_PULLUP);    //Botão 4 é entrada com resistor de pull-up interno
  	pinMode(botao_reinicio, INPUT_PULLUP);
  
    pinMode(piezo, OUTPUT);           //Transdutor é saída digital
    
    digitalWrite(led1, LOW);          //Desliga LED1 
    digitalWrite(led2, LOW);          //Desliga LED2
    digitalWrite(led3, LOW);          //Desliga LED3
  	digitalWrite(led4, LOW);          //Desliga LED4
    noTone(piezo);                    //Desliga piezo
  	contarLevel(level);
    
        
    memoryClear();                    //limpa memória
    
    
} //end setup


// --- Loop Infinito ---
void loop()
{
  contarLevel(level);
  start();                  //chama função de inicialização
	
  for(int i = 0; i < level; i++)
  {
    byte rand = random(1, 5); // gera valores randômicos entre 1 e 3
    memory[i] = rand;         // vetor de memória recebe um número aleatório
            
            
    switch(rand)                  //Switch para demonstrar qual botão deve ser pressionado
    {
       case 1: output1(); break;  //caso num igual a 1, pisca led1
       case 2: output2(); break;  //caso num igual a 2, pisca led2
       case 3: output3(); break;  //caso num igual a 3, pisca led3
       case 4: output4(); break;  //caso num igual a 4, pisca led4
       
    } //end switch
    
    
    espera = 1;                     //seta variável wait  
    
  } //end for
  
  while(espera){                    //aguarda até que um botão seja pressionado
  
  		
        if(!digitalRead(botao1))     //botão 1 pressionado?
        {                            //sim...
        
            output1();               //aciona LED1
            
            check(0x01);             //verifica se foi pressionado corretamente
                    
        } //end if B1
        
        if(!digitalRead(botao2))     //botão 2 pressionado?
        {                            //sim...
        
            output2();               //aciona LED2
            
            check(0x02);             //verifica se foi pressionado corretamente
                          
        } //end if B2
        
        if(!digitalRead(botao3))     //botão 3 pressionado?
        {                            //sim...
        
            output3();               //aciona LED3
            
            check(0x03);             //verifica se foi pressionado corretamente
                      
        } //end if B3
    
    	if(!digitalRead(botao4))       //botão 4 pressionado?
        {                            //sim...
        
            output4();               //aciona LED4
            
            check(0x04);             //verifica se foi pressionado corretamente
                      
        } //end if B4
    	if(!analogRead(botao_reinicio))
        {
          gameOver();
        }
        
        
  } //end while
  
  
} //end loop

 
void output1()   //aciona LED1 com nota Dó
{
   digitalWrite(led1, HIGH);
   tone(piezo, nota_C4);
   delay(300);
   digitalWrite(led1, LOW);
   noTone(piezo);
   delay(300);
   
} //end output1


void output2()   //aciona LED2 com nota Mí
{
   digitalWrite(led2, HIGH);
   tone(piezo, nota_E4);
   delay(300);
   digitalWrite(led2, LOW);
   noTone(piezo);
   delay(300);
   
} //end output2


void output3()  //aciona LED3 com nota Sol
{
   digitalWrite(led3, HIGH);
   tone(piezo, nota_G4);
   delay(300);
   digitalWrite(led3, LOW);
   noTone(piezo);
   delay(300);
   
} //end output3

void output4()  //aciona LED4 com nota La
{
   digitalWrite(led4, HIGH);
   tone(piezo, nota_A4);
   delay(300);
   digitalWrite(led4, LOW);
   noTone(piezo);
   delay(300);
   
} //end output4


void start()    //sinaliza o início de um novo nível
{
  for(int j=0; j<5; j++)  //repete 5 vezes
  {
    digitalWrite(led1,  HIGH); 
    digitalWrite(led2,  LOW);
    digitalWrite(led3,  LOW);
    digitalWrite(led4,  LOW);
    delay(100);
    digitalWrite(led1,  LOW);
    digitalWrite(led2,  HIGH);
    digitalWrite(led3,  LOW);
    digitalWrite(led4,  LOW);
    delay(100);
    digitalWrite(led1,  LOW); 
    digitalWrite(led2,  LOW);
    digitalWrite(led3,  HIGH);
    digitalWrite(led4,  LOW);
    delay(100);
    digitalWrite(led1,  LOW); 
    digitalWrite(led2,  LOW);
    digitalWrite(led3,  LOW);
    digitalWrite(led4,  HIGH);
    delay(100);

    
  } //end for
  
  digitalWrite(led4, LOW); //apaga led3 (último a ser acionado)
  
  delay(500); //aguarda 500ms
         
} //end start


void gameOver()    //função para reiniciar o jogo em caso de erro
{
  
   start();        //chama função de reinicialização
   level = 1;   //reinicia no nível 1
   espera  = 0;   //limpa wait
   count = 0;   //reinicia contador
   
} //end gameOver

 
void memoryClear()        //função para limpar memória
{
   for(int i = 0; i < 15; i++) memory[i] = 0; //todas as posições do vetor de memória recebem o valor zero
    
} //end memoryClear

 
void complete()             //função que indica conclusão do nível
{    
        if(level == count)  //level é igual a count?
        {                   //sim...
        
          espera  = 0;        //limpa wait para invalidar laço while
          count = 0;          //reinicia o contador
          memoryClear();      //limpa memória
          contarLevel(level); //função para contar o nível
          level++;            //avança para o próximo nível
          
        } 
        
} //end complete

void check(byte number)           //função para verificar se o botão pressionado é o correto
{
   if(memory[count] == number)    //verifica posição de memória
   {          
         count++;                 //Incrementa contador        
         complete();              //Chama função que indica nível completo
     	 contarLevel(level);
         
   } //end if
   
   else gameOver(); //se o botão for errado, reinicia


} //end check

void contarLevel(int n)
{
  if(n == 0)
  {
    digitalWrite(pino_a, HIGH);
    digitalWrite(pino_b, HIGH);
    digitalWrite(pino_c, HIGH);
    digitalWrite(pino_d, HIGH);
    digitalWrite(pino_e, HIGH);
    digitalWrite(pino_f, HIGH);
  }else if(n == 1)
  {
    digitalWrite(pino_a, LOW);
    digitalWrite(pino_b, HIGH);
    digitalWrite(pino_c, HIGH);
    digitalWrite(pino_d, LOW);
    digitalWrite(pino_e, LOW);
    digitalWrite(pino_f, LOW);
    digitalWrite(pino_g, LOW);
  }else if(n == 2)
  {
    digitalWrite(pino_a, HIGH);
    digitalWrite(pino_b, HIGH);
    digitalWrite(pino_c, LOW);
    digitalWrite(pino_d, HIGH);
    digitalWrite(pino_e, HIGH);
    digitalWrite(pino_f, LOW);
    digitalWrite(pino_g, HIGH);
  }else if(n == 3)
  {
    digitalWrite(pino_a, HIGH);
    digitalWrite(pino_b, HIGH);
    digitalWrite(pino_c, HIGH);
    digitalWrite(pino_d, HIGH);
    digitalWrite(pino_e, LOW);
    digitalWrite(pino_f, LOW);
    digitalWrite(pino_g, HIGH);
  }else if(n == 4)
  {
    digitalWrite(pino_a, LOW);
    digitalWrite(pino_b, HIGH);
    digitalWrite(pino_c, HIGH);
    digitalWrite(pino_d, LOW);
    digitalWrite(pino_e, LOW);
    digitalWrite(pino_f, HIGH);
    digitalWrite(pino_g, HIGH);
  }else if(n == 5)
  {
    digitalWrite(pino_a, HIGH);
    digitalWrite(pino_b, LOW);
    digitalWrite(pino_c, HIGH);
    digitalWrite(pino_d, HIGH);
    digitalWrite(pino_e, LOW);
    digitalWrite(pino_f, HIGH);
    digitalWrite(pino_g, HIGH);
  }else if(n == 6)
  {
    digitalWrite(pino_a, HIGH);
    digitalWrite(pino_b, LOW);
    digitalWrite(pino_c, HIGH);
    digitalWrite(pino_d, HIGH);
    digitalWrite(pino_e, HIGH);
    digitalWrite(pino_f, HIGH);
    digitalWrite(pino_g, HIGH);
  }else if(n == 7)
  {
    digitalWrite(pino_a, HIGH);
    digitalWrite(pino_b, HIGH);
    digitalWrite(pino_c, HIGH);
    digitalWrite(pino_d, LOW);
    digitalWrite(pino_e, LOW);
    digitalWrite(pino_f, LOW);
    digitalWrite(pino_g, LOW);
  }else if(n == 8)
  {
    digitalWrite(pino_a, HIGH);
    digitalWrite(pino_b, HIGH);
    digitalWrite(pino_c, HIGH);
    digitalWrite(pino_d, HIGH);
    digitalWrite(pino_e, HIGH);
    digitalWrite(pino_f, HIGH);
    digitalWrite(pino_g, HIGH);
  }else if(n == 9)
  {
    digitalWrite(pino_a, HIGH);
    digitalWrite(pino_b, HIGH);
    digitalWrite(pino_c, HIGH);
    digitalWrite(pino_d, LOW);
    digitalWrite(pino_e, LOW);
    digitalWrite(pino_f, HIGH);
    digitalWrite(pino_g, HIGH);
  }else if(n == 10)
  {
    digitalWrite(pino_a, HIGH);
    digitalWrite(pino_b, HIGH);
    digitalWrite(pino_c, HIGH);
    digitalWrite(pino_d, LOW);
    digitalWrite(pino_e, HIGH);
    digitalWrite(pino_f, HIGH);
    digitalWrite(pino_g, HIGH);
  }else if(n == 11)
  {
    digitalWrite(pino_a, LOW);
    digitalWrite(pino_b, LOW);
    digitalWrite(pino_c, HIGH);
    digitalWrite(pino_d, HIGH);
    digitalWrite(pino_e, HIGH);
    digitalWrite(pino_f, HIGH);
    digitalWrite(pino_g, HIGH);
  }else if(n == 12)
  {
    digitalWrite(pino_a, HIGH);
    digitalWrite(pino_b, LOW);
    digitalWrite(pino_c, LOW);
    digitalWrite(pino_d, HIGH);
    digitalWrite(pino_e, HIGH);
    digitalWrite(pino_f, HIGH);
    digitalWrite(pino_g, LOW);
  }else if(n == 13)
  {
    digitalWrite(pino_a, LOW);
    digitalWrite(pino_b, HIGH);
    digitalWrite(pino_c, HIGH);
    digitalWrite(pino_d, HIGH);
    digitalWrite(pino_e, HIGH);
    digitalWrite(pino_f, LOW);
    digitalWrite(pino_g, HIGH);
  }else if(n == 14)
  {
    digitalWrite(pino_a, HIGH);
    digitalWrite(pino_b, LOW);
    digitalWrite(pino_c, LOW);
    digitalWrite(pino_d, HIGH);
    digitalWrite(pino_e, HIGH);
    digitalWrite(pino_f, HIGH);
    digitalWrite(pino_g, HIGH);
  }else if(n == 15)
  {
    digitalWrite(pino_a, HIGH);
    digitalWrite(pino_b, LOW);
    digitalWrite(pino_c, LOW);
    digitalWrite(pino_d, LOW);
    digitalWrite(pino_e, HIGH);
    digitalWrite(pino_f, HIGH);
    digitalWrite(pino_g, HIGH);
  }else if(n > 15)
  {
    digitalWrite(pino_g, LOW);
    level = 1;
  }
} 
