#include <IRremote.h> // Бібліотека для ІЧ модуля
#include <Wire.h>  // i2P шина
#include <LiquidCrystal_I2C.h> // дисплей LCD 1602
LiquidCrystal_I2C lcd(0x3F,16,2);  // 

int AnimPlayer = 1; 
int GGpozX = 8; 
int GGpozY = 1; 
int Emeny_check_1 = 0;  
int Emeny_control = 0; 
int E1pozX = 16; 
int E1pozY = 1; 
int HeartControl = 0; 
int LifeCheck = 3; 

long Timer_z = 0; 
long AnimatedTime = 300;  
long AnimatedTimeCheck = 0;  
long HeartHitBig = 800;  
long HeartHitLight = 250;  
long HeartHitBigCheck = 0;  
long HeartHitLightCheck = 0;  
long BatteyBlink = 200;   
long BatteyBlinkCheck = 0;  
long JumpUP = 800;  
long JumpUPCheck = 0;   
long DHTTimeR = 1000; 
long DHTTimeRCheck = 0; 
long TimeBlink = 500;   
long TimeBlinkCheck = 0;  
  long currentMillis = 0;   

bool JumpB = false;   
bool BatteryB = false;  
bool hitON = false;   

decode_results results; 
IRrecv irrecv(A0);  

enum { SYMBOL_HEIGHT = 8 };
byte Player_1[SYMBOL_HEIGHT] = {B01110,B01110,B00100,B01110,B10101,B00100,B01110,B01010,};
byte Player_2[SYMBOL_HEIGHT] = {B00000,B01110,B01110,B00100,B11111,B00100,B01110,B01010,};
byte Enemy_1[SYMBOL_HEIGHT]  = {B00010,B00110,B10111,B10110,B11111,B00110,B00110,B11111,};
byte Heart_L[SYMBOL_HEIGHT]  = {B00000,B01010,B11111,B11111,B11111,B01110,B00100,B00000,};
byte Heart_R[SYMBOL_HEIGHT]  = {B00000,B00000,B01010,B11111,B01110,B00100,B00000,B00000,};
byte Battery1[SYMBOL_HEIGHT] = {B01110,B11111,B11111,B11111,B11111,B11111,B11111,B11111,};
byte Battery2[SYMBOL_HEIGHT] = {B01110,B10001,B10011,B10111,B11111,B11111,B11111,B11111,};
byte Battery3[SYMBOL_HEIGHT] = {B01110,B10001,B10001,B10001,B10011,B10111,B11111,B11111,};
byte Battery4[SYMBOL_HEIGHT] = {B01110,B10001,B10001,B10001,B10001,B10001,B10001,B11111,};

void setup() {
  Serial.begin(9600);   
  irrecv.enableIRIn();  
  lcd.init();   
  Wire.begin();   
  lcd.backlight();  
}
void loop() {  
  currentMillis = millis();   
  IRCheck ();   
    if (AnimPlayer < 3){  
    if (LifeCheck == 3) {lcd.createChar(3, Battery1);}  
    if (LifeCheck == 2) {lcd.createChar(3, Battery2);}  
    if (LifeCheck == 1) { 
      if (BatteryB == false){lcd.createChar(3, Battery3);}  
      if (BatteryB == true){lcd.createChar(3, Battery4);}   
      if (currentMillis - BatteyBlinkCheck >= BatteyBlink) {BatteyBlinkCheck = currentMillis;   
      if (BatteryB == false) {BatteryB = true;} else {BatteryB = false;}}   
    }
    timer();  
    check_hit_gg_1 ();  
    PlAn();   
    HeartHit ();  
    enemy_go();   
  }
}
void IRCheck ()   
{
  if ( irrecv.decode( &results ))   
  { 
      if (results.value == 0xFF18E7 && GGpozY == 1){  
        GGclear ();   
        GGpozY = 0;   
        JumpB = true;   
        JumpUPCheck = currentMillis;  
        } // 2 (кнопки на іч пульті)
      if (results.value == 0xFF10EF && GGpozX >= 0){    
        GGclear (); 
        GGpozX -= 1;    
      } // 4
      if (results.value == 0xFF5AA5 && GGpozX <= 15){ 
        GGclear ();   
        GGpozX += 1;  
        } // 6
      if (results.value == 0xFF6897){ // 0 //рестарт…
        lcd.clear(); 
        AnimPlayer = 1; 
        LifeCheck = 3; 
        Timer_z = 0; 
        GGpozX = 8; // \/ \/ \/
        GGpozY = 1; // 
        Emeny_check_1 = 0; 
        E1pozX = 16; // \/ \/ \/ 
        E1pozY = 1;   // 
   }
    irrecv.resume(); // 
 }
}
void timer () // 
{
  if (currentMillis - DHTTimeRCheck >= DHTTimeR)  
  {
    DHTTimeRCheck = currentMillis; 
    Timer_z ++;  
    lcd.setCursor(0, 0);  
    lcd.print(Timer_z); 
  }
}

