// C++ code
#include <Keypad.h>
#include <Adafruit_LiquidCrystal.h>
#include <Servo.h>

Servo doorServo;
Adafruit_LiquidCrystal lcd(0);

const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = { 2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8}; //connect to the column pinouts of the keypad
Keypad unlockPad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); //initialize an instance of class NewKeypad

int pass_len = 4;
String master_pass = "1234";
String curr_pass = "";
bool door_locked = true;
char input;

void setup(){
  doorServo.attach(9);
  ServoClose();
  lcd.begin(16,2);
  lcd.setBacklight(1);
  lcd.setCursor(0,0);
  lcd.print("Enter Password");
}

void loop(){
  input = unlockPad.getKey();
  if(input){
    if(input=='#')  unlockDoor();
    else if(input=='*') lockDoor();
    else if(curr_pass.length()<pass_len)  takeInput();
  }
}

void unlockDoor(){
  if(curr_pass.length()==pass_len){
    if(curr_pass==master_pass){
      ServoOpen();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("The Door is");
      lcd.setCursor(0,1);
      lcd.print("Open!");
      curr_pass="";
    }
    else{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Incorrect");
      lcd.setCursor(0,1);
      lcd.print("Password!");
      curr_pass="";

      delay(1000);

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter Password");
      lcd.setCursor(0,1);
      lcd.print(curr_pass);
    }
  }
  else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Enter the full");
    lcd.setCursor(0,1);
    lcd.print("Password!");

    delay(1000);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Enter Password");
    lcd.setCursor(0,1);
    lcd.print(curr_pass);
  }
}

void lockDoor(){

  ServoClose();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Door Locked");
  curr_pass="";

  delay(1000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter Password");
  lcd.setCursor(0,1);
  lcd.print(curr_pass);
}

void takeInput(){
  curr_pass+=input;
  lcd.setCursor(curr_pass.length()-1,1);
  lcd.print(input);
}

void ServoOpen(){
  if(door_locked){
    for (int pos = 90; pos >= 0; pos -= 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      doorServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    door_locked=false;
  }
}
void ServoClose(){
  if(!door_locked){
    for (int pos = 0; pos <= 90; pos += 1) { // goes from 180 degrees to 0 degrees
      doorServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    door_locked=true;
  }
}
