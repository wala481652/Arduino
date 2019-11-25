
void setup()
{
  pinMode(1, LOW);
  pinMode(13,LOW);
}

void loop() {
  if(digitalRead(1)==HIGH){
    digitalWrite(13,HIGH);
  }
  else{
    digitalWrite(13,LOW);
  }
}
