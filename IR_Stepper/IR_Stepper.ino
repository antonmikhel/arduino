#define STEPS_IN_A_DAY 86400
#define IR_PIN 0
#define MIN_DISTANCE 10.0f
#define IR_DISTANCE 50.0f
#define STEPPER_PIN_1 9
#define STEPPER_PIN_2 10
#define STEPPER_PIN_3 11
#define STEPPER_PIN_4 12
#define SOUND_SENSOR A5 // the piezo is connected to analog pin 0
#define SOUND_THRESH 20  // threshold value to decide when the detected sound is valid

#define CLOCKWISE true
#define ANTICLOCKWISE false

int stepNumber = 0;
int interactionType = 0; // 0 means no input, 1 means i see movement, 2 means i
// hear sound

void stepOne()
{
  digitalWrite(STEPPER_PIN_1, HIGH);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
}

void stepTwo()
{
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, HIGH);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, LOW);
}

void stepThree()
{
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, HIGH);
  digitalWrite(STEPPER_PIN_4, LOW);
}

void stepFour()
{
  digitalWrite(STEPPER_PIN_1, LOW);
  digitalWrite(STEPPER_PIN_2, LOW);
  digitalWrite(STEPPER_PIN_3, LOW);
  digitalWrite(STEPPER_PIN_4, HIGH);
}

void fullStepClockwise()
{
  switch (stepNumber)
  {
    case 0:
      stepOne();
      break;
    case 1:
      stepTwo();
      break;
    case 2:
      stepThree();
      break;
    case 3:
      stepFour();
      break;
  }
}

void fullStepCounterClockwise()
{
  switch (stepNumber)
  {
    case 0:
      stepFour();
      break;
    case 1:
      stepThree();
      break;
    case 2:
      stepTwo();
      break;
    case 3:
      stepOne();
      break;
  }
}

void oneFullStep(bool dir)
{
  if (dir)
    fullStepClockwise();
  else
    fullStepCounterClockwise();

  stepNumber++;
  if (stepNumber > 3)
    stepNumber = 0;
}

void oneBigStep(bool direction)
{
  // direction:
  // false = Anti-clockwise when axle facing up
  // true = clockwise when axle facing up

  oneFullStep(direction);
  delay(2);
}

void goAntiClockwiseForAWhile(int steps)
{
  for (int i = 0; i < steps; i++)
    oneBigStep(ANTICLOCKWISE);
    delay(100);
}

void setup()
{
  Serial.begin(9600);
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
}

void loop()
{
  float volts = analogRead(IR_PIN) * 0.0048828125;
  float distance = 65 * pow(volts, -1.10);
  int soundReading = analogRead(SOUND_SENSOR);

  int FIVE_SECOND_DELAY = 5000;
  int TWO_SECOND_DELAY = 2000; // in miliseconds;
  int ONE_SECOND_DELAY = 1000;

  interactionType = 0;
  if (distance < IR_DISTANCE && distance > MIN_DISTANCE)
    interactionType = 1;

  if (soundReading > SOUND_THRESH)
    interactionType = 2;

  switch (interactionType)
  {
    case 0:
      for (int i = 0; i < 5; i++)
        oneBigStep(CLOCKWISE);
      break;
    case 1:
      delay(FIVE_SECOND_DELAY);
      break;
    case 2:
      goAntiClockwiseForAWhile(random(20, 50));
      // delay(ONE_SECOND_DELAY);
      break;
  }

  Serial.print(distance);
  Serial.println(" cm");
  // Serial.print("Sound: ");
  // Serial.println(soundReading);

  delay(100);
}
