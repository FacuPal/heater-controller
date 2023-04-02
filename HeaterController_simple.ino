#define Log Serial.println

byte lastReads[16];
byte pwm;
bool power=false;
unsigned long lastPressed[16];

void checkPressButton(byte pinNumber, void (*fn)()) {
  //TODO: Agregar debouncer
  unsigned long time = millis();
  byte read = digitalRead(pinNumber);
  if(lastReads[pinNumber] != read && read == 0 && time - lastPressed[pinNumber] > 500 ){
    // Log("last pin " + (String)pinNumber + " : " + (String)lastReads[pinNumber]);
    // Log("read: " + (String)read);
    // execute();
    lastPressed[pinNumber] = time; 
    fn();
  }
  lastReads[pinNumber] = read;
}

void updatePWM(byte value){
  byte mapValue = map(value, 0, 100, 0, 255);
  Log("Se va a escribir " + (String)mapValue);
  analogWrite(D1, mapValue);
}

//Button Press Functions
void powerUp(){
  !power ? Log("Se va a prender") : Log("Se va a apagar");
  digitalWrite(D0,power=!power);
};

void speedUp(){
  if (pwm + 20 > 100){
    Log("No se aumenta por estar al máximo. PWM: " + (String)pwm);
    return;
  }
  pwm+=20;
  updatePWM(pwm);
}

void speedDown(){
  if (pwm - 20 < 20){
    Log("No se disminuye por estar al minimo. PWM: " + (String)pwm);
    return;
  }
  pwm-=20;
  updatePWM(pwm);
}


void setup() {
  Serial.begin(9600);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, INPUT_PULLUP);
  pinMode(D4, INPUT_PULLUP);
  Log((String)millis());
  delay(500);
  Log((String)millis());
}

void loop() {
  // put your main code here, to run repeatedly:
  checkPressButton(D2, &powerUp);
  checkPressButton(D3, &speedUp);
  checkPressButton(D4, &speedDown);
  
}
