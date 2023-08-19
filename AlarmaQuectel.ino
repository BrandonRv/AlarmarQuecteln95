#define ledRecepcion 13 //led de testeo de mensaje
#define PIRSENSOR 2

#include <Wire.h> 
#include "RTClib.h"
RTC_DS1307 RTC;
#include <SoftwareSerial.h>
SoftwareSerial mySerial(9,10);

int hora=0; // Variable donde se guardara la hora
int minutos=0; // Variable donde se guardaran los minutos
int segundos=0; // Variable donde se guardaran los segundos

int push1=0; //almacena el estado del botón

char numero_cell[]="3127944354";//numero de telefono
char msg;
char call;

int led =8;
boolean valor=true;
int i=0;
int j=0;
char DAT;
char DAT_dos;
char DAT_tres;
char DAT_cuatro;
char DAT_cinco;
char DAT_seis;

boolean condicion=true;
char datosSERIAL[10];
//char clave_uno[]= "LEDON"; //OTRA FORMA
char clave_uno[]={'P','I','R','O','N'};//CODIGO MENSAJE DE CONTROL
char clave_dos[]={'P','I','R','O','F'};//CODIGO MENSAJE DE CONTROL

const int ledSirena110 = 5; 

void setup(){

pinMode(2, INPUT); 
pinMode(ledSirena110, OUTPUT); 
digitalWrite(ledSirena110, HIGH);
mySerial.begin(9600); 
Serial.begin(9600);

Wire.begin();
RTC.begin();
//RTC.adjust(DateTime(__DATE__, __TIME__)); //NOTA: PARA EVITAR EL REINICIO DE LA HORA QUITAR ESTA INSTRUCCION LUEGO DE QUEMAR EL RELOJ, LUEGO VOLVER A SUBIR SIN ESTA INSTRUCCION Y ASI NO REINICIA MAS

delay(20000);

mySerial.println("AT+IPR=9600");
Serial.println("AT+IPR=9600");
delay(300);
mySerial.println("AT+CMGF=1");//modo texto
Serial.println("AT+CMGF=1");
delay(300);
mySerial.println("AT+CMGR=?");//ACTIVAMOS CODIGO PARA RECIBIR MENSAJES
Serial.println("AT+CMGR=?");
delay(300);
mySerial.println("AT+CNMI=2,2,0,0");//ACTIVAMOS PARA VER MENSAJES
Serial.println("AT+CNMI=2,2,0,0");
//Serial1.println("AT+CNMI=3,3,0,0");
//Serial.println("AT+CNMI=3,3,0,0");//ACTIVAMOS PARA VER MENSAJES
delay(300);
Serial.println("configuracion terminada");
delay(300);
Serial.println("sistema de alarma encendida by 3127944354");
delay(1000);

}

void loop(){
  
  DateTime now = RTC.now();
  hora=(now.hour(),DEC);
  minutos=(now.minute(),DEC);
  segundos=(now.second(),DEC);
    //Imprimir el dia
    Serial.print(now.day(), DEC);
    Serial.print('/');
    //Imprimir el mes
    Serial.print(now.month(), DEC);
    Serial.print('/');
    //Imprimir el año
    Serial.print(now.year(), DEC);
    Serial.print(' ');
    //Imprimir la hora
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    //Imprimir los minutos
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    //Imprimir los segundos
    Serial.print(now.second(), DEC);
    Serial.println();

Serial.println("esperando recibir mensaje...");

while(true)
{leer_mensaje();}
}

