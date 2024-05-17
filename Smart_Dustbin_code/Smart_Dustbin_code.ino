#include<Stepper.h>
#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Servo.h>
Servo myservo; 
SoftwareSerial mySerial(3, 2);

int f1,f2,perate,val,L,pos=0;
int ir=4,ir2=7;
int motor=5;
int index=12;
int duration,distance,per;
float perlevel;
int trig=6;
int echo=5;
Stepper stepper(30,8,9,10,11);
LiquidCrystal_I2C lcd(0x27,20,4);

void setup() {
  // put your setup code here, to run once:
  pinMode(ir,INPUT);
  pinMode(ir2,INPUT);
  myservo.attach(12);
  Serial.begin(9600);
  stepper.setSpeed(120);
  pinMode(index,INPUT_PULLUP);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  lcd.init();
  lcd.backlight();

  // step 34-44 are for sending SMS. if not needed command those lines
  Serial.println("Initializing..."); 
  delay(1000);
  mySerial.println("AT");
  updateSerial();
  mySerial.println("AT+CMGF=1"); 
  updateSerial();
  mySerial.println("AT+CMGS=\"+919074116989\""); // enter your phone number here (prefix country code)
  updateSerial();
  mySerial.print("Hello from Superb Tech"); // enter your message here
  updateSerial();
  mySerial.write(26);
}

void flap()
{
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
  delay(100);
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }
}

int fillLevel()
{
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  duration=pulseIn(echo,HIGH);
  distance= (duration/29)/2;
  Serial.print(distance);
  perlevel=100-((distance/36.00) *100);
  return perlevel;
}

void loop() {
  lcd.clear();
  lcd.setCursor(2,1);
  lcd.print("Hi Guys you can ");
  lcd.setCursor(2,2);
  lcd.print("use me");
  L=analogRead(A2);
  // Serial.print(L);
  val=analogRead(A0);
  f1=digitalRead(ir);
  f2=digitalRead(ir2);
  perate=100-((val/1023.00)*100);
  
  if((f1==0||f2==0) && perate>10)
  {
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Wet Waste Detect");
    per = fillLevel();
    lcd.setCursor(3,3);
    lcd.print(String("Fill level = ") + String(per) + String("%"));
    stepper.step(133);
    flap();
    stepper.step(-133);
    Serial.print("Wet Waste");
    Serial.print("\n");
    
   }
  else if(f1==0 && L==0)
  {
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Metal Waste Detect");
    per = fillLevel();
    lcd.setCursor(3,3);
    lcd.print(String("Fill level = ") + String(per) + String("%"));
    flap();
    Serial.print("Metal waste");
    Serial.print("\n");
  }
  else if(f1==0||f2==0)
  {
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Recyclable Waste ");
    per = fillLevel();
    lcd.setCursor(3,3);
    lcd.print(String("Fill level = ") + String(per) + String("%"));
    stepper.step(-133);
    flap();
    stepper.step(133);
    Serial.print("Plastic and other waste");
    Serial.print("\n");
  }
  delay(1000);
}
