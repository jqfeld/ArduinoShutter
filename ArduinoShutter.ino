const int VERSION = 1;

#include <Servo.h>

#include <Vrekrer_scpi_parser.h>
#include <EEPROM.h>


Servo servo;

SCPI_Parser instrument;

volatile int open_pos = 0;
volatile int close_pos = 90;

const unsigned int open_pos_addr = 10;
const unsigned int close_pos_addr = 20;
int magic = 0;
void setup() {
  Serial.begin(9600);


instrument.RegisterCommand(F("*IDN?"), &IDN);
  instrument.SetCommandTreeBase(F("POSition"));
  instrument.RegisterCommand(F(":SET"), &SetPosition);
  instrument.RegisterCommand(F(":SET?"), &GetPosition);
  instrument.RegisterCommand(F(":OPen"), &SetOpen);
  instrument.RegisterCommand(F(":CLose"), &SetClose);



  instrument.SetCommandTreeBase(F("SYSTem"));
  // instrument.RegisterCommand(F(":DEBug"), &SetDebug);
  // instrument.RegisterCommand(F(":RESet"), &ResetEEPROM);

  // instrument.SetCommandTreeBase(F("SYSTem:POSition"));
  instrument.RegisterCommand(F(":OPen"), &SetOpenPosition);
  instrument.RegisterCommand(F(":OPen?"), &GetOpenPosition);
  instrument.RegisterCommand(F(":CLose"), &SetClosePosition);
  instrument.RegisterCommand(F(":CLose?"), &GetClosePosition);

  servo.attach(2);  // connect yellow wire of servo with pin d9

  EEPROM.get(0, magic);

  if (magic == VERSION) {
    EEPROM.get(open_pos_addr, open_pos);
    EEPROM.get(close_pos_addr, close_pos);

    Serial.println("Configuration loaded!");

  } else {
    EEPROM.put(open_pos_addr, open_pos);
    EEPROM.put(close_pos_addr, close_pos);

    EEPROM.put(0, VERSION);
    Serial.println("No configuration found, set defaults!");
  }

  Serial.println("Setup completed!");
  Serial.print("Open position: ");
  Serial.println(open_pos);
  Serial.print("Close position: ");
  Serial.println(close_pos);
}



void loop() {
  // Serial.println("To set servo position use SET <pos>.");
  // Serial.println("<pos> can be an integer between 0 and 180.");

  instrument.ProcessInput(Serial, "\n");
}


void IDN(SCPI_C commands, SCPI_P parameters, Stream& interface) {
  interface.println("Jan Kuhfeld,ArduinoShutter,None,0.1/0.1");
}

void SetPosition(SCPI_C commands, SCPI_P parameters, Stream& interface) {
  if (parameters.Size() > 0) {
    int pos = constrain(String(parameters[0]).toInt(), 0, 180);
    servo.write(pos);
  }
}

void SetOpen(SCPI_C commands, SCPI_P parameters, Stream& interface) {
  servo.write(open_pos);
}

void SetClose(SCPI_C commands, SCPI_P parameters, Stream& interface) {
  servo.write(close_pos);
}

void GetPosition(SCPI_C commands, SCPI_P parameters, Stream& interface) {
  interface.println(servo.read());
}

void SetOpenPosition(SCPI_C commands, SCPI_P parameters, Stream& interface) {
  if (parameters.Size() > 0) {
    open_pos = constrain(String(parameters[0]).toInt(), 0, 180);
    // interface.println(open_pos);
    EEPROM.put(open_pos_addr, open_pos);
  }
}

void GetOpenPosition(SCPI_C commands, SCPI_P parameters, Stream& interface) {
  interface.println(open_pos);
}
void SetClosePosition(SCPI_C commands, SCPI_P parameters, Stream& interface) {
  if (parameters.Size() > 0) {
    close_pos = constrain(String(parameters[0]).toInt(), 0, 180);
    // interface.println(close_pos);

    EEPROM.put(close_pos_addr, close_pos);
  }
}
void GetClosePosition(SCPI_C commands, SCPI_P parameters, Stream& interface) {
  interface.println(close_pos);
}
