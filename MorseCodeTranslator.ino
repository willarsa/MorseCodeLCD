/*
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/

// include the library code:
#include <LiquidCrystal.h>
#include <Arduino.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2, buttonPin = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

template <typename T1, typename T2>
struct tuple{
  T1 character;
  T2 sequence;
};

tuple<String, String> ones[2]= {{"e","."},{"t","_"}};
tuple<String, String> twos[4]= {{"n","_."},{"a","._"},{"i",".."},{"m","__"}};
tuple<String, String> threes[8]= {{"s","..."},{"o","___"},{"g","__."},{"u",".._"},{"w",".__"},{"d","_.."},{"r","._."},{"k","_._"}};
tuple<String, String> fours[12]= {{"h","...."},{"z","__.."},{"v","..._"},{"t","__._"},{"j",".___"},{"b","_..."},{"l","._.."},{"c","_._."},{"f",".._."},{"y","_.__"},{"p",".__."},{"x","_.._"}};

unsigned long time_since_last_click = 0;
static bool last_state = LOW;
String current_seq = "";
String current_word = "";
const int unit = 500;
unsigned long start_time = 0;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  //Set Up Button Mode
  pinMode(buttonPin, INPUT_PULLUP);
}

String get_char(String seq, tuple<String, String> arr[], int size){
  for(int i = 0; i < size; i++){
    if(seq == arr[i].sequence){
      return arr[i].character;
    }
  }
  return "?";
}

String return_char(String seq){
  if(seq.length() == 1){
    return get_char(seq, ones, 2);
  }
  else if(seq.length() == 2){
    return get_char(seq, twos, 4);
  }
  else if(seq.length() == 3){
    return get_char(seq, threes, 8);
  }
  else{
    return get_char(seq, fours, 12);
  }
}

void loop() {
  unsigned long current_time = millis();
  if((current_time - time_since_last_click > 3 * unit && current_seq.length() > 0) || current_seq.length() == 4){
    current_word += return_char(current_seq);
    current_seq = "";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(current_word);
  }
  if(current_time - time_since_last_click > 7 * unit && current_word.length() != 0 && current_word.charAt(current_word.length() - 1) != ' ') {
    current_word += " ";
    current_seq = "";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(current_word); 
  }

  bool curr_state = digitalRead(buttonPin);

  if(last_state == LOW && curr_state == HIGH){
    start_time = millis();
  }

  if(last_state == HIGH && curr_state == LOW){
    unsigned long end_time = millis() - start_time;

    //Check what time tick its in.
    if(end_time < unit){
      current_seq += ".";
    }
    else if (end_time < 3 * unit){
      current_seq += "_";
    }

    lcd.setCursor(0, 1);
    lcd.print(current_seq);
    time_since_last_click = millis();
  }

  last_state = curr_state; 
}
