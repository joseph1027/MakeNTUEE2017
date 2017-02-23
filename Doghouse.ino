#include <LiquidCrystal.h>
#include <Wire.h>
LiquidCrystal lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, HIGH); //LCD object 
const int Led=13;
const int buttonpin=3;
int ntc=A0;
int val;
int transistor=8;
void lcddisplay(double temp,double weight1)
{
   lcd.setCursor(0,0);
   lcd.print("Temp:");
   lcd.print(temp,DEC);
   if(temp<25)
   {
    lcd.print("Cold");
   }
   if(weight<100) //100 grams of feed, recharge
   {
    lcd.setCursor(0,1);
    lcd.print("Refill food!");
   }
   
}


void setup()
{
  Serial.begin(9600);
  pinMode(Led,OUTPUT);
  pinMode(buttonpin,INPUT);
  Serial.print("1111");

  lcd.begin(16, 2);  //1604
  lcd.setCursor(0,0);
}

double Thermistor(int RawADC) {
  double Temp;
  Temp = log(10000.0*((1024.0/RawADC-1))); 
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
  Temp = Temp - 273.15;            // Convert Kelvin to Celcius
   //Temp =(Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
   return 43-Temp;
}

void loop()
{
  val=analogRead(ntc);
  Serial.print(val);
  Serial.print('\n');
  /*if(val==HIGH)
  {
    digitalWrite(Led,HIGH);
  }
  else
  {
    digitalWrite(Led,LOW);
  }*/
  double temp =  Thermistor(val);
  Serial.print("Temperature big sensor is = ");
  Serial.print(temp);
  Serial.println(" Celsius, ");
  if(temp<25)
  {
    digitalWrite(transistor,HIGH);
  }
  else
  {
    digitalWrite(transistor,LOW);
  }
  lcddisplay(temp);
}


