#include <LiquidCrystal.h>

#include <SoftwareSerial.h>
SoftwareSerial sim800l(0, 1);
LiquidCrystal lcd(13, 12, 11, 10, 9, 8); //initialise the library with the numbers of the interface pins 

#define NOTE_C4  262
#define NOTE_G3  196
#define NOTE_A3  220
#define NOTE_B3  247
#define NOTE_C4  262

int resval = 0;     // holds the value
int SensorPin = A0; // sensor pin used


int melody[] = { NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4 }; // notes in the melody

int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 }; // note durations: 4 = quarter note, 8 = eighth note, etc.

void setup() {
  sim800l.begin(9600);   //Module baude rate, this is on max, it depends on the version
  Serial.begin(9600);   
  delay(1000);
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
    delay(50);
  } 
  else if (resval>100 && resval<=400)
  {
    lcd.println("       Low      ");
    delay(50);
  }
  else if (resval>400 && resval<=700)
  {  
    lcd.println("     Medium     ");
    delay(50);
    } 
  else if (resval>700)
  { 
    lcd.println("     High     ");
    if (sim800l.available()){        
    Serial.write(sim800l.read()); 
    }
    SendSMS();
    delay(50);
    tone(7, 50, 1000);
  }
  delay(500); 
}


void SendSMS()
{
  Serial.println("Sending SMS...");               //Show this message on serial monitor
  sim800l.print("Email: abc@gmail.com");                   //Set the module to SMS mode
  delay(100);
  Serial.println("Email: phambachtm2@gmail.com");
  sim800l.print("AT+CMGS=\"01342567890\"\r");  //Your phone number don't forget to include your country code, example +212123456789"
  delay(500);
  Serial.println("0942560157");
  sim800l.print("SIM800l is working");       //This is the text to send to the phone number, don't make it too long or you have to modify the SoftwareSerial buffer
  delay(500);
  sim800l.print((char)26);// (required according to the datasheet)
  delay(500);
  sim800l.println();
  Serial.println("Text Sent Successfull.");
  delay(500);
}
 
