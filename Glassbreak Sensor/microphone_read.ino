
const int soundPin = 35;
int sound = 0;
void setup() {
  Serial.begin(9600);
  delay(1000);
}
void loop() {
  sound = analogRead(soundPin);
  Serial.println(sound);
  Serial.write(13);
  Serial.write(10);
  delay(3);
}
