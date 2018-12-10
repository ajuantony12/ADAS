#define BAUDRATE 9600


byte inData = 0;
volatile uint16_t counter = 0;

ISR(USART1_RX_vect) {
  counter++;
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(BAUDRATE, SERIAL_8E1);

  sei();
}

void loop() {
  Serial.println(counter);
}

