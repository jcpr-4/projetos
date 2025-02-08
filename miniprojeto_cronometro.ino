/*
Engenharia de Controle e Automação- IFPB

Cronômetro com 3 displays de 7 segmentos

Programa desenvolvido com intuito de desenvolver um cronômetro capaz
de contar até 999, ser pausado e retornar para a contagem ou reiniciar a contagem,
utilizando 3 displays 7 segmentos, 2 botões(um para iniciar e pausar e o outro para reiniciar),
resistores e transistores atuando como chaves para realizar a multiplexação dos 3 displays.

Autor: Julio Cesar Pereira Rodrigues  Data: Janeiro de 2025

Contatos: www.linkedin.com/in/julio-rodrigues-000a8b298
	  http://lattes.cnpq.br/6285832622159221

*/

/* ##### MAPEAMENTO DOS PINOS DO DISPLAY ##### */

#define PINO_A 2
#define PINO_B 3
#define PINO_C 6
#define PINO_D 7
#define PINO_E 8
#define PINO_F 5
#define PINO_G 4

//MAPEAMENTO DOS PINOS QUE ATUARÃO COMO CHAVES PARA ACIONAR O DEVIDO DISPLAY

int PINO_CHAVE_DISPLAY1=9; 	//PINO PARA ACIONAR O PRIMEIRO DISPLAY
int PINO_CHAVE_DISPLAY2=10;     //PINO PARA ACIONAR O SEGUNDO  DISPLAY
int PINO_CHAVE_DISPLAY3=11;     //PINO PARA ACIONAR O TERCEIRO DISPLAY

#define TAMANHO_PINOS_CHAVEAMENTO	3
int pinos_chaveamento[TAMANHO_PINOS_CHAVEAMENTO]= 
{PINO_CHAVE_DISPLAY1,PINO_CHAVE_DISPLAY2,PINO_CHAVE_DISPLAY3};

// DEFINIÇÃO DOS ESTADOS DO CRONÔMETRO 

#define ESTADO_INICIAL 					0
#define ESTADO_AGUARDAR_BOTAO_INICIO 	                1
#define ESTADO_CONTAGEM 				2
#define ESTADO_PAUSADO 					3 
#define ESTADO_FIM_DE_CONTAGEM 				4

// MAPEAMENTO DAS FUNÇÕES UTILIZADAS

void desligar(); 						//FUNÇÃO UTILIZADA APENAS PARA DESLIGAR O CRONÔMETRO 
void iniciar_displayZERADO();  			    		//FUNÇÃO UTILIZADA PARA INICIAR O CRONÔMETRO ZERADO
void incrementaContadores(int cont1, int cont2);                //FUNÇÃO UTILIZADA PARA INCREMENTAR O VALOR DOS CONTADORES
void imprimeNumerodeSegundos();				        //FUNÇÃO UTILIZADA PARA IMPRIMIR O NÚMERO DE SEGUNDOS NOS DISPLAYS
void pausado(); 			 		        //FUNÇÃO UTILIZADA PARA PAUSAR O CRONÔMETRO
void resetar(int botao_reset);				        //FUNÇÃO UTILIZADA PARA RESETAR O CRONÔMETRO QUANDO ESTIVER PAUSADO
void leituraBotaoInicio(const int botao); 	                //FUNÇÃO UTILIZADA PARA REALIZAR A LEITURA DO BOTÃO INÍCIO/PAUSA

// DEFINIÇÕES DE VARIÁVEIS UTILIZADAS

uint8_t estadoAtual; 	//VARIÁVEL UTILIZADA PARA ATUALIZAR O ESTADO DE FUNCIONAMENTO DO CRONÔMETRO

#define TAMANHO_VETOR_PINOS 7
int pinos_display[TAMANHO_VETOR_PINOS] = {PINO_A, PINO_B, PINO_C, PINO_D,
                                         PINO_E, PINO_F, PINO_G};

// variáveis utilizadas para fazer a multiplexação dos displays
int uni,dezen,cent; //uni:   variável para armazenar o valor da unidade.
		    //dezen: variável para armazenar o valor da dezena.
	            //cent:  variável para armazenar o valor da centena.
#define tempMULTIPLEXACAO 5 //TEMPO UTILIZADO PARA FAZER A MULTIPLEXAÇÃO DOS DISPLAYS

//VARIÁVEIS UTILIZADAS PARA O BOTÃO DE INÍCIO/PAUSA

