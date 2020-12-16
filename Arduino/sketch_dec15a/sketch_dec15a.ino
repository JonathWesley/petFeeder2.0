void setup() {
  Serial.begin(9600);
  pinMode(7, OUTPUT);
  pinMode(8, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(8) == HIGH){
    digitalWrite(7, HIGH);
    Serial.println("Motor ligado.");
    delay(10);
  }else{
    digitalWrite(7, LOW);
    Serial.println("Motor desligado.");
    delay(10);
  }
}
