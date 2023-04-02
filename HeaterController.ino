#define Log Serial.println

class ArduinoComponent {
  private: 
    byte pinNumber;
    byte pinType;
    byte value; 
};

//PWM Class
class PWM : public ArduinoComponent {
  public:
    PWM(byte _pinNumber): pinNumber(_pinNumber), value(20){
      pinMode(pinNumber, OUTPUT);
    }
    
    byte speedUp(){
      if (value + 20 > 100){
        Log("No se aumenta por estar al máximo. PWM: " + (String)value);
        updatePWM(value);
        return value;
      }
      return value+=20;
    }

    byte speedDown(){
      if (value - 20 < 20){
        Log("No se disminuye por estar al minimo. PWM: " + (String)value);
        updatePWM(value);
        return value;
      }
      return value-=20;
    }

    void updatePWM(char value){
      analogWrite(pinNumber, map(value, 0, 100, 0, 255));
    }

    byte getValue(){
      return value;
    }
  private: 
    byte pinNumber;
    byte pinType = OUTPUT;
    byte value;

};


//Button Class
class Button {
  public: 
    Button(byte _pinNumber, void (*_function)(ArduinoComponent), ArduinoComponent * _arduinoComponent) : pinNumber(_pinNumber), function(_function), lastRead(1), arduinoComponent(_arduinoComponent) {
      pinMode(pinNumber, INPUT_PULLUP);
    }

    void checkPressButton() {
      //TODO: Agregar debouncer
      byte read = digitalRead(pinNumber);
      if(lastRead != read && read == 0 ){
        Serial.println("last pin " + (String)pinNumber + " : " + (String)lastRead);
        Serial.println("read: " + (String)read);
        execute();
      }
      lastRead = read;
    }

    byte getLastRead(){
      return lastRead;
    }

    unsigned long getLastPush(){
      return lastPush;
    }

    void execute(){
      function(*arduinoComponent);
    }

  private: 
    byte pinNumber;
    byte lastRead; 
    ArduinoComponent * arduinoComponent;
    unsigned long lastPush;
    void (*function)(ArduinoComponent);
};



//Button Press Functions
void imprimir(){
  Serial.println("impresion");
};

void speedUp(PWM * pwm){
  Log((String)"Se aumenta velocidad: " + pwm->speedUp());
};

void speedDown(PWM * pwm){
  Log((String)"Se disminuye velocidad: " + pwm->speedDown());
};



//Initialization
PWM pwm(6);
Button redButton(2, &speedUp, &pwm); 
Button orangeButton(3, &speedDown, &pwm);
Button whiteButton(4,&imprimir, &pwm);


void setup() {
  Serial.begin(9600);
  // pinMode(pinRed, INPUT_PULLUP);
  // pinMode(pinOrange, INPUT_PULLUP);
  // pinMode(pinWhite, INPUT_PULLUP);
};

void loop() {
  redButton.checkPressButton();
  orangeButton.checkPressButton();
  whiteButton.checkPressButton();
  // Log("El valor de pwm es: " + (String)pwm.getValue());
};