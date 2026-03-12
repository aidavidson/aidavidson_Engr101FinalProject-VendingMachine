// header files
#include <LiquidCrystal.h>
#include <Keypad.h>


#define numItems 10
#define maxNumItems 20
#define ROWS 4
#define COLS 4

struct Vending {
  String ItemCode;
  String vendingItem;
  double vendingPrices;
};

struct Vended{
  String Item;
  double itemPrice;
};





char hexaKeys[ROWS][COLS] = 
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

//pins
//pushbuttons
int pushbutton1 = 39;
int pushbutton2 = 41;

// motor
//int DIRA = 22;
//int DIRB = 24;


// joystick
int JoyStick_X = A0; 
int JoyStick_Y = A1;
int JoyStick_Z = 5; 

// thermistor 
int thermPin = A2;

// PIR sensor
int PIRIn = 13;
int PIROut = 12;


byte rowPins[ROWS] = {37, 35, 33, 31}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {29, 27, 25, 23}; //connect to the column pinouts of the keypad
LiquidCrystal lcd(11, 10, 9, 8, 7, 6);

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

Vending drinks[numItems] = {
  {"0", "Pepsi" , 2.0},
  {"1", "PepsiZero" , 1.5},
  {"2", "CocaCola" , 2.0},
  {"3", "DietCoke" , 2.5},
  {"4", "Fanta" , 2.0},
  {"5", "Powerade" , 3.0},
  {"6", "Dasani" , 1.0},
  {"7", "Gatorade" , 2.0},
  {"8", "Monster" , 2.5},
  {"9", "Sprite" , 2.0},
};

Vending snacks[numItems] = {
  {"0", "m&ms" , 1.0},
  {"1", "PopTart" , 3.0},
  {"2", "Twix" , 2.0},
  {"3", "Snickers" , 1.5},
  {"4", "HotCheetos" , 2.5},
  {"5", "Doritos" , 2.0},
  {"6", "BBQLays" , 2.0},
  {"7", "Skittles" , 1.0},
  {"8", "TrailMix" , 2.0},
  {"9", "Munchies" , 2.0},
};

char userInput;
int lcdOn = 0;
// current indicy of viewing
int ItemIndicy = 1;
int cartIndicy = 0;
// drinks / snacks
int vendMode = 0;
//scrolling / buying
int resetted = 0;
int vendOver = 0;

int tempF;
int fanOn;
//user items
Vended userCart[maxNumItems];
Vending vendArr[numItems];


//function declarations
void appendItem(String keycode);

float totalPrice();

void setMode(int type);

void printItems();



//function definitions
void appendItem(String keycode)
{
  for(int i = 0; i < numItems; i++)
  {
    
    if(keycode == vendArr[i].ItemCode)
    {
      userCart[cartIndicy] = {vendArr[i].vendingItem, vendArr[i].vendingPrices};
    } 
    
  }
  cartIndicy++;
}


void setMode(int type){
  if(type == 1){
    for(int i = 0; i < numItems; i++){
      vendArr[i].ItemCode = snacks[i].ItemCode;
      vendArr[i].vendingItem = snacks[i].vendingItem;
      vendArr[i].vendingPrices = snacks[i].vendingPrices;
    }
    vendMode = 1;
  }else if(type == 0){
    for(int i = 0; i < numItems; i++){
      vendArr[i].ItemCode = drinks[i].ItemCode;
      vendArr[i].vendingItem = drinks[i].vendingItem;
      vendArr[i].vendingPrices = drinks[i].vendingPrices; 
    }
    vendMode = 0;
  }
}


void printItems() {
   lcd.setCursor(0,0);
   lcd.print(vendArr[ItemIndicy-1].ItemCode);
   lcd.print(vendArr[ItemIndicy-1].vendingItem);
   lcd.print(vendArr[ItemIndicy-1].vendingPrices);
   lcd.setCursor(0,1);
   lcd.print(vendArr[ItemIndicy].ItemCode);
   lcd.print(vendArr[ItemIndicy].vendingItem);
   lcd.print(vendArr[ItemIndicy].vendingPrices);
}
                 

float totalPrice() {
  float totalPrice = 0;
  for(int i = 0; i < maxNumItems; i++){
    totalPrice += userCart[i].itemPrice;
  }
  return totalPrice;
}


char validkeys[10] = {'0','1','2','3','4','5','6','7','8','9'};

bool validKey(char key){
  for(int i = 0; i < 10; i++){
    if(key == validkeys[i]){
       return true;
    }
  }
  return false;
}

void reset(){
  ItemIndicy = 1;
  cartIndicy = 0;
  vendMode = 0;
  resetted = 1;
  for(int i = 0; i < numItems; i++){
    vendArr[i] = {"","",0.0};
  }
  Vended emptyObj = {"",0.0};
  for(int i = 0; i < maxNumItems; i++){
    userCart[i] = {"",0.0};
  }
}
  