const int BOTAO_INICIO=12; 	         //BOTÃO UTILIZADO PARA INICIAR CONTAGEM DO CRONÔMETRO
int estadoBotaoInicio;		         //VARIÁVEL UTILIZADA PARA ARMAZENAR O ESTADO DO BOTÃO DE INÍCIO
int cont_botaoI=0;			 //CONTADOR UTILIZADO PARA ATUALIZAR O ESTADO DO BOTÃO DE INÍCIO(INICIAR OU PAUSAR)
int ultimoEstadoBotao=LOW;               //VARIÁVEL UTILIZADA PARA ARMAZENAR
unsigned long ultimoTempDebounce=0;      //VARIÁVEL UTILIZADA PARA ARMAZENAR O ÚLTIMO TEMPO DE DEBOUNCE
unsigned long delayDebounce=30;          //VARIÁVEL UTILIZADA PARA DEFINIR UM TEMPO MÍNIMO DE DELAY PARA O ACABAR O EFEITO DEBOUNCE

#define TEMPO_DE_UM_SEGUNDO    1000      //TEMPO DE 1 SEGUNDO EM MILLISEGUNDOS

//VARIÁVEIS UTILIZADAS PARA O BOTÃO DE RESET

const int BOTAO_RESET=13; 	         //BOTÃO UTILIZADO PARA INICIAR CONTAGEM DO CRONÔMETRO
int estadoBotaoReset;		         //VARIÁVEL UTILIZADA PARA ARMAZENAR O ESTADO DO BOTÃO DE INÍCIO

int contador1=0; 		         //CONTADOR RESPONSÁVEL POR ATUALIZAR OS DOIS PRIMEIROS DÍGITOS
int cont1_aux=0;		         //VARIÁVEL UTILIZADA PARA ARMAZENAR O ÚLTIMO NÚMERO DO CONTADOR1
int contador2=0; 		         //CONTADOR RESPONSÁVEL POR ATUALIZAR O TERCEIRO DÍGITO
int cont2_aux=0;		         //VARIÁVEL UTILIZADA PARA ARMAZENAR O ÚLTIMO NÚMERO DO CONTADOR2

unsigned long tempsegundos=0;            //VARIÁVEL DE TEMPO RESPONSÁVEL PELA ATUALIZAÇÃO DOS SEGUNDOS

// MATRIZ PARA ARMAZENAR SEQUÊNCIA BINÁRIA NÚMERICA DE CADA NÚMERICO DECIMAL
#define LINHAS		10
#define COLUNAS 	7 

int numeros[LINHAS][COLUNAS]= { // 1, 2, 3, 4, 5,6, 7, 8, 9.
                                {1,1,1,1,1,1,0}, // NÚMERO 0
                                {0,1,1,0,0,0,0}, // NÚMERO 1 
                                {1,1,0,1,1,0,1}, // NÚMERO 2
                                {1,1,1,1,0,0,1}, // NÚMERO 3
                                {0,1,1,0,0,1,1}, // NÚMERO 4
                                {1,0,1,1,0,1,1}, // NÚMERO 5
                                {1,0,1,1,1,1,1}, // NÚMERO 6
                                {1,1,1,0,0,0,0}, // NÚMERO 7
                                {1,1,1,1,1,1,1}, // NÚMERO 8
                                {1,1,1,0,0,1,1}, // NÚMERO 9
                              }; 


void setup()
{
  Serial.begin(9600);
  
  //DEFININDO OS PINOS DE ACIONAMENTO COMO SAÍDAS DIGITAIS
  for(int i=0;i<TAMANHO_PINOS_CHAVEAMENTO;i++)
  {
    pinMode(pinos_chaveamento[i], OUTPUT);
  }
  
  //DEFININDO OS PINOS DOS DISPLAYS COMO SAÍDAS DIGITAIS
  for(int i=0;i<TAMANHO_VETOR_PINOS;i++)
  {
    pinMode(pinos_display[i], OUTPUT);
  }
  
  //DEFININDO OS BOTÕES COMO ENTRADAS DO SISTEMA
  pinMode(BOTAO_INICIO, INPUT_PULLUP);
  pinMode(BOTAO_RESET, INPUT_PULLUP);
  
} // fim do setup

void loop()
{
  // ESTRUTURA DE REPERTIÇÃO UTILIZADA PARA DEFINIR O ESTADO DO CRONÔMETRO
  switch(estadoAtual)
  {
    // PRIMEIRO ESTADO DO CRONÔMETRO: ESTADO INICIAL 
    case ESTADO_INICIAL:
    
    	iniciar_displayZERADO();
    	estadoAtual++;
    
    	break;
    
    //SEGUNDO ESTADO DO CRONÔMETRO: AGUARDAR O BOTÃO DE INÍCIO
    case ESTADO_AGUARDAR_BOTAO_INICIO:
    
    	estadoBotaoInicio=digitalRead(BOTAO_INICIO);
    	if(estadoBotaoInicio==HIGH)
        {
          desligar();
          estadoAtual++;
        }
    	break;
    
    // TERCEIRO ESTADO DO CRONÔMETRO: INÍCIO DA CONTAGEM DO CRONÔMETRO
    case ESTADO_CONTAGEM:
    	
    	leituraBotaoInicio(BOTAO_INICIO);
    	incrementaContadores(contador1,contador2);
        imprimeNumerodeSegundos();
    	break;
    
    case ESTADO_PAUSADO:
	    
    	pausado();
    	break;
    case ESTADO_FIM_DE_CONTAGEM:
	estadoAtual=ESTADO_INICIAL;
	break;
    
	}// final do laço de repetição switch
  	
}// fim do loop

