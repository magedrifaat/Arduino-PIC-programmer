#define MCLR 8
#define PGM 9
#define CLK 10
#define DATA 11


/*
This sketch is the arduino part of the pic programmer project.
It works on the low voltage programming mode and the configuration word
is fixed to ensure it remains in this mode
All the user interaction should be from the python part and this sketch
should only be edited for changing the pin numbers, or if you really know 
what you are doing.

Connection between the And the arduino should be as follows:
  Aduino    |    PIC
--------------------------------  
  MCLR(8)  ->   MCLR(usually pin 1)
  PGM(9)   ->    RB3
  CLK(10)  ->    RB6
  DATA(11) ->    RB7

If your PIC is already configured to LVP disabled just dont connect the pgm pin to anything, keep the MCLR pin of the pic
low then after starting the serial connection in the python program and before choosing an option, connect the MCLR pin of the PIC
to 13V (12V will do also).It might not pass the verification afterwards because it will have LVP enabled already so just reconnect the 
MCLR an PGM to the arduino to test it.
*/

int DATA_SIZE = 0;
int debugging = 0; //flag for debugging mode
void setup() 
{
    pinMode(MCLR, OUTPUT);
    pinMode(PGM, OUTPUT);
    pinMode(DATA, OUTPUT);
    pinMode(CLK, OUTPUT);
    
    digitalWrite(DATA, LOW);
    digitalWrite(CLK, LOW);
    digitalWrite(PGM, LOW);
    digitalWrite(MCLR, LOW);
    
    Serial.begin(9600);
    DATA_SIZE = nextWord(); //getting the data size by making nextWord function send 0x04
    Serial.print("Enter P for prgramming, V for verification, or D for verification with debugging: ");
}

void loop() {
    
    Serial.write(2); //request input
    while(Serial.available() == 0); //waiting for input
    int input = Serial.read();
    switch(input)
    {
        case 'p':
        case 'P':
          LVP_init();
          Program(DATA_SIZE);
          break;
        case 'v':
        case 'V':
          debugging = 0;
          LVP_init();
          verify(DATA_SIZE);
          break;
        case 'd':
        case 'D':
          debugging = 1;
          LVP_init();
          verify(DATA_SIZE);
          break;
    }
}

void LVP_init()
{
    // LVP protocol is keeping Data pin and clock pin low
    // then driving pgm pin high then MCLR pin high
    digitalWrite(DATA, LOW);
    digitalWrite(CLK, LOW);
    digitalWrite(PGM, LOW);
    digitalWrite(MCLR, LOW);
    delay(1);
    digitalWrite(PGM, HIGH);
    delayMicroseconds(1);
    digitalWrite(MCLR, HIGH);
    delayMicroseconds(10);
    
}

void Program(int _data_size)
{
    chipErase();
    for (int i = 0; i < _data_size; i++)
    {
       LoadData(nextWord());
       StartProgram();
       increment(); 
    }
    loadConfig();
    for(int i = 0; i < 7; i ++)increment();
    LoadData(0b11111111111010); //Configuration word (edit only if you know what you are doing
    StartEraseProgram();
    Serial.println("Progamming Done");
    Serial.write(3);
}

void verify(int _data_size)
{
    int flag = 0;
    for(int i = 0; i < _data_size; i++)
    {
        int _word = nextWord();
        if(readData() != _word)
        {
            if(debugging == 1)
            {
                Serial.print("Error at word "); 
                Serial.print(i,DEC); 
                Serial.print(": Expected ");  
                Serial.print(_word,BIN); 
                Serial.print(" but found "); 
                Serial.println(readData(),BIN);
            }
            flag = 1;
        }
        delay(1);
        increment();
    }
    loadConfig();
    for(int i = 0; i < 7; i ++)increment();
    if(readData() != 0b11111111111010)
    {
        if(debugging == 1)
        {
            Serial.print("Error in configuration word: ");
            Serial.println(readData(),BIN);
        }
        flag = 1;
    }
    if(flag == 0)
        Serial.println("Verification Done");
    else if(debugging == 0)
        Serial.println("Verification failed, try reprogramming or debugging.");
    Serial.write(3);
}

int nextWord()
{
    int value = 0;
    if(DATA_SIZE == 0)
      Serial.write(4);
    else
      Serial.write(1);
    for(int i = 0; i < 2; i++)
    {
        while(Serial.available() == 0);
        value += Serial.read()<<(8*i);
    }
    return value;
}
    
void writeBit(int a)
{
    digitalWrite(DATA, a);
    digitalWrite(CLK, HIGH);
    delayMicroseconds(1);
    digitalWrite(CLK, LOW);
    delayMicroseconds(1);
    digitalWrite(DATA, LOW);
}

byte readBit()
{
    byte _bit;
    digitalWrite(CLK, HIGH);
    delayMicroseconds(1);
    _bit = digitalRead(DATA);
    digitalWrite(CLK, LOW);
    delayMicroseconds(1);
    return _bit;
}

void sendCommand(char command)
{
  for (int i = 1; i <= 0b100000; i *= 2)
  {
    writeBit((command & i) >= 1);
  }
}

void sendData(uint16_t dataword)
{
  
   writeBit(0);
   for (int i = 1; i <= 0b10000000000000; i *= 2)
   {
     writeBit((dataword & i) >= 1);
   }
   writeBit(0);
}

void LoadData(uint16_t dataword)
{
   sendCommand(0b000010);
   delayMicroseconds(2); 
   sendData(dataword);
   delayMicroseconds(2);
}

uint16_t readData()
{
    sendCommand(0b000100);
    delayMicroseconds(2);
    uint16_t value = 0;
    pinMode(DATA, INPUT);
    readBit();
    for(int i = 0; i < 14; i++)
    {
        value = value | (readBit()<<i);
    }
    readBit();
    pinMode(DATA, OUTPUT);
    return value;
}

void StartProgram()
{
   sendCommand(0b011000);
   delay(1);
   sendCommand(0b010111);
   delayMicroseconds(2);
}

void StartEraseProgram()
{
   sendCommand(0b001000);
   delay(4);
}

void loadConfig()
{
   sendCommand(0b000000);
   delayMicroseconds(2); 
   sendData(0x3FFF);
   delayMicroseconds(2);
}

void increment()
{
   sendCommand(0b000110);
   delayMicroseconds(2);
}

void chipErase()
{
   sendCommand(0b011111);
   delay(8);
}
