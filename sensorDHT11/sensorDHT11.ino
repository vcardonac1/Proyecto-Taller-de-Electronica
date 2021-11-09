// --------------- LIBRERIAS SENSOR DHT11 --------------- //
#include <DHT.h>                

// --------------------- DEFINICIÓN DE LOS PINES ---------------------- //
#define DHTpin 7
#define DHTtype DHT11
DHT dht(DHTpin, DHTtype);

void setup() {
  Serial.begin(9600); // Inicializamos comunicación serie
  dht.begin(); // Comenzamos el sensor DHT
}
 
void loop() {
  delay(5000); // Esperamos 5 segundos entre medidas
  float h = dht.readHumidity(); // Leemos la humedad relativa
  float t = dht.readTemperature(); // Leemos la temperatura en grados centígrados (por defecto)
 
  if (isnan(h) || isnan(t)) { // Comprobamos si ha habido algún error en la lectura
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }
  
  float hic = dht.computeHeatIndex(t, h, false); // Calcular el índice de calor en grados centígrados
 
  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" *C\t");
  Serial.print("Índice de calor: ");
  Serial.print(hic);
  Serial.print(" *C"); 
}
