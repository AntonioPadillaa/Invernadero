  /*** LIBRERIAS ***/
#include <DHT.h>
#include <Wire.h>              // LIBRERIA PARA LA COMUNICACION DE COMPONENTE I2C
#include <LiquidCrystal_I2C.h> // LIBRERIA PARA COMPONENTE I2C

  /*** INICIALIZACION DE LCD's 20X4 Y 16X2 CONFIGURACION DE LAS PANTALLAS ***/
       
                        /*** EQUIVALENCIA MODULO I2C ***/
                  /***  addr,en,rw,rs,d4,d5,d6,d7,bl,blpol***/
LiquidCrystal_I2C lcd1(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C lcd2(0x26, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

  /*** DECLARACION DE VARIABLES ***/
  
/*** DETECTOR DE HUMO ***/
int sensorValue=0;
int extractor1 = 6;
int bombaIncendio = 10;
/*** TEMPERATURA ***/
int temp, humedad;
int sensorDHT = 2;
int enfriador1 = 3;
int calentador = 5;
/*** ILUMINACION ***/
int sensorValueLDR = 0;
int lampara =  11; 
int sensorLDR = A7;
  /*** ESPECIFICACION DEL TIPO DE SENSOR ***/
DHT dht(sensorDHT, DHT11);
  /*** INICIALIZACION DE LOS COMPONENTES QUE SE UTILIZARAN ***/
void setup() {
  lcd1.clear();
  lcd2.clear();
  lcd1.begin(20, 4);
  lcd2.begin(20, 4);
  pinMode(enfriador1,OUTPUT);
  pinMode(calentador,OUTPUT);
  pinMode(extractor1, OUTPUT);
  pinMode(bombaIncendio, OUTPUT);
  pinMode(lampara, OUTPUT); 
}
/****** FUNCIONES HUMO ******/
void inicioHumo(){
  datoshumo();
  lcd2.setCursor(0, 0);
  lcd2.print("HUMO:");
  lcd2.setCursor(0, 1);
  lcd2.print("PPM CO:");
  lcd2.setCursor(0, 2);
  lcd2.print("EXTRACTOR:");
  lcd2.setCursor(0, 3);
  lcd2.print("ALARMA:");
}
void datoshumo(){
  lcd2.clear();
  lcd2.setCursor(8, 1);
  lcd2.print(sensorValue);
}
void detectado(){
  inicioHumo();
  digitalWrite(extractor1, HIGH);
  lcd2.setCursor(7, 0);
  lcd2.print("DETECTADO");
  lcd2.setCursor(10, 2);
  lcd2.print("ENCENDIDO");
  lcd2.setCursor(8, 3);
  lcd2.print(" ACTIVADA  ");
  sensorValue = analogRead(A5); 
}
void despejado(){
  inicioHumo();
  correccion();
  digitalWrite(extractor1, LOW);
  lcd2.setCursor(7, 0);
  lcd2.print("DESPEJADO");
  lcd2.setCursor(10, 2);
  lcd2.print(" APAGADO ");
  lcd2.setCursor(8, 3);
  lcd2.print("DESACTIVADA");
  sensorValue = analogRead(A5); 
}
void correccion(){
  inicioHumo();
  digitalWrite(extractor1, LOW);
  lcd2.setCursor(11, 1);
  lcd2.print(" ");
  lcd2.setCursor(7, 0);
  lcd2.print("DESPEJADO");
  lcd2.setCursor(10, 2);
  lcd2.print(" APAGADO ");
  lcd2.setCursor(8, 3);
  lcd2.print("DESACTIVADA");
  sensorValue = analogRead(A5); 
}
/****** FUNCIONES TEMPERATURA ******/
void inicioTemperatura(){
  lcd1.setCursor(0, 0);
  lcd1.print("Temperatura:");
  lcd1.setCursor(15, 0);
  lcd1.print("C");
  lcd1.setCursor(0, 1);
  lcd1.print("Humedad:");
  lcd1.setCursor(11, 1);
  lcd1.print("%  ");
  lcd1.setCursor(0, 2);
  lcd1.print("Enfriador:");
  lcd1.setCursor(0, 3);
  lcd1.print("Calentador:");
}
void datostemperatura(){
  lcd1.setCursor(13, 0);
  lcd1.print(temp);
}
void datoshumedad(){
  lcd1.setCursor(9, 1);
  lcd1.print(humedad);
}
void enfriamientoOn(){
  digitalWrite(enfriador1,HIGH);
  datoshumedad();
  datostemperatura();
  lcd1.setCursor(11, 2);
  lcd1.print("ENCENDIDO");
  temp= dht.readTemperature();
  humedad = dht.readHumidity();
}
void enfriamientoOff(){
  digitalWrite(enfriador1,LOW);
  datoshumedad();
  datostemperatura();
  lcd1.setCursor(11, 2);
  lcd1.print(" APAGADO ");
  temp= dht.readTemperature();
  humedad = dht.readHumidity();
}
void calefaccionOn(){
  digitalWrite(calentador,HIGH); //Encendemos el ventilador
  datoshumedad();
  datostemperatura();
  lcd1.setCursor(11, 3);
  lcd1.print("ENCENDIDO");
  temp= dht.readTemperature(); //Volvemos a leer la temperatura
  humedad = dht.readHumidity();
}
void calefaccionOff(){
  digitalWrite(calentador,LOW); //Encendemos el ventilador
  datoshumedad();
  datostemperatura();
  lcd1.setCursor(11, 3);
  lcd1.print(" APAGADO ");
  temp= dht.readTemperature(); //Volvemos a leer la temperatura
  humedad = dht.readHumidity();
}
/****** FUNCIONES LUZ ******/
void luzOn(){
  digitalWrite(lampara, HIGH);
}
void luzOff(){
  digitalWrite(lampara, LOW);
}
void loop() {
/****** FORMATO DE DATOS DE INICIO DE CADA PANTALLA ******/ 
  inicioTemperatura();
  inicioHumo();
  
 /****** LECTURA ANALOGICA DE LOS SENSORES DE 
          TEMPERATURA Y HUMEDAD, HUMO, SUELO E
          ILUMINACION ******/
  temp = dht.readTemperature();
  humedad = dht.readHumidity();
  sensorValue = analogRead(A5); 
  sensorValueLDR = analogRead (sensorLDR);

  /****** FUNCIONAMIENTO TEMPERATURA ******/
  if (temp>=25)
  { 
    enfriamientoOn();
   }
  else if (temp<=24)
  { 
    enfriamientoOff();
   }
  if (temp<=15)
  { 
    calefaccionOn();
   }
  else if (temp>=16)
  { 
    calefaccionOff();
   }
   /****** FUNCIONAMIENTO DETECTOR DE HUMO ******/
  if(sensorValue < 1500 && sensorValue >= 400)
  {
    detectado();
  }
  else if (sensorValue < 400 && sensorValue >= 100)
  {
    despejado();
  }
  else if (sensorValue < 100 )
  {
    correccion();
  }
  /****** FUNCIONAMIENTO LUZ AUTOMATICA  ******/
   if (sensorValueLDR <= 800) 
  {   
    //luzOn();
    luzOff();
  }
  else if (sensorValueLDR > 800) 
  {
    //luzOff();
    luzOn();
  }
  delay(1000);
}