void leer_mensaje()
{
  salir:
    
    if (mySerial.available()>0)
    {     
      char DAT =mySerial.read();
      if(DAT=='@')//el arroba hace detectar el inicio del codigo
      {
      digitalWrite(ledRecepcion,HIGH);//led de testeo de mensaje
      while(true)//Esto es solo para leer los codigos futuros que estan por llegar despues de la arroba
      {
           if (mySerial.available()>0)
           {
               char DAT_dos =mySerial.read();//@PIRON codigo y enter
               datosSERIAL[j]= DAT_dos;//almacena los caracteres en cadenas,     
               j++;//Variable incrementativa osea se va sumando los caracteres
               if (DAT_dos=='\n')//Termina de entregar todos los datos y de enter que es \n       
               {
                 for(int i=0;i<=j;i++)
                 {}
                    PIR_on();//Funcion1                 
                    delay(500);
                    digitalWrite(ledRecepcion,LOW);                  
                    for(int i=0;i<=j;i++)
                    {
                      datosSERIAL[i]==0;//borro array
                      DAT_dos=0;
                      DAT=0;
                     }
                  j=0;//borra el acumulador de codigos si no se hace esto no detecta los sigueintes codigos
                  goto salir;
               }     
           }
      }
      }    
    }
}  

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////--------FUNCIONES -- DEPURADAS--------/////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
void PIR_on()
{ 
   if(datosSERIAL[0]==clave_uno[0] && datosSERIAL[1]==clave_uno[1] && datosSERIAL[2]==clave_uno[2] && datosSERIAL[3]==clave_uno[3] && datosSERIAL[4]==clave_uno[4])
   {
    digitalWrite(led,HIGH);
    mensaje_encendido();//Funcion4
    sensor_movimiento();//Funcion3
   }
}

void sensor_movimiento()
{
  digitalWrite(ledRecepcion,LOW);
  push1 = digitalRead(PIRSENSOR); //lee si el botón está pulsado
  DateTime now = RTC.now();
  hora=(now.hour(),DEC);
  minutos=(now.minute(),DEC);
  segundos=(now.second(),DEC);
  delay(10000);
      
    while (hora >= 0 && minutos >= 0)
   {       
     if (mySerial.available()>0)
    {     
      char DAT =mySerial.read();
      char DAT_dos =mySerial.read();
      char DAT_tres =mySerial.read();
      char DAT_cuatro =mySerial.read();
      char DAT_cinco =mySerial.read();
      char DAT_seis =mySerial.read();
      
      if(DAT=='@' && DAT_dos=='P' && DAT_tres=='I' && DAT_cuatro=='R' && DAT_cinco=='O' && DAT_seis=='F')//el arroba hace detectar el inicio del codigo
      {
       digitalWrite(ledRecepcion,HIGH);//led de testeo de mensaje
       delay(1000);
       digitalWrite(ledRecepcion,LOW);
       mensaje_apagado();//Funcion5
       digitalWrite(led,LOW);
       break;                        
      }                
    }
    
    DateTime now = RTC.now();
    hora=(now.hour(),DEC);
    minutos=(now.minute(),DEC);
    segundos=(now.second(),DEC);
    
     if (now.hour()== 7 && now.minute()== 00)
    {    
    digitalWrite(led,LOW);
    mensaje_apagado();//Funcion5
    break;
    }

    delay(50);
    push1 = digitalRead(2); //lee si el botón está pulsado
    Serial.println(push1);
    
    if(push1==LOW){ //si el boton es pulsado y antes no lo estaba
    digitalWrite(ledSirena110, LOW); 
    CALL_SMSMovimiento();//Funcion6 
    delay(10000);
    digitalWrite(ledSirena110, HIGH);
    digitalWrite(led,LOW);
    delay(500);    break;   
    }   
   }
}

void PIR_off()
{
   if(datosSERIAL[0]==clave_dos[0] && datosSERIAL[1]==clave_dos[1] && datosSERIAL[2]==clave_dos[2] && datosSERIAL[3]==clave_dos[3] && datosSERIAL[4]==clave_dos[4])
   {
    digitalWrite(led,LOW);
    mensaje_apagado();//Funcion5
   }
}

