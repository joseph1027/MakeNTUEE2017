int ppin=3;
int npin=4;

void setup() {
  // put your setup code here, to run once:
  pinMode(ppin, OUTPUT);  
  pinMode(npin, OUTPUT); 
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(ppin, HIGH);
  digitalWrite(npin, LOW);
  delay(200);
  digitalWrite(npin, HIGH);
  digitalWrite(ppin, LOW);
  delay(200);
    digitalWrite(ppin, LOW);
  digitalWrite(npin, HIGH);
  delay(200);
  digitalWrite(ppin, HIGH);
  digitalWrite(npin, LOW);
  delay(200);
}
