#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
SoftwareSerial sim800l(0, 1); // RX,TX for Arduino and for the module it's TXD RXD, they should be inverted

#define button1 7 //Button pin, on the other pin it's wired with GND

bool button_State; //Button state

LiquidCrystal lcd(13, 12, 11, 10, 9, 8); //initialise the library with the numbers of the interface pins 

#define NOTE_C4  262
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  262

int resval = 0;     // holds the value
int SensorPin = A0; // sensor pin used
int RedLed = 2 , YellowLed = 3 , WhiteLed = 4 , BlueLed = 5;

int melody[] = { NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4 }; // notes in the melody

int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 }; // note durations: 4 = quarter note, 8 = eighth note, etc.

void setup() {
  pinMode(button1, INPUT_PULLUP); //The button is always on HIGH level, when pressed it goes LOW
  sim800l.begin(9600);   //Module baude rate, this is on max, it depends on the version
  Serial.begin(9600);   
  delay(1000);
  pinMode(RedLed,OUTPUT);
  pinMode(YellowLed,OUTPUT);
  Serial.begin(9600);
  lcd.begin(20, 4); // set up the LCD's number of columns and rows
  lcd.setCursor(1,0); // Print a message to the LCD.
  lcd.print("WATER LEVEL :");
  lcd.setCursor(0,2); 
  lcd.print("Analog Value: ");
  lcd.setCursor(0,3);
  lcd.print("Voltage: ");
  for (int thisNote = 0; thisNote < 8; thisNote++) { // iterate over the notes of the melody:
    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote]; 
    tone(7, melody[thisNote], noteDuration);
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(7);
  }  
}
void loop() {

  int SensorValue = analogRead(SensorPin);   
  float SensorVolts = analogRead(SensorPin)*0.0048828125;  
  
  lcd.setCursor(14, 2); 
  lcd.print(SensorValue);
  lcd.setCursor(9, 3);  
  lcd.print(SensorVolts);     
  lcd.print(" V");
  delay(1000);
  
  lcd.setCursor(0, 1); // set the cursor to column 0, line 1 
  resval = analogRead(SensorPin);    //Read data from analog pin and store it to resval variable
  
  if (resval<=100)
  { 
    lcd.println("     Empty    ");
    digitalWrite(RedLed,LOW);
    digitalWrite(YellowLed,HIGH);
    digitalWrite(WhiteLed,LOW);
    digitalWrite(BlueLed,LOW);
    delay(50);
  } 
  else if (resval>100 && resval<=300)
  {
    lcd.println("       Low      ");
    digitalWrite(RedLed,LOW);
    digitalWrite(YellowLed,LOW);
    digitalWrite(WhiteLed,LOW);
    digitalWrite(BlueLed,HIGH);
    delay(50);
  }
  else if (resval>300 && resval<=330)
  {  
    lcd.println("     Medium     ");
    digitalWrite(RedLed,LOW);
    digitalWrite(YellowLed,LOW);
    digitalWrite(WhiteLed,HIGH);
    digitalWrite(BlueLed,LOW);
    delay(50);
    } 
  else if (resval>330)
  { 
    lcd.println("      High      "); 
    digitalWrite(RedLed,HIGH);
    digitalWrite(YellowLed,LOW);
    digitalWrite(WhiteLed,LOW);
    digitalWrite(BlueLed,LOW);
    delay(50);
    tone(7, 50, 1000);
  }
  delay(500); 
  button_State = digitalRead(button1);   //We are constantly reading the button State
 
  if (button_State == LOW) {            //And if it's pressed
    Serial.println("Button pressed");   //Shows this message on the serial monitor
    delay(200);                         //Small delay to avoid detecting the button press many times
    
    SendSMS();                          //And this function is called

 }
 
  if (sim800l.available()){            //Displays on the serial monitor if there's a communication from the module
    Serial.write(sim800l.read()); 
  }
}
void SendSMS()
{
  Serial.println("Sending SMS...");               //Show this message on serial monitor
  sim800l.print("AT+CMGF=1\r");                   //Set the module to SMS mode
  delay(100);
  sim800l.print("AT+CMGS=\"+919080659745\"\r");  //Your phone number don't forget to include your country code, example +212123456789"
  delay(500);
  sim800l.print("SIM800l is working");       //This is the text to send to the phone number, don't make it too long or you have to modify the SoftwareSerial buffer
  delay(500);
  sim800l.print((char)26);// (required according to the datasheet)
  delay(500);
  sim800l.println();
  Serial.println("Text Sent.");
  delay(500);

}
