#define BAUD 9600
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)

#define BUF_LEN 370
uint8_t rxBuffer[BUF_LEN];
uint16_t rxBufferPointer = 0;
bool rxBufferFull = false;

void setup() {
  Serial.begin(115200);
  uart1_init();
  interrupts();
}

void loop() {


  if (rxBufferFull) {
    // print received data
    Serial.println("Buffer is full!");
    for (uint16_t n = 0; n < BUF_LEN; n++)
    {
      Serial.println(rxBuffer[n], HEX);
    }
    // reset flag
    rxBufferFull = false;
  }
  

}


ISR(USART1_RX_vect)
{
  if (!rxBufferFull)
  {
    // Write rx data to buffer
    rxBuffer[rxBufferPointer] = UDR1;
    rxBufferPointer++;

    if (rxBufferPointer == BUF_LEN)
    {
      // Set flag if buffer is full
      rxBufferFull = true;
      rxBufferPointer = 0;
    }
  } else {
    // flush data
    (void) UDR1;
  }
}


void uart1_init(void)
{
  // Enable Receiver and Transmitter
  UCSR1B |= (1 << RXEN1) | (1 << TXEN1);
  // Ayncrhonous USART
  UCSR1C &= ~((1 << UMSEL11) | (1 << UMSEL10));
  // Set baudrate
  UBRR1L = BAUD_PRESCALE;
  UBRR1H = (BAUD_PRESCALE >> 8);
  // Enable even parity
  UCSR1C &= ~(1 << UPM10);
  UCSR1C |= ~(1 << UPM11);
  // Enable receive interrupt
  UCSR1B |= (1 << RXCIE0);
}

int uart1_put(char data)
{
  // wait for free tx buffer
  while ( !( UCSR1A & (1 << UDRE1)));
  // send data
  UDR1 = data;
  return 0;
}

int uart1_get(void)
{
  while ( !(UCSR1A & (1 << RXC1)) );

  // return rx data
  return UDR1;
}

