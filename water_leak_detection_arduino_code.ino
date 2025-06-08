int valve = 6;
int ledPin = 7; // LED pin
byte sensorInterrupt = 0;
byte sensorPin = 3;
byte sensorInterrupt1 = 1;
byte sensorPin1 = 4;
float calibrationFactor = 4.5;
volatile byte pulseCount;
volatile byte pulseCount1;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
float flowRate1;
unsigned int flowMilliLitres1;
unsigned long totalMilliLitres1;
float difference;
unsigned long oldTime;
unsigned long oldTime1;

void setup() {
  Serial.begin(9600);
  pinMode(valve, OUTPUT);
  pinMode(ledPin, OUTPUT); // Set LED pin as output
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);
  pinMode(sensorPin1, INPUT);
  digitalWrite(sensorPin1, HIGH);
  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  oldTime = 0;
  pulseCount1 = 0;
  flowRate1 = 0.0;
  flowMilliLitres1 = 0;
  totalMilliLitres1 = 0;
  oldTime1 = 0;
  difference = 0;
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  attachInterrupt(sensorInterrupt1, pulseCounter1, FALLING);
}

void loop() {
  delay(100);
  if ((millis() - oldTime) > 1000) {
    detachInterrupt(sensorInterrupt);
    detachInterrupt(sensorInterrupt1);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    flowRate1 = ((1000.0 / (millis() - oldTime)) * pulseCount1) / calibrationFactor;
    difference = flowRate - flowRate1;

    if ((int)difference == 1) {
      digitalWrite(ledPin, HIGH); // Turn LED ON if difference is exactly 1
    } else {
      digitalWrite(ledPin, LOW);  // Otherwise turn LED OFF
    }

    if (difference > 2) {
      digitalWrite(valve, HIGH);
      delay(1000);
      digitalWrite(valve, LOW);
    }

    oldTime = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;

    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));
    Serial.print("L/min\t");
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate1));
    Serial.print("L/min\n");
    Serial.print("Difference: ");
    Serial.print(int(difference));
    Serial.print("L/min\n");

    pulseCount = 0;
    pulseCount1 = 0;

    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
    attachInterrupt(sensorInterrupt1, pulseCounter1, FALLING);
  }
}

void pulseCounter() {
  pulseCount++;
}

void pulseCounter1() {
  pulseCount1++;
}