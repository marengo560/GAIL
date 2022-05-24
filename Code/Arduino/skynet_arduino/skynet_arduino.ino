/*
 * Skynet Arduino Control Code
*/

#include <Servo.h>
#include "adagio.h"

Servo riser, pan, tilt;

int pos;
int tiltPos;
int riserPos=0;

void setup() {
  initServos();
  initSerial();
  cmdRequest = true;  //request a user command
  Serial.println("(i)Enter <1> including brackets for menu(/i)");
}

void loop() {
  getUserInput();  // Non blocking when no data in UART buffer

  if (newData || requestUserInput) {  // There is:  1.A new command
                                      //            2.An on-going check for user input from a non blocking
                                      //              function being called from the 'case' as indicated
                                      //              by 'userCmd'(loop() that continues to call through 'switch')
                                      //            3.New valid user data not yet 'consumed' by some code

    if (newData && cmdRequest) {  // Update 'userCmd' new user command for switch structure
      userCmd = atoi(rxBuff);
      clear_rxBuff();  // clear the buffer for the next user input
      newData = cmdRequest = false;
    }

    switch (userCmd) {  // Same case could be called many times e.g. if a
                        // downstream function is awaiting user input (non blocking)
      case 1:           // <1>
        printMenu();
        newData = requestUserInput = false;  // Clear flags, downstream function could request user input again
        cmdRequest = true;                   // Should ideally be set by called function return - task may not complete on this call
        break;

      case 2:  // <2>
        changeAwakeState();
        break;

      // case 3:  // <2>
      //   pan();
      //   break;

      // case 4:  // <2>
      //   tilt();
      //   break;

      case 5:           // <4>
        blinkLED_BUILT_IN();
        newData = requestUserInput = false;  // Clear flags, downstream function could request user input again
        cmdRequest = true;                   // Should ideally be set by called function return - task may not complete on this call
        break;        

      default:
        Serial.print("(w)User command = ");
        Serial.print(userCmd);
        Serial.println(" not recognised(/w)");
        clear_rxBuff();
        newData = false;
        cmdRequest = true;
        break;
    }
  }
}


/*-------------------------FUNCTIONS----------------------------*/
void initServos(){
  
  riser.attach(9);  
  pan.attach(10);
  tilt.attach(11);
  riser.write(90);  
  pan.write(105);
  tilt.write(80);
  
}
void initSerial() {
  /*
  unsigned long t0, t1;
  t0 = micros();
  */
  Serial.begin(BAUDRATE);
  /*
  while(!Serial) {;}          // haven't seen this add any delay on Arduino Uno
  t1 = micros();
  Serial.print("Serial initialized in ");
  Serial.print(t1 - t0);
  Serial.println("us");
  Serial.println();
  */
}

// This function gets the user inputs encapsulated within the angle brackets <...> from the serial port
void getUserInput() {
  static bool inProgress = false;  // to indicate when a valid msg is being read - see next comment re static declaration
  static uint8_t index = 0;        // declare static because valid msg(s) might be read over multiple calls
  char rx;                         // local variable for sucessive RX bytes from UART
  char startMark = '<';            // look for start symbol. Don't assume all UART buffer data is valid
  char endMark = '>';              // message ends when indicated by the end symbol

  while (Serial.available() > 0 && !newData) {  // only process one user input at a time
    rx = (char)Serial.read();                   // always read data if available which empties the UART buffer whether valid data or not
    if (inProgress) {                           // only retreive data after the start symbol (startMark byte) is detected
      if (rx != endMark && index < MAX_DATA_LEN) {
        rxBuff[index++] = rx;  // add the data to rxBuff after the start byte was found on a previous iteration
        //Serial.println(rx);               // debug
      }
      if (rx == endMark) {
        rxBuff[index] = '\0';              // put a string termination char (0x00) at the end of the rxBuff char array
        index = 0;                         // reset the index for the next valid message
        newData = true;                    // set the newData flag to indicate a valid message
        if (rxBuff[index] != startMark) {  // only set inProgress to false if there isn't another user input
          inProgress = false;
        }
        if (index == MAX_DATA_LEN && !newData) {  // fault - more data than allocated buffer space - dump the data
          inProgress = false;
          index = 0;
          clear_rxBuff();
        }
      }
    } else if (rx == startMark) {  // when the start symbol is found inProgress flag is set and if(inProgress)
      inProgress = true;           // evaluates true on the next iteration
    }
  }
}

// This function clears the receive buffer in case incorrect data is received
void clear_rxBuff() {
  for (int i = 0; i < MAX_DATA_LEN; i++) {
    rxBuff[i] = '\0';  // 0x00 string termination character
  }
}

// Function for debug purposes
void toggleDebugPin() {
  static boolean pinState = false;
  if (pinState) {
    digitalWrite(DEBUG_PIN, LOW);
  } else {
    digitalWrite(DEBUG_PIN, HIGH);
  }
  pinState = !pinState;
}

// Function for debug purposes
void blinkLED_BUILT_IN() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}

void changeAwakeState() {
  if (requestUserInput) {
    if (newData) {
      String argument = String(rxBuff);
      int awakeFlag = sanitiseInputNum(argument.toInt());
      //Do Something------------------------------------
      if(awakeFlag==1){
          for (riserPos = 90; riserPos <= 180; riserPos += 1) { // goes from 0 degrees to 180 degrees
            riser.write(riserPos); 
            delay(5); 
            tiltPos = map(riserPos,80,180,80,20);     
            tilt.write(tiltPos);             // tell servo to go to position in variable 'pos'
            delay(5); 
          }
          } 
else{         
   for (riserPos = 180; riserPos >= 90; riserPos -= 1) { // goes from 0 degrees to 180 degrees
            riser.write(riserPos); 
            delay(5); 
            tiltPos = map(riserPos,80,180,80,20);     
            tilt.write(tiltPos);             // tell servo to go to position in variable 'pos'
            delay(5); 
   }
  
}                
      Serial.print("(i)Function <2> called. Argument parsed correctly as float. Value was: ");
      Serial.print(awakeFlag);
      Serial.println("(/i)");
    
      //----------------------------------------------------
      newData = requestUserInput = false;
      cmdRequest = true;  // return to looking for user commands
      }
    }
   else {
    newData = false;  // if rxBuff data isn't available request it
    requestUserInput = true;
  }
}

void moveServos() {
  if (requestUserInput) {
    if (newData) {
      String argument = String(rxBuff);
      //Do Something------------------------------------
      float readNum = sanitiseInputNum(argument.toInt());
      //Do Something------------------------------------
            
      pan.write(readNum);
      Serial.print("(i)Function <2> called. Argument parsed correctly as float. Value was: ");
      Serial.print(readNum);
      Serial.println("(/i)");
      //----------------------------------------------------
      newData = requestUserInput = false;
      cmdRequest = true;  // return to looking for user commands
    }
  } else {
    newData = false;  // if rxBuff data isn't available request it
    requestUserInput = true;
  }
}

// This function prints the main menu of options from FLASH
void printMenu() {
  for (int i = 0; i < mainStrTableCount; i++) {
    strcpy_P(flashBuff, (char *)pgm_read_word(&(stringTable[i])));  // Necessary casts and dereferencing.
    Serial.println(flashBuff);
    delay(1);
  }
}

// This function sanitises voltage inputs in case a NaN is sent
float sanitiseInputNum(float num) {
  if (isnan(num)) {
    Serial.println("(w)Numeric input was parsed as NaN. Using 0(/w)");
    return 0;
  } else {
    return num;
  }
}