void CALL_SMSMovimiento()
{
     mySerial.print("ATD");//Llamada
     mySerial.print("317274XXXX"); //DONDE ESTA EL: ''317274XXXX'' ES DONDE VA A LLEGAR LA LLAMADA DE QUE SE  ACTIVO EL SENSOR DE MOVIMIENTO
     mySerial.println(";");
     delay(25000);
     mySerial.println("ATH");
     delay(2000);

     mySerial.print("ATD");//Llamada
     mySerial.print("318560XXXX"); //DONDE ESTA EL: ''318560XXXX'' ES DONDE VA A LLEGAR LA LLAMADA DE QUE SE  ACTIVO EL SENSOR DE MOVIMIENTO
     mySerial.println(";");
     delay(25000);
     mySerial.println("ATH");
     delay(1000);
//----------------------------------------------------------------------------------------------------------------------------------------------------------------//  
      mySerial.println("AT+CMGF=1");//Mesaje de Texto
      delay(1000); 
      mySerial.println("AT+CMGS=\"317274XXXX\"\r"); // DONDE ESTA EL: ''317274XXXX'' ES DONDE VA A LLEGAR A EL MESAJE DE TEXTO LUEGO QUE SE ACTIVO EL SENSOR DE MOVIMIENTO
      delay(1000);
      mySerial.println("ALARMA ACTIVADA!!!");
      delay(100);
      mySerial.println((char)26);
      delay(1000);

      mySerial.println("AT+CMGF=1");//Mesaje de Texto
      delay(1000); 
      mySerial.println("AT+CMGS=\"318560XXXX\"\r"); // DONDE ESTA EL: ''318560XXXX'' ES DONDE VA A LLEGAR A EL MESAJE DE TEXTO LUEGO QUE SE ACTIVO EL SENSOR DE MOVIMIENTO
      delay(1000);
      mySerial.println("ALARMA ACTIVADA!!!");
      delay(100);
      mySerial.println((char)26);
      delay(1000);
}

void mensaje_encendido()
{
      mySerial.println("AT+CMGF=1");// Mensaje de Texto
      delay(1000);
      mySerial.println("AT+CMGS=\"317274XXXX\"\r"); // DONDE ESTA EL: ''317274XXXX'' ES DONDE VA A LLEGAR A EL MESAJE DE TEXTO DONDE INDICA QUE LA ALARMA ESTA ACTIVA
      delay(1000);//tiempo para la respuesta del modulo >
      mySerial.print("SENSOR ENCENDIDO");//SMS
      delay(100);
      mySerial.print((char)26);//Simbolo ASCII 26,Corresponde a CTRL+Z,con esto se termina el msj
      delay(1000);

      mySerial.println("AT+CMGF=1");// Mensaje de Texto
      delay(1000);
      mySerial.println("AT+CMGS=\"318560XXXX\"\r"); // DONDE ESTA EL: ''318560XXXX'' ES DONDE VA A LLEGAR A EL MESAJE DE TEXTO DONDE INDICA QUE LA ALARMA ESTA ACTIVA
      delay(1000);//tiempo para la respuesta del modulo >
      mySerial.print("SENSOR ACTIVO");//SMS
      delay(100);
      mySerial.print((char)26);//Simbolo ASCII 26,Corresponde a CTRL+Z,con esto se termina el msj
      delay(1000);
}

void mensaje_apagado()
{
      mySerial.println("AT+CMGF=1");//modo texto 
      delay(1000);
      mySerial.println("AT+CMGS=\"317274XXXX\"\r"); // DONDE ESTA EL: ''317274XXXX'' ES DONDE VA A LLEGAR A EL MESAJE DE TEXTO DONDE INDICA QUE LA ALARMA ESTA DESACTIVADA
      mySerial.print("SENSOR APAGADO");//Mensaje
      delay(100);
      mySerial.print((char)26);
      delay(1000);
      
      mySerial.println("AT+CMGF=1");//modo texto 
      delay(1000);
      mySerial.println("AT+CMGS=\"318560XXXX\"\r"); // DONDE ESTA EL: ''318560XXXX'' ES DONDE VA A LLEGAR A EL MESAJE DE TEXTO DONDE INDICA QUE LA ALARMA ESTA DESACTIVADA
      delay(1000);
      mySerial.print("SENSOR APAGADO");//Mensaje
      delay(100);
      mySerial.print((char)26);
      delay(1000);
} 

//FIN