//INÍCIO DA IMPLEMENTAÇÃO DAS FUNÇÕES UTILIZADAS NO CÓDIGO

void iniciar_displayZERADO()
{ 
  // LAÇO DE REPETIÇÃO QUE ATIVA TODOS OS PINOS DE CHAVEAMENTO 
  for(int i=0;i<TAMANHO_PINOS_CHAVEAMENTO;i++)
  {
    digitalWrite(pinos_chaveamento[i],HIGH);
  }
  
  //LAÇO DE REPETIÇÃO PARA ACIONAR OS 3 DISPLAYS COM O VALOR IGUAL A 0
  for(int i = 0; i < TAMANHO_VETOR_PINOS; i++)
  {
    digitalWrite(pinos_display[i], HIGH);
  }
  digitalWrite(PINO_G,LOW);
}

void desligar()
{ 
  // LAÇO DE REPETIÇÃO PARA DESLIGAR TODOS OS DISPLAYS
  for(int i = 0; i < TAMANHO_VETOR_PINOS; i++)
  {
    digitalWrite(pinos_chaveamento[i], LOW);
  }
}

void incrementaContadores(int cont1, int cont2)
{
  //cont1=contador_DIGITO1
  //cont2=contador_DIGITO2
  
  if(cont1==100)
  {
    contador2++;
    cont2_aux=contador2;
  }
  if(cont2>9){
    contador2=0;
    estadoAtual=ESTADO_FIM_DE_CONTAGEM;
  }
  if(cont1>99)
  {
    contador1=0;
  }else if((millis()-tempsegundos)>TEMPO_DE_UM_SEGUNDO)
  {
    contador1++;
    cont1_aux=contador1;
    tempsegundos=millis();
  }
}

void imprimeNumerodeSegundos()
{
  //BLOCO DE CÓDIGO QUE REALIZA A MULTIPLEXAÇÃO DOS DISPLAYS
  //E ATUALIZA O NÚMERO DE SEGUNDOS DOS DISPLAYS
  if((millis()-tempsegundos)<TEMPO_DE_UM_SEGUNDO)
  {
    cent=contador2%10; //ATRIBUI VALOR À VARIÁVEL CENT QUE REPRESENTA A CENTENA DO DISPLAY
    for(int c=0;c<COLUNAS;c++)
    {
      digitalWrite(pinos_display[c], numeros[cent][c]);
    }
	  
    //BLOCO DE CÓDIGO PARA FAZER A MULTIPLEXAÇÃO DO DÍGITO MAIS SIGNIFICATIVO DO DISPLAY
    digitalWrite(PINO_CHAVE_DISPLAY3,HIGH);
    delay(tempMULTIPLEXACAO);
    digitalWrite(PINO_CHAVE_DISPLAY3,LOW);
          
    dezen=contador1/10; //ATRIBUI VALOR À VARIÁVEL DEZ QUE REPRESENTA A DEZENA DO DISPLAY
    for(int c=0;c<COLUNAS;c++)
    {
      digitalWrite(pinos_display[c], numeros[dezen][c]);
    }
	  
    // BLOCO DE CÓDIGO PARA FAZER A MULTIPLEXAÇÃO DO DISPLAY DO DÍGITO DAS DEZENAS
    digitalWrite(PINO_CHAVE_DISPLAY2,HIGH);
    delay(tempMULTIPLEXACAO);
    digitalWrite(PINO_CHAVE_DISPLAY2,LOW);

    uni=contador1%10; //ATRIBUI VALOR À VARIÁVEL UNI QUE REPRESENTA A UNIDADE DO DISPLAY
    for(int c=0;c<COLUNAS;c++)
    {
      digitalWrite(pinos_display[c], numeros[uni][c]);
    }
	  
    //BLOCO DE CÓDIGO PARA FAZER A MULTIPLEXAÇÃO DO DÍGITO MENOS SIGNIFICATIVO DO DISPLAY
    digitalWrite(PINO_CHAVE_DISPLAY1,HIGH);
    delay(tempMULTIPLEXACAO);
    digitalWrite(PINO_CHAVE_DISPLAY1,LOW);
  }
}