void PlAn () 
{
  if (JumpB == true && GGpozY == 0){ 
    if (currentMillis - JumpUPCheck >= JumpUP) { 
      JumpB = false; GGclear (); GGpozY = 1; 
    }
  }
  if (AnimPlayer == 1){lcd.createChar(0, Player_1);} 
  if (AnimPlayer == 2){lcd.createChar(0, Player_2);} 
  if (AnimPlayer < 2) 
  {
    lcd.setCursor(GGpozX, GGpozY);  
    lcd.write(0); 
  }
  if (currentMillis - AnimatedTimeCheck >= AnimatedTime) { 
    AnimatedTimeCheck = currentMillis; 
    if (AnimPlayer == 2){AnimPlayer = 1;} 
    else{AnimPlayer = 2;} 
  }
}
void GGclear () 
{
  lcd.setCursor(GGpozX, GGpozY);  
  lcd.print(" "); 
}
void enemy_go () 
{
  if (Emeny_check_1 == 0)  
      {
        Emeny_control = random (100); 
        if (Emeny_control == 1) {  
Emeny_check_1  =  1; 
hitON = false; 
      }
  }
  if (Emeny_check_1 == 1)   
  {
    if (currentMillis - TimeBlinkCheck >= TimeBlink)  
    {
      TimeBlinkCheck = currentMillis;   
      lcd.createChar(2, Enemy_1);   
      lcd.setCursor(E1pozX, E1pozY); 
      lcd.print(" ");   
      E1pozX--;   
      lcd.setCursor(E1pozX, E1pozY);    
      lcd.write(2);   
      if (E1pozX <= 0)  
      {
        lcd.setCursor(0,1);   
        lcd.print(" ");   
        Emeny_control = 0;  
        Emeny_check_1 = 0;  
        E1pozX = 16; // - \/ \/ \/
        E1pozY = 1;   
      }
    }
  }
}
void check_hit_gg_1 ()  
{
  if (E1pozX == GGpozX && E1pozY == GGpozY && hitON == false){    
    LifeCheck -= 1;   
    hitON = true;   
    if (LifeCheck <= 0){  
      AnimPlayer = 50;  
      Emeny_check_1 = 50;   
      lcd.clear();  
      lcd.setCursor(3, 0);  
      lcd.print("GAME OVER");   
    }
  } else {  
  lcd.setCursor(13, 0);   
  lcd.write(1);   
  lcd.setCursor(14, 0);
  lcd.print("=");   
  lcd.setCursor(15, 0);
  lcd.write(3);   
  }
}
void HeartHit ()  
{
  if (HeartControl == 0 || HeartControl == 2){lcd.createChar(1, Heart_L);}  
  if (HeartControl == 1 || HeartControl == 3){lcd.createChar(1, Heart_R);} 
  if (currentMillis - HeartHitBigCheck >= HeartHitBig) { 
    if (currentMillis - HeartHitLightCheck >= HeartHitLight) {  
      HeartHitLightCheck = currentMillis;   
      if (HeartControl<3){HeartControl++;}    
      else {HeartControl = 0; HeartHitBigCheck = currentMillis;} 
    } 
  }
}
