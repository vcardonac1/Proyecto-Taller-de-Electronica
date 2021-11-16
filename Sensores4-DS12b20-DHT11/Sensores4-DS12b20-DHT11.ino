// ---------------------------- CONEXIONES ---------------------------- //
/*
 *      DS18b20 (1) -> pin digital 2
 *      DS18b20 (2) -> pin digital 3
 *      DS18b20 (3) -> pin digital 4
 *      DS18b20 (4) -> pin digital 5
 *      DHT11       -> pin digital 6
 */

// ------------------------ LIBRERIAS SENSORES ------------------------ //
#include <OneWire.h>                              // Libreria para ds18b20
#include <DallasTemperature.h>                    // Libreria para ds18b20
#include <DHT.h>                                  // Libreria para dht11
#include <LiquidCrystal.h>                        // Libreria para LCD

// --------------------- DEFINICIÓN DE LOS PINES ---------------------- //
#define pinDS18b20_s1 2                           // Asignar pin digital 2 a ds18b20 sensor 1
#define pinDS18b20_s2 3                           // Asignar pin digital 3 a ds18b20 sensor 2
#define pinDS18b20_s3 4                           // Asignar pin digital 4 a ds18b20 sensor 3
#define pinDS18b20_s4 5                           // Asignar pin digital 5 a ds18b20 sensor 4

#define DHTpin 6                                  // Asignar pin digital 6 a dht
#define DHTtype DHT11                             // Definir el tipo de dht

#define pinRS 11                                  // Asignar pin 11 al RS del LCD
#define pinEN 12                                  // Asignar pin 12 al EN del LCD
#define pind4 7                                   // Asignar pin 7 al D4 del LCD
#define pind5 8                                   // Asignar pin 8 al D5 del LCD
#define pind6 9                                   // Asignar pin 9 al D6 del LCD
#define pind7 10                                  // Asignar pin 10 al D7 del LCD

// ------------------------ VARIABLES GLOBALES ------------------------ //
OneWire ourWire_s1(pinDS18b20_s1);                // Se establece el pin para la entrada de los datos del sensor 1 DS18b20
DallasTemperature temperatura_s1(&ourWire_s1);    // Se declara una variable u objeto para nuestro sensor 1

OneWire ourWire_s2(pinDS18b20_s2);                // Se establece el pin para la entrada de los datos del sensor 2 DS18b20
DallasTemperature temperatura_s2(&ourWire_s2);    // Se declara una variable u objeto para nuestro sensor 2

OneWire ourWire_s3(pinDS18b20_s3);                // Se establece el pin para la entrada de los datos del sensor 3 DS18b20
DallasTemperature temperatura_s3(&ourWire_s3);    // Se declara una variable u objeto para nuestro sensor 3

OneWire ourWire_s4(pinDS18b20_s4);                // Se establece el pin para la entrada de los datos del sensor 4 DS18b20
DallasTemperature temperatura_s4(&ourWire_s4);    // Se declara una variable u objeto para nuestro sensor 4

DHT dht(DHTpin, DHTtype);                         // Iniciar DHT

LiquidCrystal lcd(pinRS, pinEN, pind4, pind5, pind6, pind7); // Iniciar LCD 

// -------------------- INICIALIZACIÓN DEL SISTEMA -------------------- //
void setup() {
  delay(1000);
  Serial.begin(9600);                             // Se define 9600baud para la escritura de los datos
  temperatura_s1.begin();                         // Se inicia el sensor 1 de temperatura
  temperatura_s2.begin();                         // Se inicia el sensor 2 de temperatura
  temperatura_s3.begin();                         // Se inicia el sensor 3 de temperatura
  temperatura_s4.begin();                         // Se inicia el sensor 4 de temperatura
  dht.begin();                                    // Se inicia el sensor de temperatura y humedad
  
  lcd.begin(16, 2);                               // Inicializar el LCD con el número de  columnas y filas del LCD
}
 
void loop() {
  medirTemperatura();                             // Función que mide la temperatura y la registra en el serial
  medirTemperaturaHumedad();                      // Función que mide la temperatura y humedad y la registra en el serial
  delay(800);                     
}

// ----------------- FUNCIÓN QUE MIDE LA TEMPERATURA ----------------- //
void medirTemperatura(){
  float temp1;
  float temp2;
  float temp3;
  float temp4;
  float nivel_1;                                  // Se calcula el promedio entre los sensores 1 y 3
  float nivel_2;                                  // Se calcula el promedio entre los sensores 2 y 4
  
  temperatura_s1.requestTemperatures();           // Se envía el comando para leer la temperatura del sensor 1
  temp1 = temperatura_s1.getTempCByIndex(0) - 2.5;// Se obtiene la temperatura en ºC

  temperatura_s2.requestTemperatures();           // Se envía el comando para leer la temperatura del sensor 2
  temp2 = temperatura_s2.getTempCByIndex(0) - 2.5;// Se obtiene la temperatura en ºC

  temperatura_s3.requestTemperatures();           // Se envía el comando para leer la temperatura del sensor 3
  temp3 = temperatura_s3.getTempCByIndex(0) - 2;// Se obtiene la temperatura en ºC

  temperatura_s4.requestTemperatures();           // Se envía el comando para leer la temperatura del sensor 4
  temp4 = temperatura_s4.getTempCByIndex(0) - 2.5;// Se obtiene la temperatura en ºC
  
  printInLCDcacao(temp1,temp2,temp3, temp4); 
  
  Serial.print("S1: ");
  Serial.print(temp1);
  Serial.print(" C\t");

  Serial.print("S2: ");
  Serial.print(temp2);
  Serial.print(" C\t");

  Serial.print("S3: ");
  Serial.print(temp3);
  Serial.print(" C\t");

  Serial.print("S4: ");
  Serial.print(temp4);
  Serial.print(" C\n");
}

// ------------ FUNCIÓN QUE MIDE LA TEMPERATURA Y HUMEDAD ------------ //
void medirTemperaturaHumedad(){
  float h = dht.readHumidity();                   // Se lee la humedad relativa
  float t = dht.readTemperature();                // Se lee la temperatura en grados centígrados (por defecto)
 
  if (isnan(h) || isnan(t)) {                     // Se comprueba si ha habido algún error en la lectura
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }
  float hic = dht.computeHeatIndex(t, h, false);  // Calcular el índice de calor en grados centígrados

  //printInLCDambiente(h, t);
  
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" C\t");
  Serial.print("Índice de calor: ");
  Serial.print(hic);
  Serial.print(" C\n\n");
}

void printInLCDambiente(float h, float t){
  lcd.setCursor(0, 0);
  lcd.print("Humedad ");
  lcd.print(h);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Temperatura ");
  lcd.print(t);
  lcd.print("C");
}

void printInLCDcacao(float temp1,float temp2,float temp3,float temp4){
  lcd.setCursor(0, 0);
  lcd.print("1:");
  lcd.print(temp1);
  lcd.print(" 2:");
  lcd.print(temp2);
  
  lcd.setCursor(0, 1);
  lcd.print("3:");
  lcd.print(temp3);
  lcd.print(" 4:");
  lcd.print(temp4);
}
