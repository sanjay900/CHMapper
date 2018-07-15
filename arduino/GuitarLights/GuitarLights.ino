void setup() {
  Serial.begin(115200);
  for (int i =0; i < 13; i++) {
    pinMode(i, OUTPUT); 
    digitalWrite(i,LOW); 
  }
}

void loop() {
  if (Serial.available() >= 2) {
    int light = Serial.read();
    int state = Serial.read();
    digitalWrite(light,state);
  }
//  Serial.write(2);
//  Serial.write(digitalRead(2));
//  Serial.write(3);
//  Serial.write(digitalRead(3));
//  Serial.write(4);
//  Serial.write(analogRead(0)>20);
//  Serial.write(5);
//  Serial.write(analogRead(1)>20);
  
}
