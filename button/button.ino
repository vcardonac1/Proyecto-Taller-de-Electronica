
const int buttonPin = A5;     // the number of the pushbutton pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  Serial.begin(9600); 
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    // turn LED on:
Serial.print("HIGH");
  } else {
    // turn LED off:
Serial.print("LOW");
  }
  delay(500);
}
