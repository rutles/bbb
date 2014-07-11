// Arduino sketch
// 7segments LED driver with i2c slave

#include <Wire.h>
#define I2C_ADRS 0x69

#define FONT7_0 0b10101111
#define FONT7_1 0b10100000
#define FONT7_2 0b10011101
#define FONT7_3 0b10110101
#define FONT7_4 0b10110010
#define FONT7_5 0b00110111
#define FONT7_6 0b00111111
#define FONT7_7 0b10100001
#define FONT7_8 0b10111111
#define FONT7_9 0b10110111

#define FONT7_E 0b00011111
#define FONT7_r 0b00011000
#define FONT7_o 0b00111100

#define FONT7_M 0b00010000
#define FONT7_P 0b01000000
#define FONT7_X 0b00000000

const byte font7[10] = {
  FONT7_0, FONT7_1, FONT7_2, FONT7_3, FONT7_4,
  FONT7_5, FONT7_6, FONT7_7, FONT7_8, FONT7_9
};
volatile byte ptn[4];

volatile union{
  byte b[4];
  float f;
} buf;

ISR(TIMER2_OVF_vect){
  static byte dig;

  PORTB |= 0x0f;
  PORTD = ptn[dig];
  digitalWrite(11 - dig, LOW);
  dig++;
  dig &= 0x3;
}

void setup(){
  Wire.begin(I2C_ADRS);
  Wire.onReceive(receive);
  Wire.onRequest(send);

  DDRB |= 0x0f;
  DDRD |= 0xff;
  bitSet(TIMSK2, TOIE2);

  putf(0);
}

void receive(int count){
  buf.b[0] = Wire.read();
  buf.b[1] = Wire.read();
  buf.b[2] = Wire.read();
  buf.b[3] = Wire.read();

  while(Wire.available())
    Wire.read();
}

void send(){
  Wire.write((const uint8_t *)buf.b, 4);
}

void putf(float f){
  byte i;
  int n;
  byte len;
  char ratio;
  byte unsupress;
  byte minus;
  const word pow10[] = {
    1, 10, 100, 1000, 10000
  };

  if(f > 9999 || f < -999){
    ptn[3] = FONT7_E;
    ptn[2] = FONT7_r;
    ptn[1] = FONT7_o;
    ptn[0] = FONT7_r;
    return;
  }

  unsupress =(f < 1 && f > -1);
  minus = (f < 0);

  n = f;
  len = 0;
  while(n){
    n /= 10;
    len++;
  }

  ratio = 4 - (len + unsupress + minus);
  f *= pow10[ratio];
  n = f;
  if(n == 0){
    ptn[3] = FONT7_X;
    ptn[2] = FONT7_X;
    ptn[1] = FONT7_X;
    ptn[0] = FONT7_0;
    return;
  }

  while((n % 10) == 0 && ratio > 0){
    n /= 10;
    ratio--;
  }
  
  if(minus)
    n = -n;

  for(i = 0; i < (len + ratio) ; i++){
    ptn[i] = font7[n % 10];
    n /= 10;
  }

  if(unsupress)
    ptn[i++] = FONT7_0;

  if(minus)
    ptn[i++] = FONT7_M;

  for(; i < 4; i++)
    ptn[i] = FONT7_X;

  if(ratio > 0)
    ptn[ratio] |= FONT7_P;
}

void loop(){
  putf(buf.f);
}
