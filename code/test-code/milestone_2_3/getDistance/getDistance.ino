// Sharp IR GP2Y0A41SK0F Distance Test
// http://tinkcore.com/sharp-ir-gp2y0a41-skf/

#define sensor A0 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)

void setup() {
  Serial.begin(9600); // start the serial port
}

void loop() {
  get_distance();
  delay(10);
}

int get_distance() {
  // Read values from IR sensor
  float volts = analogRead(sensor)*0.0048828125;  // value from sensor * (5/1024)
  int distance = 13*pow(volts, -1); // worked out from datasheet graph
  if (distance <= 80 && distance != 0) {
    Serial.print("Distance: "); Serial.print(distance); Serial.println();
    return distance;
  } else {
    return -1;
  }
}
