// ---------------------------- CONEXIONES ---------------------------- //
/*
 *      DS18b20 (1) ------------------> pin digital 2
 *      DS18b20 (2) ------------------> pin digital 3
 *      DS18b20 (3) ------------------> pin digital 4
 *      DS18b20 (4) ------------------> pin digital 5
 *      
 *      DHT11       ------------------> pin digital 6
 *      
 *      LCD D4      ------------------> pin digital 7
 *      LCD D5      ------------------> pin digital 8
 *      LCD D6      ------------------> pin digital 9
 *      LCD D7      ------------------> pin digital 10
 *      LCD RS      ------------------> pin digital 11
 *      LCD EN      ------------------> pin digital 12
 *      
 *      Button confirmar volteo ------> pin analogo A5
 *      Button visualizacion    ------> pin analogo A4
 *      
 *      LED verde   ------------------> pin analogo A3 
 *      LED rojo    ------------------> pin analogo A2
 */

// ------------------------ LIBRERIAS SENSORES ------------------------ //
#include <OneWire.h>                            // Libreria para ds18b20
#include <DallasTemperature.h>                  // Libreria para ds18b20
#include <DHT.h>                                // Libreria para dht11
#include <LiquidCrystal.h>                      // Libreria para LCD
#include <NoDelay.h>                            // Libreria para manejar el delay de las mediciones

// --------------------- DEFINICIÓN DE LOS PINES ---------------------- //
#define pinDS18b20_s1 2                         // Asignar pin digital 2 a ds18b20 sensor 1
#define pinDS18b20_s2 3                         // Asignar pin digital 3 a ds18b20 sensor 2
#define pinDS18b20_s3 4                         // Asignar pin digital 4 a ds18b20 sensor 3
#define pinDS18b20_s4 5                         // Asignar pin digital 5 a ds18b20 sensor 4

#define DHTpin 6                                // Asignar pin digital 6 a dht
#define DHTtype DHT11                           // Definir el tipo de dht

#define pinRS 11                                // Asignar pin 11 al RS del LCD
#define pinEN 12                                // Asignar pin 12 al EN del LCD
#define pind4 7                                 // Asignar pin 7 al D4 del LCD
#define pind5 8                                 // Asignar pin 8 al D5 del LCD
#define pind6 9                                 // Asignar pin 9 al D6 del LCD
#define pind7 10                                // Asignar pin 10 al D7 del LCD

const int buttonVolteo = A5;                    // Asiganr el pin A5 para la señal del boton que confirma el volteo
const int buttonVisual = A4;                    // Asiganr el pin A4 para la señal del boton que cambia la visualizacion
const int greenStatus = A3;                     // Asiganr el pin A3 para encender el led verde
const int redStatus = A2;                       // Asiganr el pin A3 para encender el led rojo

// ------------------------ VARIABLES GLOBALES ------------------------ //
OneWire ourWire_s1(pinDS18b20_s1);              // Se establece el pin para la entrada de los datos del sensor 1 DS18b20
DallasTemperature temperatura_s1(&ourWire_s1);  // Se declara una variable u objeto para nuestro sensor 1

OneWire ourWire_s2(pinDS18b20_s2);              // Se establece el pin para la entrada de los datos del sensor 2 DS18b20
DallasTemperature temperatura_s2(&ourWire_s2);  // Se declara una variable u objeto para nuestro sensor 2

OneWire ourWire_s3(pinDS18b20_s3);              // Se establece el pin para la entrada de los datos del sensor 3 DS18b20
DallasTemperature temperatura_s3(&ourWire_s3);  // Se declara una variable u objeto para nuestro sensor 3

OneWire ourWire_s4(pinDS18b20_s4);              // Se establece el pin para la entrada de los datos del sensor 4 DS18b20
DallasTemperature temperatura_s4(&ourWire_s4);  // Se declara una variable u objeto para nuestro sensor 4

DHT dht(DHTpin, DHTtype); // Iniciar DHT

LiquidCrystal lcd(pinRS, pinEN, pind4, pind5, pind6, pind7); // Iniciar LCD

/*
 * Para pruebas se tomara cada 10s = 10000
 * Ambiente real cada 30 minutos  = 1800000
*/
int tiempoEntreMediciones = 10000;              // Tiempo entre mediciones                                                    $$$$$$
int iter = 0;                                   // Tiempo entre mediciones
long startTime;                                 // Variable que registra el tiempo de inicio
long mediumTime;                                // Variable que registra el tiempo en medio de la ejecucion
long endTime;                                   // Variable que registra el tiempo de fin de una medicion
long duration;                                  // Variable que registra el tiempo en un intervalo

