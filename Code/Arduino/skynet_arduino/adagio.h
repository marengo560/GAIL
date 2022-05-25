#include <stdint.h>
#include <EEPROM.h>         // Used to read the internal Arduino EEPROM

#define RISER_SLEEP_POS 80
#define RISER_AWAKE_POS 180
#define PAN_SLEEP_POS 105
#define PAN_AWAKE_POS 105
#define TILT_SLEEP_POS 80
#define TILT_AWAKE_POS 20


// Serial Port and Generic Constants
const int BAUDRATE = 9600;        // Serial port speed
const uint8_t DEBUG_PIN = 48;
const uint8_t MAX_CMD_LEN = 32;    // Max Data size of command received (usually 2 chars that represent a number e.g <12>)
const uint8_t MAX_DATA_LEN = 104;  // Max Data size of data received as argument for the functions (can be anything so we give enough space)

// Arduino EEPROM Constants
const uint8_t EEDAC1MIN = 0;     // EEPROM cal data stored as 4 byte float
const uint8_t EEDAC1MAX = 4;     // Increment 4 bytes to next address

// Application Constants

// Strings stored in FLASH
const char mainMenu0[] PROGMEM = {"(i)Encapsulate input in angle brackets <...>\nMenu options:\n"};
const char mainMenu1[] PROGMEM = {"<1>  Print this menu"};
const char mainMenu2[] PROGMEM = {"<2>  Function 2 - Sleep/Awake Mode. Example <2><1> for Awake and <2><0> for sleep mode"};
const char mainMenu3[] PROGMEM = {"<3>  Function 3 - Pan/Tilt . Example <3><deadbeef>"};
const char mainMenu4[] PROGMEM = {"<4>  Blink on-board LED (/i)"};

// Main menu strings table
const uint8_t mainStrTableCount = 5;
const char *const stringTable[mainStrTableCount] PROGMEM = {mainMenu0, mainMenu1, mainMenu2, mainMenu3, mainMenu4};

// Application Variables
char rxBuff[MAX_DATA_LEN + 1];           // Serial port receive buffer. Add 1 to allow for adding a string termination char
bool newData = false;                    // Flag to indicate a valid user input data
bool cmdRequest = false;                 // Flag to indicate a user command request
bool requestUserInput = false;           // Flag to indicate awaiting data from the user
int userCmd = 0;                         // For switch structure, '0' not a valid case                 
      
char flashBuff[128];                     // Holds data from FLASH memory (strings saved and so on)

int riserPos = 0;
int tiltPos = 0;
int panPos = 0;

// Basic Functions
void initSerial(void); 
void getUserInput(void); 
void clear_rxBuff(void); 
void toggleDebugPin(void);
void blinkLED_BUILT_IN(void);
void printMenu(void);
void changeAwakeState(void);
void moveServos(void);
float sanitiseInputNum(float num);