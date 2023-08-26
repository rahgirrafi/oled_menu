
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

const int upButton = 7;
const int downButton = 6;
const int enterButton = 5;
const int backButton = 4;


  bool upPressed;
  bool downPressed;
  bool enterPressed;
  bool backPressed;
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int numberOfLevels= 3;
int previousSelected[numberOfLevels] = {0,0,0};
int selectedItem = 0 ;
int currentLevel = 0;



const char* mainMenuOptions[]= {
  "SENSOR_READING",
  "PID",
  "MEMORY",
  "MOTOR_SPEED",
  "RUN"
};

int mainMenuSize = sizeof(mainMenuOptions) / sizeof(mainMenuOptions[0]);


const char* lvl1_sensorReadingOptions[] = {
  "Threshold",
  "BIN_value",
  "Raw_value",
  "Position",
};

int sensorReadingSize = sizeof(lvl1_sensorReadingOptions) / sizeof(lvl1_sensorReadingOptions[0]);

const char* lvl1_pidMenuOptions[] = {
  "Kp",
  "Ki",
  "Kd",
  "Save",
};
 int pidMenuSize = sizeof(lvl1_pidMenuOptions) / sizeof(lvl1_pidMenuOptions[0]);

const char* lvl1_memoryMenuOptions[] = {
  "History",
  "Save",
  "Load",
};

int memoryMenuSize = sizeof(lvl1_memoryMenuOptions) / sizeof(lvl1_memoryMenuOptions[0]);


/*
case menu will be added later
*/

const char* lvl1_motorSpeedMenuOptions[] = {
  //the motor speede will be shown in this menu. Will be added later.
  "Increase",
  "Decrease",
  "Back"
};

int motorSpeedMenuSize = sizeof(lvl1_motorSpeedMenuOptions) / sizeof(lvl1_motorSpeedMenuOptions[0]);



void setup() {
  Serial.begin(9600);

//button setup
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(enterButton, INPUT_PULLUP);
  pinMode(backButton, INPUT_PULLUP);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.display(); 
}

void displayMenu( const char* menuOptions[], int menuSize) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 10);

  for (int i = 0; i < menuSize; ++i) {
    if (i == selectedItem) {
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }

    display.println(menuOptions[i]);
  }

  display.display();
}

void getMenu(int level, int selected){
  
  if(level == 0){
    displayMenu(mainMenuOptions, mainMenuSize);
  }
  else if(level == 1){
    if(selected == 0){
      displayMenu(lvl1_sensorReadingOptions, sensorReadingSize);
    }
    else if(selected == 1){
      displayMenu(lvl1_pidMenuOptions, pidMenuSize);
    }
    else if(selected == 2){
      displayMenu(lvl1_memoryMenuOptions, memoryMenuSize);
    }
    else if(selected == 3){
      displayMenu(lvl1_motorSpeedMenuOptions, motorSpeedMenuSize);
    }

}
}
//this function moves the cursor up and down according to menu size. 
//It is later used in navigateMenu function to reduce the code size.
void cursor_move(int menuSize)
{
  if(upPressed == HIGH){
    selectedItem = (selectedItem + 1) % menuSize;
    delay(200);
  }
  else if(downPressed == HIGH){
    selectedItem = (selectedItem - 1) % menuSize;
    delay(200);
  }
}
//this function navigates the menu. It is called in loop function.
void navigateMenu(){
  upPressed = digitalRead(upButton);
  downPressed = digitalRead(downButton);
  enterPressed = digitalRead(enterButton);
  backPressed = digitalRead(backButton);
//this part moves the cursor up and down
  if(currentLevel==0)
  {
    cursor_move(mainMenuSize);
  }
  if (currentLevel==1 && previousSelected[currentLevel-1]==0)
  {
    cursor_move(sensorReadingSize);
  }
  else if (currentLevel==1 && previousSelected[currentLevel-1]==1)
  {
    cursor_move(pidMenuSize);
  }
  else if (currentLevel==1 && previousSelected[currentLevel-1]==2)
  {
    cursor_move(memoryMenuSize);
  }
  else if (currentLevel==1 && previousSelected[currentLevel-1]==3)
  {
    cursor_move(motorSpeedMenuSize);
  }

//this part changes the level of menu
  if(enterPressed == HIGH){
    previousSelected[currentLevel] = selectedItem;
    selectedItem = 0;
    currentLevel = (currentLevel + 1) % numberOfLevels;
    delay(200);
  }
  else if(backPressed == HIGH){
    
    currentLevel = (currentLevel - 1) % numberOfLevels;
    delay(200);
  }



}


void loop() {
  Serial.print(currentLevel);
  Serial.print('\t');
  Serial.print(previousSelected[currentLevel-1]);
  Serial.print('\t');
  Serial.println(selectedItem);


  if(currentLevel==0)
  {
    getMenu(currentLevel, selectedItem);
  }
  else
    getMenu(currentLevel, previousSelected[currentLevel-1]);

  navigateMenu();

}
