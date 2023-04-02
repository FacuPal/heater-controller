// byte valor = 0;
// byte outputPin = 6; 

// void setup() {
//   Serial.begin(9600);
//   // put your setup code here, to run once:
//   pinMode(outputPin, OUTPUT);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   if (valor == 250 )
//   {
//     valor = 0;
//   }
//   Serial.println("el valor es ");
//   Serial.println(valor);
  
//   delay(5000);

//   valor += 25;
//   analogWrite(outputPin, valor);
// }

// //Pines
// byte pinRed=2;
// byte pinOrange = 3;
// byte pinWhite=4;
// byte lastRed=1;
// byte lastOrange=1;
// byte lastWhite=1;

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
    PWM(byte pin){
      pinMode(pinNumber = pin, OUTPUT);
      value = 200;
    }
    
    virtual byte speedUp(){
      if (value + 20 > 100){
        Log("No se aumenta por estar al máximo. PWM: " + (String)value);
        updatePWM(value);
        return value;
      }
      return value+=20;
    }

    virtual byte speedDown(){
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

    virtual byte getValue(){
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
    Button(byte pin, void (*fn)(ArduinoComponent), ArduinoComponent component){
      lastRead = 1;
      function = fn;
      arduinoComponent = component;
      pinMode(pinNumber = pin, INPUT_PULLUP);
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
      function(arduinoComponent);
    }

  private: 
    byte pinNumber;
    byte lastRead; 
    ArduinoComponent arduinoComponent;
    unsigned long lastPush;
    void (*function)(ArduinoComponent);
};



//Button Press Functions
void imprimir(){
  Serial.println("impresion");
};

void speedUp(PWM pwm){
  Log((String)"Se aumenta velocidad: " + pwm.speedUp());
};

void speedDown(PWM pwm){
  Log((String)"Se disminuye velocidad: " + pwm.speedDown());
};



//Initialization
PWM pwm(6);
Button redButton(2, &speedUp, pwm); 
Button orangeButton(3, &speedDown, pwm);
Button whiteButton(4,&imprimir, pwm);


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

//Viejo
// void loop() {
//   checkPressButton(pinRed, lastRed);
//   checkPressButton(pinOrange, lastOrange);
//   checkPressButton(pinWhite, lastWhite);
// }

// void checkPressButton(byte pin, byte &lastRead) {
//   byte read = digitalRead(pin);
//   if(lastRead != read && read == 0 ){
//     Serial.println("last pin " + (String)pin + " : " + (String)lastRead);
//     Serial.println("read: " + (String)read);
//   }
//   lastRead = read;
// }