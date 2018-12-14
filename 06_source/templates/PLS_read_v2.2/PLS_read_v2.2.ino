#define BAUD 9600
#define F_CPU 16000000UL
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)

#define BUF_LEN 500
uint8_t rxBuffer[BUF_LEN];
uint16_t rxMsgLen = 0;
volatile uint16_t rxBufferPointer = 0;
bool rxBufferFull = false;
bool rxBufferRdy = false;


#define PLS_STX 0x02 // start byte
#define PLS_ADR 0x80 // address of PLS


// PLS Telegram
#define MAX_PLS_TGM_LEN 500
struct tgm_struct {
  bool rdy = false;
  uint16_t len;
  uint8_t cmd;
  uint16_t data[MAX_PLS_TGM_LEN];
  uint16_t CRC;
};
tgm_struct pls_tgm;

void setup() {
  Serial.begin(115200);
  uart1_init();
}

void loop() {

  if (rxBufferRdy) {
    Serial.println("Received telegram:");
    for (int n = 0; n < rxMsgLen + 2; n++)
    {
      Serial.print(rxBuffer[n], HEX); Serial.print(" ");
    }
    Serial.println("");

    rxBufferPointer = 0;
    rxMsgLen = 0;
    rxBufferRdy = false;
  }

  if (rxBufferFull)
  {
    Serial.println("rxBuffer is full!");

    rxBufferPointer = 0;
    rxMsgLen = 0;
    rxBufferFull = false;
  }


}


// UART1 interrupt
ISR(USART1_RX_vect)
{
  if ((!rxBufferFull) && (!rxBufferRdy))
  {
    // Read incoming data

    // Write rx data to buffer
    rxBuffer[rxBufferPointer] = UDR1;
    rxBufferPointer++;


    if (rxBufferPointer == 3)
    {
      // Check for start byte and PLS address
      if ((rxBuffer[0] != PLS_STX) && (rxBuffer[1] != PLS_ADR))
      {
        // STX & ADR not in first two bytes => overwrite received data
        rxBufferPointer = 0;
      }
    } else if (rxBufferPointer == 5)
    {
      // Check length of telegram
      rxMsgLen = (rxBuffer[3] << 8) + rxBuffer[2];
    } else if (rxBufferPointer == rxMsgLen + 2) // + 2 for CRC
    {
      // Received full telegram
      rxBufferRdy = true;
    } else if (rxBufferPointer == BUF_LEN)
    {
      // Set flag if buffer is full
      rxBufferFull = true;
    }


  } else {
    // flush data
    (void) UDR1;
  }
}

// Function to initialization of UART1
void uart1_init(void)
{
  // Set baudrate
  UBRR1L = BAUD_PRESCALE;
  UBRR1H = (BAUD_PRESCALE >> 8);

  // Ayncrhonous USART
  bitWrite(UCSR1C, UMSEL00, 0);
  bitWrite(UCSR1C, UMSEL01, 0);

  // Enable even parity
  bitWrite(UCSR1C, UPM10, 0);
  bitWrite(UCSR1C, UPM11, 1);

  // 8-bit mode
  bitWrite(UCSR1C, UCSZ02, 0);
  bitWrite(UCSR1C, UCSZ01, 1);
  bitWrite(UCSR1C, UCSZ00, 1);


  // Enable Receiver and Transmitter
  bitWrite(UCSR1B, RXEN1, 1);
  bitWrite(UCSR1B, TXEN1, 1);

  // Enable receive interrupt
  bitWrite(UCSR1B, RXCIE0, 1);

  sei();
}


// Function to send a char/byte via UART1
int uart1_put(char data)
{
  // wait for free tx buffer
  while ( !( UCSR1A & (1 << UDRE1)));
  // send data
  UDR1 = data;
  return 0;
}

// Function to read UART1 rx buffer
int uart1_get(void)
{
  while ( !(UCSR1A & (1 << RXC1)) );

  // return rx data
  return UDR1;
}

