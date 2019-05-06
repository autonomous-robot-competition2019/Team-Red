void setup() {
  Serial.begin(9600);
  pinMode(53, INPUT);
}

void loop() {
  Serial.println(digitalRead(53));
}
