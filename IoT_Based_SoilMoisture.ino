/*
    Incident Control Air quality index monitoring system
    Email - priti.sharma@vit.edu.in
    Password - Sensor@2022
*/

/*
   ESP8266      Arduino
      RX  ----- D10(TX)
      TX  ----- D11(RX)
*/
#define SOIL A0
#define Relay 12
#define Buzzer 13

#include <LiquidCrystal.h>
const int rs = 8, en = 7, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#include <SoftwareSerial.h>
SoftwareSerial mySerial(11, 10);    //Arduino (Rx, Tx)

String network = "Aditya";                     // access point SSID
String password = "Aditya9372";                    // wifi Access Point password
#define IP "184.106.153.149"                       // IP address of thingspeak.com
String GET = "GET /update?api_key=XUBR8OGQA5969200";   // replace with your channel key (key from thingspeakwhich write)

int soil_in, soil_inP, wps;
String ps;
int flag;

void setup()
{
  Serial.begin(9600);
  delay(100);
  mySerial.begin(115200);
  delay(100);
  lcd.begin(16, 2);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, LOW);
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, HIGH);

  mySerial.println("AT");
  delay(1000);
  mySerial.println("AT+RST");
  delay(1000);
  mySerial.println("AT+CWMODE=1");
  delay(1000);
  mySerial.println("AT+CWQAP");
  delay(1000);
  mySerial.print("AT+CWJAP=\"");
  mySerial.print(network);
  mySerial.print("\",\"");
  mySerial.print(password);
  mySerial.print("\"\r");
  delay(1000);
  if (mySerial.available())
  {
    String a = mySerial.readString();
    delay(10);
    Serial.println(a);
  }

  lcd.setCursor(0, 0);
  lcd.print(" IoT Based Soil ");
  lcd.setCursor(0, 1);
  lcd.print("MoistureDetector");
  delay(5000);
  lcd.setCursor(0, 0);
  lcd.print(" Project by...  ");
  lcd.setCursor(0, 1);
  lcd.print("  Priti Sharma  ");
  delay(3000);
  lcd.setCursor(0, 0);
  lcd.print("  Om  Devlekar  ");
  lcd.setCursor(0, 1);
  lcd.print(" Sunita  Bamble ");
  delay(5000);
}

void Upload()
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  mySerial.println(cmd);
  delay(5000);
  if (mySerial.find("Error")) {
    return;
  }
  cmd = GET + "&field1=" + soil_inP + "&field2=" + wps + "\r\n";
  mySerial.print("AT+CIPSEND=");
  mySerial.println(cmd.length());
  delay(15000);
  if (mySerial.find(">"))
  {
    mySerial.print(cmd);
    Serial.println("data sent");
    BuZ();
    lcd.setCursor(0, 0);
    lcd.print("   Data  Sent   ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    delay(2000);
  }
  else
  {
    mySerial.println("AT+CIPCLOSE");
    delay(5000);
    Serial.println("channel close");
  }
}

void BuZ()
{
  digitalWrite(Buzzer, HIGH);
  delay(200);
  digitalWrite(Buzzer,LOW);
  delay(600);
  digitalWrite(Buzzer, HIGH);
  delay(200);
  digitalWrite(Buzzer,LOW);
}

void loop()
{
  lcd.setCursor(0, 0);
  lcd.print("Soil   WaterPump");
  lcd.setCursor(0, 1);
  lcd.print("                ");


  soil_in = analogRead(SOIL);
  soil_inP = map(soil_in, 300, 1023, 100, 0);

  if (soil_inP < 40)
  {
    BuZ();
    digitalWrite(Relay, LOW);
    wps = 1;
    ps = "ON";
  }
  else
  {
    BuZ();
    digitalWrite(Relay, HIGH);
    wps = 0;
    ps = "OFF";
  }
      
  lcd.setCursor(0, 1);
  lcd.print(soil_inP); lcd.print("%"); lcd.print("       ");
  lcd.print(ps);lcd.print(" ");

  Serial.print(soil_in);
  Serial.print(" : ");
  Serial.print(soil_inP);
  Serial.print(" : ");
  Serial.print(wps);
  Serial.print(" : ");
  Serial.println(ps);
  delay(5000);
  
  lcd.setCursor(0, 0);
  lcd.print(" Data Sending...");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  
  Upload();
  
  lcd.setCursor(0, 1);
  lcd.print("  Data Sent ...");
  delay(2000);
}
