#include <LiquidCrystal.h>
#include <Wire.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);    //LCD object 
const int Led=13;
const int buttonpin=3;
int ntc=A0; //ntc temp value sensor.
int val;
int transistor=8;
void lcddisplay(double temp,double weight)
{
   int temp_int;
   temp_int=(int)temp;
   
   if(temp<25)
   {
    lcd.setCursor(0,0);
    lcd.print("Temp:");
    lcd.print(temp_int);
    lcd.setCursor(9,0); //Col 12 Row 0
    lcd.print("Cold>_<");
   }
   else
   {
    lcd.setCursor(0,0);
    lcd.print("Temp:");
    lcd.print(temp_int);
    lcd.setCursor(9,0); //Col 12 Row 0
    lcd.print(" 
    
    Fine:)");
   }
   /*if(weight<100) //100 grams of feed, recharge
   {
    lcd.setCursor(0,1);
    lcd.print("Refill food!");
   }*/
   
}


void setup()
{
  Serial.begin(9600); //serial IO speed
  pinMode(Led,OUTPUT);
  pinMode(buttonpin,INPUT);
  //LCD required procedure

  lcd.begin(16, 2);  
}

double Thermistor(int RawADC) //thermal sensor for KY 038-like module
{
  double Temp;
  Temp = log(10000.0*((1024.0/RawADC-1))); 
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
  Temp = Temp - 273.15;            // Convert Kelvin to Celcius
   //Temp =(Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
   return 43-Temp;
}

void loop() //system loop
{
  val=analogRead(ntc); //get specified temp data by analog read at the specified analog port
  
  double temp =  Thermistor(val);
  delay(1000);
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
  lcddisplay(temp,temp);
}


