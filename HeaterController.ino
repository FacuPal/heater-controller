#define Log Serial.println
#define DEBUG false 
#define DEBOUNCER 500

class ArduinoComponent {
  private: 
    byte pinNumber;
    byte pinType;
    byte value; 
};

class Relay : public ArduinoComponent {
  public: 
    Relay(byte _pinNumber): pinNumber(_pinNumber), turnedOn(false){
      pinMode(pinNumber, OUTPUT);
    }

    void turnOn(){
      turnedOn ? Log("Se va a apagar") : Log("Se va a prender");
      digitalWrite(pinNumber,turnedOn=!turnedOn);
      return;
    }
  private: 
    byte pinNumber;
    bool turnedOn;
};

//PWM Class
class PWM : public ArduinoComponent {
  public:
    PWM(byte _pinNumber): pinNumber(_pinNumber), value(20){
      pinMode(pinNumber, OUTPUT);
      updatePWM(20);
    }
    
    void speedUp(){
      if (value + 20 > 100){
        Log("No se aumenta por estar al máximo. PWM: " + (String)value);
        
        return;
      }
      value+=20;
      Log((String)"Se aumenta velocidad: " + value);
      updatePWM(value);
      return;
    }

    void speedDown(){
      if (value - 20 < 20){
        Log("No se disminuye por estar al minimo. PWM: " + (String)value);
        return;
      }
      value-=20;
      Log((String)"Se disminuye velocidad: " + value);
      updatePWM(value);
      return;
    }

    void updatePWM(byte value){
      byte mapValue = map(value, 0, 100, 255, 0);
      if (DEBUG) Log("Se va a escribir " + (String)mapValue);
      analogWrite(pinNumber, mapValue);
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
    Button(byte _pinNumber, void (*_function)(ArduinoComponent*), ArduinoComponent * _arduinoComponent) : pinNumber(_pinNumber), function(_function), lastRead(1), arduinoComponent(_arduinoComponent) {
      pinMode(pinNumber, INPUT_PULLUP);
    }

    void checkPressButton() {
      //TODO: Agregar debouncer
      byte read = digitalRead(pinNumber);
      unsigned long time = millis();
      if(lastRead != read && read == 0 && time - lastPush > DEBOUNCER){
        execute();
        lastPush = time;
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
      function(arduinoComponent);
    }

  private: 
    byte pinNumber;
    byte lastRead; 
    ArduinoComponent * arduinoComponent;
    unsigned long lastPush;
    void (*function)(ArduinoComponent*);
};



//Button Press Functions
void turnOn(Relay * relay){
  relay->turnOn();
  // Log((String)"Se maneja power: " + relay->turnOn());
};

void speedUp(PWM * pwm){
  pwm->speedUp();
  // Log((String)"Se aumenta velocidad: " + pwm->speedUp());
};

void speedDown(PWM * pwm){
  pwm->speedDown();
  //Log((String)"Se disminuye velocidad: " + pwm->speedDown());
};



//Initialization
ArduinoComponent * pwm = NULL;
ArduinoComponent * relay = NULL;
Button * redButton = NULL;
Button * orangeButton = NULL;
Button * whiteButton = NULL; 

void setup() {
  Serial.begin(9600);
  // pinMode(pinRed, INPUT_PULLUP);
  // pinMode(pinOrange, INPUT_PULLUP);
  // pinMode(pinWhite, INPUT_PULLUP);
  pwm = new PWM(6);
  relay = new Relay(5);
  redButton = new Button(2, &speedUp, pwm); 
  orangeButton = new Button(3, &speedDown, pwm);
  whiteButton = new Button(4,&turnOn, relay);
};

void loop() {
  redButton->checkPressButton();
  orangeButton->checkPressButton();
  whiteButton->checkPressButton();
  // Log("El valor de pwm es: " + (String)pwm.getValue());
};