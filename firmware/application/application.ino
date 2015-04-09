#define LED 3
// the setup routine runs once when you press reset:
void setup()
{                
//  Serial.begin(9600);
  pinMode(LED, OUTPUT);
}

void loop() {
  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(5000);               // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(5000);               // wait for a secondIGH is the voltage level)
//  Serial.println("hello there");
}
