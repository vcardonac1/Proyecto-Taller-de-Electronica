const int greenStatus = A3;                     // Asiganr el pin A3 para encender el led verde

void setup() {
  // put your setup code here, to run once:
  digitalWrite(greenStatus, HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:
  pinMode(greenStatus, OUTPUT);                 // Se inicia el LED verde

}