void turnOnFan(float temperature){
  if(temperature > 70){
    fanOn = 1;
  }
  /*
  if(fanOn = 1){
    digitalWrite(DIRA,HIGH); //one way
    digitalWrite(DIRB,LOW);
  }*/
}

float tempReading(void){
  int tempReading = analogRead(thermPin);
  // This is OK
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
  float tempF = (tempC * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
  return tempF;
}

void joyStickInput(int x, int y, int z){
  while(z == 0){
    tempF = tempReading();
    lcd.clear();
    lcd.print("Temp F:   ");
    lcd.print(tempF);
    lcd.setCursor(0,1);
    if(fanOn == 1){
      lcd.print("Fan is On");
    }else if(fanOn == 0){
      lcd.print("Fan is Off");
    }
    z = digitalRead(JoyStick_Z);
    delay(1000);
  }
  printItems();
  if(x < 250){
    if(vendMode != 1){
      ItemIndicy = 1;
      setMode(1);
      lcd.clear();
	    printItems();
    }
  }else if(x > 750){
    if (vendMode != 0){
      ItemIndicy = 1;
      setMode(0);
      lcd.clear();
	    printItems();
    }
  }else if(y < 250){
    ItemIndicy++;
    lcd.clear();
    printItems();
  }else if(y > 750){
    if(ItemIndicy < numItems-1){
      ItemIndicy--;
      lcd.clear();
      printItems();
    }
  }
  delay(1000);
}
void userBuyRequest(){
  lcd.clear();
  lcd.print("Would you like");
  lcd.setCursor(0,1);
  lcd.print("to buy?");
  delay(1000);
  lcd.clear();
  lcd.print("  purchase?   ");
  lcd.setCursor(0,1);
  lcd.print("yes(B) no(any)");
  userInput = customKeypad.waitForKey();
  if(userInput == 'B'){
      lcd.clear();
      lcd.print("Buy which item:");
      char code = customKeypad.waitForKey();
      if(!validKey(code)){
        while(!validKey(code)){
          lcd.clear();
          lcd.print("invalid");
          lcd.clear();
          code = customKeypad.waitForKey(); 
        }
      }
      lcd.setCursor(0,1);
      lcd.print(String(code));
      appendItem(String(code));
      delay(1000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("continue buying?");
      lcd.setCursor(0,1);
      lcd.print("yes(Lb)   no(Rb)");
      pushButtonCode();
  }else{
    return;
  }
}

void pushButtonCode(){
  int leftButtonState = digitalRead(pushbutton1);
  int rightButtonState = digitalRead(pushbutton2);
  
  while(leftButtonState == LOW || rightButtonState == LOW){
    Serial.print("Left Button State: ");
    Serial.print(leftButtonState);
    Serial.println("");
    Serial.print("Right Button State: ");
    Serial.print(rightButtonState);
    Serial.println("");
    if(leftButtonState == HIGH){
        ItemIndicy = 1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Continue to shop");
        break;
    }
    if(leftButtonState == HIGH){
        ItemIndicy = 1;
        lcd.clear();
        lcd.setCursor(0,0);
        pay();
        delay(1000);
        vendOver = 1;
        break;

    }
    leftButtonState = digitalRead(pushbutton1);
    rightButtonState = digitalRead(pushbutton2);
  }
}
void pay(){
  lcd.print("User Cart: ");
  lcd.setCursor(0,1);
  lcd.print("$");
  lcd.print(totalPrice());
}

void setup() {
  lcd.begin(16,2);
  
  // put your setup code here, to run once:
  pinMode(JoyStick_Z, INPUT_PULLUP); //Z axis is defined as an input PS2
  //pinMode(DIRA,OUTPUT);
  //pinMode(DIRB,OUTPUT);
  pinMode(PIRIn,INPUT);
  pinMode(PIROut,OUTPUT);
  pinMode(pushbutton1, INPUT);
  pinMode(pushbutton2, INPUT);
  pinMode(thermPin, INPUT);
  setMode(0);
  Serial.begin(9600);
  
  
}





void loop() {
  //User Independant
  lcdOn = 1;
  if(vendOver == 1)
  {
    reset();
    digitalWrite(PIROut, LOW);
    lcdOn = 0;
    vendOver = 0;
     
  }
  
  
  if(resetted == 1)
  {
    setMode(0);
    resetted = 0;
  }
  
  
  if(userCart[maxNumItems-1].itemPrice != 0.0){
    pay();
  }
  
  
  if(ItemIndicy == numItems-2){
    userBuyRequest();
  }
  

  if(lcdOn == 1){
    tempF = tempReading();
    turnOnFan(tempF);
    lcd.begin(16,2);
    digitalWrite(PIROut, HIGH);
    printItems();
    int x,y,z;
    x = analogRead(JoyStick_X);
    y = analogRead(JoyStick_Y);
    z = digitalRead(JoyStick_Z);
    joyStickInput(x,y,z);
  }
  
  // User Dependant
  

}