//IMPLEMENTAÇÃO DA FUNÇÃO RESETAR()
void resetar(int botao_reset)
{
  estadoBotaoReset=digitalRead(botao_reset);
  if(estadoBotaoReset==HIGH)
  {
    contador1=0;
    contador2=0;
  }
}

//IMPLEMENTAÇÃO DA FUNÇÃO UTILIZADA PARA PAUSAR O CRONÔMETRO
void pausado()
{
  cent=cont2_aux%10; //ATRIBUI VALOR À VARIÁVEL CENT QUE REPRESENTA A CENTENA DO DISPLAY
  for(int c=0;c<COLUNAS;c++)
  {
    digitalWrite(pinos_display[c], numeros[cent][c]);
  }
  
  //BLOCO DE CÓDIGO PARA FAZER A MULTIPLEXAÇÃO DO DÍGITO MAIS SIGNIFICATIVO DO DISPLAY
  digitalWrite(PINO_CHAVE_DISPLAY3,HIGH);
  delay(tempMULTIPLEXACAO);
  digitalWrite(PINO_CHAVE_DISPLAY3,LOW);
          
  dezen=cont1_aux/10; //ATRIBUI VALOR À VARIÁVEL DEZ QUE REPRESENTA A DEZENA DO DISPLAY
  for(int c=0;c<COLUNAS;c++)
  {
    digitalWrite(pinos_display[c], numeros[dezen][c]);
  }
  
  // BLOCO DE CÓDIGO PARA FAZER A MULTIPLEXAÇÃO DO DISPLAY DO DÍGITO DAS DEZENAS
  digitalWrite(PINO_CHAVE_DISPLAY2,HIGH);
  delay(tempMULTIPLEXACAO);
  digitalWrite(PINO_CHAVE_DISPLAY2,LOW);

  uni=cont1_aux%10; //ATRIBUI VALOR À VARIÁVEL UNI QUE REPRESENTA A UNIDADE DO DISPLAY
  for(int c=0;c<COLUNAS;c++)
  {
    digitalWrite(pinos_display[c], numeros[uni][c]);
  }
  //BLOCO DE CÓDIGO PARA FAZER A MULTIPLEXAÇÃO DO DÍGITO MENOS SIGNIFICATIVO DO DISPLAY
  digitalWrite(PINO_CHAVE_DISPLAY1,HIGH);
  delay(tempMULTIPLEXACAO);
  digitalWrite(PINO_CHAVE_DISPLAY1,LOW);
  
  //BLOCO DE CÓDIGO UTILIZADO PARA DEFINIR SE A CONTAGEM SERÁ REINICIADA OU RETOMADA
   leituraBotaoInicio(BOTAO_INICIO);
   //SE 'BOTAO_RESET' ACIONADO A CONTAGEM É REINICIADA
   estadoBotaoReset=digitalRead(BOTAO_RESET);
   if(estadoBotaoReset==HIGH)
   {
     contador1=0;
     contador2=0;
     cont_botaoI=0;
     estadoAtual=ESTADO_INICIAL;
   }
} //FIM DA FUNÇÃO PAUSADO

//IMPLEMENTAÇÃO DA FUNÇÃO UTILIZADA PARA LER O ESTADO DO BOTÃO DE INÍCIO E PAUSA DO CRONÔMETRO
void leituraBotaoInicio(const int botao)
{
  //REALIZA A LEITURA DO ESTADO DO BOTÃO E ARMAZENA EM VARIÁVEL LOCAL
  int leitura=digitalRead(botao);
  
  //COMPARA O ULTIMO ESTADO ARMAZENADO DO BOTÃO COM A LEITURA DO BOTÃO
  if(leitura!=ultimoEstadoBotao)
  {
    ultimoTempDebounce=millis();
  }
  
  //VERIFICA A LEITURA DO BOTÃO E COMPARA COM O TEMPO MÍNIMO DE DELAY PARA EVITAR O DEBOUNCE
  if((millis()-ultimoTempDebounce)>delayDebounce)
  {
    if(leitura!=estadoBotaoInicio)
    {
      estadoBotaoInicio=leitura;
      if(estadoBotaoInicio==HIGH)
      {
        cont_botaoI++;
        
        //SE O CONTADOR DO BOTÃO DE INÍCIO/PAUSA FOI MENOR OU IGUAL A UM, ATUALIZA O ESTADO DO CRONÔMETRO
        if(cont_botaoI<=1)
        {
          estadoAtual++;
        }else if(cont_botaoI==2)
        {
          //SE O CONTADOR DO BOTÃO DE INÍCIO/PAUSA FOR IGUAL A 2
          //O CRONÔMETRO RETORNA PRO ESTADO DE CONTAGEM E ZERA O CONTADOR DO BOTÃO
          cont_botaoI=0;
          estadoAtual=ESTADO_CONTAGEM;
        }
      }
    }
  }
  ultimoEstadoBotao=leitura;
}
