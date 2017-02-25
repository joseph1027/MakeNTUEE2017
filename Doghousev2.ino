#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <OneWire.h>
#include <hx711.h>

#define DOUT A1
#define SCK A0
Hx711 scale(A1,A0);
LiquidCrystal_I2C lcd(0x27,20,4);    //LCD object
OneWire  ds(9); 

const int buttonpin=3;//ntc overall_temp value sensor.
const int transistor=13;

int overall_temp;
int overall_weight;

void lcddisplay(double overall_temp,double weight)
{
   int temp_int;
   temp_int=(int)overall_temp;
   
   if(overall_temp<25)
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
    lcd.print(" Fine:)");
   }
   
}
float temp_sensor()
{
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  float celsius, fahrenheit;
  
  if ( !ds.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
  Serial.println();
 
  // the first ROM byte indicates which chip
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(500);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  Serial.print("  Data = ");
  Serial.print(present, HEX);
  Serial.print(" ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(" CRC=");
  Serial.print(OneWire::crc8(data, 8), HEX);
  Serial.println();

  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");

  return celsius;
}
float weight_sensor()
{
  //lcd.setCursor(0, 1);
  //lcd.print(scale.getGram(), 1);
  Serial.print("Measured weigh: ");
  Serial.print(scale.getGram());
  //lcd.print(" g");
  //lcd.print("       ");
  delay(500);
}
void setup()
{
  Serial.begin(9600); //serial IO speed
  pinMode(transistor,OUTPUT);

  //LCD required procedure
  //lcd.backlight();
  lcd.init();
  lcd.begin(20, 4);  
}

void loop() //system loop
{
  overall_temp =  temp_sensor();
  overall_weight = weight_sensor();
  Serial.println("123");
  
  if(overall_temp<25)
  {
    digitalWrite(transistor,HIGH);
  }
  else
  {
    digitalWrite(transistor,LOW);
  }
  
  lcddisplay(overall_temp,overall_weight);
}
