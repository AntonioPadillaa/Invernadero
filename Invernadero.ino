  /*** LIBRERIAS ***/
#include <DHT.h>
#include <Wire.h>              // LIBRERIA PARA LA COMUNICACION DE COMPONENTE I2C
#include <LiquidCrystal_I2C.h> // LIBRERIA PARA COMPONENTE I2C
#include <SoftwareSerial.h>    // LIBRERIA PARA COMUNICACION SIM900 GPRS/GSM
SoftwareSerial SIM900(10, 11);   // CONFIGURACION DEL PUERTO SERIE PARA EL SIM900 GPRS/GSM

  /*** INICIALIZACION DE LCD's 20X4 Y 16X2 CONFIGURACION DE LAS PANTALLAS ***/
       
                        /*** EQUIVALENCIA MODULO I2C ***/
                  /***  addr,en,rw,rs,d4,d5,d6,d7,bl,blpol***/
LiquidCrystal_I2C lcd1(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C lcd2(0x26, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
LiquidCrystal_I2C lcd3(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  


  /*** DECLARACION DE VARIABLES ***/
/*** DETECTOR DE HUMO ***/
int sensorValueMQ=0;
int sensorHumo = A5; 
int extractor1 = 28;
int bombaIncendio = 40;
/*** TEMPERATURA ***/
int temp = 0;
int humedad = 0;
int sensorDHT = 52;
int enfriador1 = 26;
int calentador = 51;
/*** ILUMINACION ***/
int sensorValueLDR = 0;
int lampara =  53; 
int sensorLDR = A7;
/*** HUMEDAD DE SUELO ***/
int sensorValueYL = 0;
int sensorSuelo = A1;
int bombaRiego = 38;

  /*** ESPECIFICACION DEL TIPO DE SENSOR ***/
DHT dht(sensorDHT, DHT11);
  /*** INICIALIZACION DE LOS COMPONENTES QUE SE UTILIZARAN ***/
void setup() {
  SIM900.begin(19200); 
  lcd1.clear();
  lcd2.clear();
  lcd3.clear();
  lcd1.begin(20, 4);
  lcd2.begin(20, 4);
  lcd3.begin(16, 2);
  pinMode(enfriador1,OUTPUT);
  pinMode(calentador,OUTPUT);
  pinMode(extractor1, OUTPUT);
  pinMode(bombaIncendio, OUTPUT);
  pinMode(lampara, OUTPUT); 
  pinMode(bombaRiego, OUTPUT); 
}
/****** FUNCIONES HUMO ******/
void inicioHumo(){
  datoshumo();
  lcd2.setCursor(0, 0);
  lcd2.print("HUMO:");
  lcd2.setCursor(0, 1);
  lcd2.print("PPM CO2:");
  lcd2.setCursor(0, 2);
  lcd2.print("EXTRACTOR:");
  lcd2.setCursor(0, 3);
  lcd2.print("ALARMA:");
}
void datoshumo(){
  lcd2.clear();
  lcd2.setCursor(9, 1);
  lcd2.print(sensorValueMQ);
}
void detectado(){ 
  inicioHumo();
  digitalWrite(extractor1, HIGH);
  digitalWrite(bombaIncendio, HIGH);
  lcd2.setCursor(7, 0);
  lcd2.print("DETECTADO");
  lcd2.setCursor(10, 2);
  lcd2.print("ENCENDIDO");
  lcd2.setCursor(8, 3);
  lcd2.print(" ACTIVADA  ");
  sensorValueMQ = analogRead(sensorHumo); 
  mensaje_sms();
}
void extraccion(){ 
  inicioHumo();
  digitalWrite(extractor1, HIGH);
  lcd2.setCursor(7, 0);
  lcd2.print("DETECTADO");
  lcd2.setCursor(10, 2);
  lcd2.print("ENCENDIDO");
  lcd2.setCursor(8, 3);
  lcd2.print("DESACTIVADA");
  sensorValueMQ = analogRead(sensorHumo); 
}
void despejado(){
  inicioHumo();
  correccion();
  digitalWrite(extractor1, LOW);
  digitalWrite(bombaIncendio, LOW);
  lcd2.setCursor(7, 0);
  lcd2.print("DESPEJADO");
  lcd2.setCursor(10, 2);
  lcd2.print(" APAGADO ");
  lcd2.setCursor(8, 3);
  lcd2.print("DESACTIVADA");
  sensorValueMQ = analogRead(sensorHumo); 
}
void correccion(){
  inicioHumo();
  digitalWrite(extractor1, LOW);
  digitalWrite(bombaIncendio, LOW);
  lcd2.setCursor(12, 1);
  lcd2.print(" ");
  lcd2.setCursor(7, 0);
  lcd2.print("DESPEJADO");
  lcd2.setCursor(10, 2);
  lcd2.print(" APAGADO ");
  lcd2.setCursor(8, 3);
  lcd2.print("DESACTIVADA");
  sensorValueMQ = analogRead(sensorHumo); 
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
  temp= dht.readTemperature();
  humedad = dht.readHumidity();
  datoshumedad();
  datostemperatura();
  lcd1.setCursor(11, 2);
  lcd1.print("ENCENDIDO");

}
void enfriamientoOff(){
  digitalWrite(enfriador1,LOW);
  temp= dht.readTemperature();
  humedad = dht.readHumidity();
  datoshumedad();
  datostemperatura();
  lcd1.setCursor(11, 2);
  lcd1.print(" APAGADO ");
}
void incendio(){
  digitalWrite(enfriador1,LOW);
  temp= dht.readTemperature();
  humedad = dht.readHumidity();
  datoshumedad();
  datostemperatura();
  lcd1.setCursor(11, 2);
  lcd1.print(" APAGADO ");
}
void calefaccionOn(){
  digitalWrite(calentador,HIGH); //Encendemos el ventilador
  temp= dht.readTemperature(); //Volvemos a leer la temperatura
  humedad = dht.readHumidity();
  datoshumedad();
  datostemperatura();
  lcd1.setCursor(11, 3);
  lcd1.print("ENCENDIDO");
}
void calefaccionOff(){
  digitalWrite(calentador,LOW); //Encendemos el ventilador
   temp= dht.readTemperature(); //Volvemos a leer la temperatura
  humedad = dht.readHumidity();
  datoshumedad();
  datostemperatura();
  lcd1.setCursor(11, 3);
  lcd1.print(" APAGADO ");
}
/****** FUNCIONES LUZ ******/
void luzOn(){
  digitalWrite(lampara, HIGH);
}
void luzOff(){
  digitalWrite(lampara, LOW);
}
/****** FUNCIONES HUMEDAD SUELO ******/
void inicioSuelo(){
  lcd3.setCursor(0, 0);
  lcd3.print("Suelo: ");
  lcd3.setCursor(0, 1);
  lcd3.print("Riego:");
  sensorValueYL = analogRead(sensorSuelo);
}
void sueloSeco(){
  inicioSuelo();
  digitalWrite(bombaRiego,HIGH);
  lcd3.setCursor(8, 0);
  lcd3.print("Seco   ");
  lcd3.setCursor(7, 1);
  lcd3.print("Encendido");
  sensorValueYL = analogRead(sensorSuelo);
}
void sueloHumedo(){
  inicioSuelo();
  lcd3.setCursor(8, 0);
  lcd3.print("Humedo ");
  lcd3.setCursor(7, 1);
  lcd3.print("Encendido");
  sensorValueYL = analogRead(sensorSuelo);
}
void sueloMojado(){
  digitalWrite(bombaRiego,LOW);
  inicioSuelo();
  lcd3.setCursor(8, 0);
  lcd3.print("Mojado ");
  lcd3.setCursor(7, 1);
  lcd3.print("Apagado  ");
  sensorValueYL = analogRead(sensorSuelo);
}
/****** FUNCIONES ENVIO DE MENSAJE ******/
void mensaje_sms(){
      Serial.println("Enviando SMS...");
      SIM900.print("AT+CMGF=1\r");  //Configura el modo texto para enviar o recibir mensajes
      delay(1000);
      SIM900.println("AT+CMGS=\"3339500076\"");  //Numero al que vamos a enviar el mensaje
      delay(1000);
      //SIM900.println("SMS enviado desde un Arduino.");  // Texto del SMS
      SIM900.println("-ALERTA DE INCENDIO-\n");  // Texto del SMS
      SIM900.println("Ubicacion:");  // Texto del SMS
      SIM900.println("Calle Jose Guadalupe Zuno Hernandez #1964 Col. Americana, C.P. #44160 Guadalajara, Jal."); //Colocamos la url de google maps
      SIM900.println("https://goo.gl/maps/XufCJYDJKG32");  // Texto del 
      delay(100);
      SIM900.println((char)26); //Comando de finalización ^Z
      delay(100);
      SIM900.println();
      //delay(5000);  // Esperamos un tiempo para que envíe el SMS
}
void loop() {
/****** FORMATO DE DATOS DE INICIO DE CADA PANTALLA ******/ 
  inicioTemperatura();
  inicioHumo();
  inicioSuelo();
  
 /****** LECTURA ANALOGICA DE LOS SENSORES DE 
          TEMPERATURA Y HUMEDAD, HUMO, SUELO E
          ILUMINACION ******/
  temp = dht.readTemperature();
  humedad = dht.readHumidity();
  sensorValueMQ = analogRead(sensorHumo); 
  sensorValueLDR = analogRead (sensorLDR);
  sensorValueYL = analogRead(sensorSuelo);

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
    if(sensorValueMQ < 1500 && sensorValueMQ >= 400 && temp>=25)
  {
    incendio();
    detectado();
  }
  else if(sensorValueMQ < 1500 && sensorValueMQ >= 400)
  {
    extraccion();
  }
  else if (sensorValueMQ < 400 && sensorValueMQ >= 100)
  {
    despejado();
  }
  else if (sensorValueMQ < 100 )
  {
    correccion();
  }
  /****** FUNCIONAMIENTO LUZ AUTOMATICA  ******/
   if (sensorValueLDR <= 800) 
  {   
    luzOff();
  }
  else if (sensorValueLDR >= 800) 
  {
    luzOn();
  }
  /****** FUNCIONAMIENTO SENSOR DE SUELO  ******/
  if(sensorValueYL >= 800){
   sueloSeco();
  }
   else if(sensorValueYL <= 370){
   sueloMojado(); 
  }
  delay(1000);
}