// ------------------- VARIABLES SENSORES TEMPERATURA ------------------ //
float temp1;                                    // Variable que registra la temperatura del sensor 1
float temp2;                                    // Variable que registra la temperatura del sensor 2
float temp3;                                    // Variable que registra la temperatura del sensor 3
float temp4;                                    // Variable que registra la temperatura del sensor 4
float meanTemps;                                // Variable que registra la temperatura promedio de los sensores 1 2 3 4
float maxTempLim = 45;                          // Temperatura limite superior                                                $$$$$$
float tiempoPrimerVolteo = 48;                  // Tiempo en horas para realizar el primer volteo                             $$$$$$
float tiempoVolteos = 24;                       // Tiempo en horas para realizar los demas volteos                            $$$$$$
int etapaVolteo = 0;                            // Determina la etapa de fermentacion

// -------------------- VARIABLES SENSOR DE HUMEDAD ------------------- //
float h;                                        // Variable que registra la humedad ambiente
float t;                                        // Variable que registra la temperatura ambiente
float hic;                                      // Variable que registra el indice de calor

// ------------------------- VARIABLES BOTONES ------------------------ //
int buttonState = 0;                            // Variable para leer el estado de un boton
int viewLCD = 0;                                // Variable para elegir que mostrar en el LCD
// -------------------- INICIALIZACIÓN DEL SISTEMA -------------------- //
void setup(){
  delay(1000);
  Serial.begin(9600);                           // Se define 9600baud para la escritura de los datos
  temperatura_s1.begin();                       // Se inicia el sensor 1 de temperatura
  temperatura_s2.begin();                       // Se inicia el sensor 2 de temperatura
  temperatura_s3.begin();                       // Se inicia el sensor 3 de temperatura
  temperatura_s4.begin();                       // Se inicia el sensor 4 de temperatura
  dht.begin();                                  // Se inicia el sensor de temperatura y humedad

  pinMode(buttonVolteo, INPUT);                 // Se inicia el boton de confirmacion de volteo
  pinMode(buttonVisual, INPUT);                 // Se inicia el boton de cambio de visualizacion

  pinMode(greenStatus, OUTPUT);                 // Se inicia el LED verde
  pinMode(redStatus, OUTPUT);                   // Se inicia el LED rojo

  lcd.begin(16, 2);                             // Inicializar el LCD con el número de  columnas y filas del LCD
  startTime = millis();                         // Almacenar el tiempo de inicio
  digitalWrite(greenStatus, HIGH);

  // display en LCD
  lcd.setCursor(0, 0);
  lcd.print("Bienvenido  ");
  lcd.print(0);
  lcd.setCursor(0, 1);
  lcd.print("Iniciando proceso");
}

void medirTemperatura();                        // Función que mide la temperatura y la registra en el serial
void verificarCondicionVolteo();                // Se validan las condiciones de volteo
void medirTemperaturaHumedad();                 // Función que mide la temperatura y humedad y la registra en el serial
void verificarSensores();                       // Se verifica si hay algun error en alguno de los sensores

noDelay medirTemperaturaTIME(tiempoEntreMediciones, medirTemperatura);
noDelay verificarCondicionVolteoTIME(tiempoEntreMediciones, verificarCondicionVolteo);
noDelay medirTemperaturaHumedadTIME(tiempoEntreMediciones, medirTemperaturaHumedad);
noDelay verificarSensoresTIME(tiempoEntreMediciones, verificarSensores);


void loop(){
  medirTemperaturaTIME.update();                           // Función que mide la temperatura y la registra en el serial
  verificarCondicionVolteoTIME.update();                   // Se validan las condiciones de volteo
  medirTemperaturaHumedadTIME.update();                    // Función que mide la temperatura y humedad y la registra en el serial
  verificarSensoresTIME.update();                          // Se verifica si hay algun error en alguno de los sensores

  buttonState = digitalRead(buttonVisual);
  if(buttonState == HIGH){
    changeViewLCD();
  }
  buttonState = digitalRead(buttonVolteo);
  if (buttonState == HIGH)  {
    displayEtapa();
  }
}

// ----------------- FUNCIÓN QUE MIDE LA TEMPERATURA ----------------- //
void medirTemperatura(){

  temperatura_s1.requestTemperatures();            // Se envía el comando para leer la temperatura del sensor 1
  temp1 = temperatura_s1.getTempCByIndex(0) - 2.5; // Se obtiene la temperatura en ºC

  temperatura_s2.requestTemperatures();            // Se envía el comando para leer la temperatura del sensor 2
  temp2 = temperatura_s2.getTempCByIndex(0) - 2.5; // Se obtiene la temperatura en ºC

  temperatura_s3.requestTemperatures();          // Se envía el comando para leer la temperatura del sensor 3
  temp3 = temperatura_s3.getTempCByIndex(0) - 2; // Se obtiene la temperatura en ºC

  temperatura_s4.requestTemperatures();            // Se envía el comando para leer la temperatura del sensor 4
  temp4 = temperatura_s4.getTempCByIndex(0) - 2.5; // Se obtiene la temperatura en ºC
}

