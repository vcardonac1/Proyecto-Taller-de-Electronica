// ------------------------ LIBRERIAS SENSORES ------------------------ //
#include <OneWire.h>                              // Libreria para ds18b20
#include <DallasTemperature.h>                    // Libreria para ds18b20
#include <DHT.h>                                  // Libreria para dht11

// --------------------- DEFINICIÓN DE LOS PINES ---------------------- //
#define pinDS18b20 2                              // Asignar pin digital 2 a ds18b20
#define DHTpin 7                                  // Asignar pin digital 7 a dht
#define DHTtype DHT11                             // Definir el tipo de dht

// ------------------------ VARIABLES GLOBALES ------------------------ //
OneWire ourWire(pinDS18b20);                      // Se establece el pin 2 para la entrada de los datos del sensor DS18b20
DallasTemperature temperatura(&ourWire);          // Se declara una variable u objeto para nuestro sensor
DHT dht(DHTpin, DHTtype);                         // Iniciar DHT

// -------------------- INICIALIZACIÓN DEL SISTEMA -------------------- //
void setup() {
  delay(1000);
  Serial.begin(9600);                             // Se define 9600baud para la escritura de los datos
  temperatura.begin();                            // Se inicia el sensor de temperatura
  dht.begin();                                    // Se inicia el sensor de temperatura y humedad
}
 
void loop() {
  medirTemperatura();                             // Función que mide la temperatura y la registra en el serial
  medirTemperaturaHumedad();                      // Función que mide la temperatura y humedad y la registra en el serial
  delay(100);                     
}

// ----------------- FUNCIÓN QUE MIDE LA TEMPERATURA ----------------- //
void medirTemperatura(){
  float temp;
  temperatura.requestTemperatures();              // Se envía el comando para leer la temperatura
  temp = temperatura.getTempCByIndex(0);          // Se obtiene la temperatura en ºC
  
  Serial.print("Temperatura= ");
  Serial.print(temp);
  Serial.println(" C");
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
 
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" C\t");
  Serial.print("Índice de calor: ");
  Serial.print(hic);
  Serial.print(" C\n");
}
