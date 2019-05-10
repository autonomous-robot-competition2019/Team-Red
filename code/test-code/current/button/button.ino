void setup() {
  //start serial connection
  Serial.begin(9600);
  //configure pin 2 as an input and enable the internal pull-up resistor
  pinMode(41, INPUT_PULLUP);
}

void loop() {
  int sensorVal = digitalRead(41);
  
  if (sensorVal == HIGH) {
    Serial.println("Released");
  } else {
    Serial.println("Pressed");
  }
}