// ------------ FUNCIÓN QUE MIDE LA TEMPERATURA Y HUMEDAD ------------ //
void medirTemperaturaHumedad(){

  h = dht.readHumidity();    // Se lee la humedad relativa
  t = dht.readTemperature(); // Se lee la temperatura en grados centígrados (por defecto)

  if (isnan(h) || isnan(t))
  { // Se comprueba si ha habido algún error en la lectura
    return;
  }
  hic = dht.computeHeatIndex(t, h, false); // Calcular el índice de calor en grados centígrados

}

void printInLCDambiente(){
  lcd.setCursor(0, 0);
  lcd.print("Humedad ");
  lcd.print(h);
  lcd.print("%    ");

  lcd.setCursor(0, 1);
  lcd.print("Temperatura ");
  lcd.print(t);
  lcd.print("C");
}

void printInLCDcacao(){
  lcd.setCursor(0, 0);
  lcd.print("1:");
  lcd.print(temp1, 1);
  lcd.print(" 2:");
  lcd.print(temp2, 1);

  lcd.setCursor(0, 1);
  lcd.print("3:");
  lcd.print(temp3, 1);
  lcd.print(" 4:");
  lcd.print(temp4, 1);
}

void verificarSensores(){
  if (isnan(h) || isnan(t) || temp1 < 0 || temp2 < 0 || temp3 < 00 || temp4 < 0)  {
    lcd.setCursor(0, 0);
    lcd.print("Error sensores: ");
    digitalWrite(greenStatus, LOW);
    digitalWrite(redStatus, HIGH);
  }

  lcd.setCursor(0, 1);
  if (isnan(h) || isnan(t))  { // Se comprueba si ha habido algún error en la lectura del DHT11
    lcd.print("Amb ");
  }
  if (temp1 < 0)  {
    lcd.print("S1 ");
  }
  if (temp2 < 0)  {
    lcd.print("S2 ");
  }
  if (temp3 < 0)  {
    lcd.print("S3 ");
  }
  if (temp4 < 0)  {
    lcd.print("S4 ");
  }
}

void verificarCondicionVolteo(){
  if (etapaVolteo == 0)  {
    // Calcular tiempo transcurrido
    endTime = millis(); // Almacenar el tiempo de fin
    duration = endTime - startTime;
    duration = duration / 3600000.0; // Tiempo en transcurrido en horas

    // Si han pasado 48 horas y la temperatura es mayor a 45C
    if ((temp1 > maxTempLim || temp2 > maxTempLim || temp3 > maxTempLim || temp4 > maxTempLim) &&
        (duration >= tiempoPrimerVolteo))
    {
      activarAlertas();
    }
  }
  if (etapaVolteo > 0)  {
    // Calcular tiempo transcurrido
    endTime = millis(); // Almacenar el tiempo de fin
    duration = endTime - mediumTime;
    duration = duration / 3600000.0; // Tiempo en transcurrido en horas

    // Si han pasado 24 horas y la temperatura se encuentra entre 45C +- 5C
    int delta = 5; // Temperatura de intervalo           $$$$$$
    if ((temp1 > maxTempLim - delta || temp2 > maxTempLim - delta || temp3 > maxTempLim - delta || temp4 > maxTempLim - delta) &&
        (duration >= tiempoVolteos))
    {
      activarAlertas();
    }
  }
}

void activarAlertas(){
  buttonState = digitalRead(buttonVolteo);
  while (buttonState == LOW)  {
    digitalWrite(greenStatus, LOW);
    digitalWrite(redStatus, HIGH);
    
    // ---------- activar alarma auditiva

    // display en LCD
    lcd.setCursor(0, 0);
    lcd.print("Fin Etapa ");
    lcd.print(etapaVolteo);
    lcd.setCursor(0, 1);
    lcd.print("Realizar volteo");

    buttonState = digitalRead(buttonVolteo); // Recibir señal del boton
  }

  digitalWrite(redStatus, LOW);
  digitalWrite(greenStatus, HIGH);
  etapaVolteo = etapaVolteo + 1; // Se actualiza la etapa de volteo
  // ---------- Establecer nuevos valores
  mediumTime = millis();
}

void changeViewLCD(){
  if (viewLCD == 0){
    printInLCDambiente();
    viewLCD = 1;
  } else{
    printInLCDcacao();
    viewLCD = 0;
  } 
}

void displayEtapa(){
  // display en LCD
  lcd.setCursor(0, 0);
  lcd.print("Se encuentra en");
  lcd.setCursor(0, 1);
  lcd.print("Etapa ");
  lcd.print(etapaVolteo);
}
