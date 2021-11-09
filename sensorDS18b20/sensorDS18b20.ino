// ------------------------ LIBRERIAS SENSORES ------------------------ //
#include <OneWire.h>                              // Libreria para ds18b20
#include <DallasTemperature.h>                    // Libreria para ds18b20

// --------------------- DEFINICIÓN DE LOS PINES ---------------------- //
#define pinDS18b20 2;                             // Asignar pin digital 2 a ds18b20

// ------------------------ VARIABLES GLOBALES ------------------------ //
OneWire ourWire(pinDS18b20);                      // Se establece el pin 2 para la entrada de los datos del sensor DS18b20
DallasTemperature temperatura(&ourWire);          // Se declara una variable u objeto para nuestro sensor

// -------------------- INICIALIZACIÓN DEL SISTEMA -------------------- //
void setup() {
  delay(1000);
  Serial.begin(9600);                             // Se define 9600baud para la escritura de los datos
  temperatura.begin();                            // Se inicia el sensor de temperatura
}
 
void loop() {
  medirTemperatura();                             // Función que mide la temperatura y la registra en el serial
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
