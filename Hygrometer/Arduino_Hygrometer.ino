const int hygrometer = A0;
int value;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  value = analogRead(hygrometer);
  value = constrain(value, 400, 1023);    //Keep the ranges!
  value = map(value, 400, 1023, 100, 0);  //Map value : 400 will be 100 and 1023 will be 0
  Serial.print("Soil humidity : ");
  Serial.println(value);;
  delay(2000);
}
