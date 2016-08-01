
// --------------------------------------------
// Multiple Touch Sensor 
// (CC)2016 by D.F.Mac. @TripArts Music
// 2016.05.23
// --------------------------------------------

// --------------------------------------------
// ATTiny44/84 Pins
//                +-------+
//             VCC|1    14|GND
//           XTAL1|2    13|PA0/ADC0 (SensorOut1)
//           XTAL2|3    12|PA1/ADC1 (SensorOut2)
//           RESET|4    11|PA2/ADC2 (SensorOut3)
//  (LED1)     PB2|5    10|PA3/ADC3 (SensorIn) 
//  (LED2)PA7/ADC7|6     9|PA4/ADC4/SCK (LED3)
//(USB D-)PA6/ADC6|7     8|PA5/ADC5 (BUTTON)
//                +-------+
// --------------------------------------------

/*

###########################################
# ATmega8A
#
# 25:PC2/ADC2/●D16(SIN3)---------------+
# 26:PC3/ADC3/●D17(SIN4)-------------+ |
# 27:PC4/ADC4/SDA/●D18(SIN5)-------+ | |
# 28:PC5/ADC5/SCL/●D19(SIN6)-----+ | | | 
# 29:PC6/RESET       ----------+ | | | |
# 30:PD0/RXD/●D0(LED1)-------+ | | | | |
# 31:PD1/TXD/●D1(LED2)-----+ | | | | | |
# 32:PD2/INT0/●D2(LED3)--+ | | | | | | |
#                      +-+-+-+-+-+-+-+-+-+
# 01:PD3/INT1/●D3(LED4)| ●               | 24:PC1/ADC1/●D15(SIN2)
# 02:PD4/XCK/T0/●D4(LED5)                | 23:PC0/ADC0/●D14(SIN1)
# 03:GND               |                 | 22:ADC7
# 04:VCC               |                 | 21:GND
# 05:GND               |                 | 20:AREF
# 06:VCC               |                 | 19:ADC6
# 07:PB6/XTAL1         |                 | 18:AVCC
# 08:PB7/XTAL2         |                 | 17:PB5/SCK/●D13(LED6)
#                      +-+-+-+-+-+-+-+-+-+
# 09:PD5/●D5(LED6)   ----+ | | | | | | |
# 10:PD6/●D6(BUTTON) ------+ | | | | | |
# 11:PD7/●D7         --------+ | | | | |
# 12:PB0/●D8(SOT1)   ----------+ | | | |
# 13:PB1/OC1A/●D9(SOT2)----------+ | | | 
# 14:PB2/SS/OC1B/●D10(SOT3)--------+ | |
# 15:PB3/MOSI/OC2/●D11(SOT4)---------+ |
# 16:PB4/MISO/●D12(SOT5)---------------+
#
###########################################

*/

#define SENSOR_NUM 6

// LED Macro
#define LED1_PIN PD0
#define LED2_PIN PD1
#define LED3_PIN PD2
#define LED4_PIN PD3
#define LED5_PIN PD4
#define LED6_PIN PD5
#define LED_PORT PORTD
#define LED_DDR  DDRD

void Led_Init(){
//  LED_DDR |= ((1 << LED1_PIN)|(1 << LED2_PIN)|(1 << LED3_PIN)|(1 << LED4_PIN)|(1 << LED5_PIN)|(1 << LED6_PIN));
}

void Led_On(uint8_t sensor){
  LED_DDR |= (1 << sensor);
  LED_PORT |=  (1 << sensor);
}

void Led_Off(uint8_t sensor){
  LED_PORT &=  ~(1 << sensor);
  LED_DDR &= ~(1 << sensor);
}

// Touch Sensor
#define SENSOR1_OUT_PIN  PB0
#define SENSOR2_OUT_PIN  PB1
#define SENSOR3_OUT_PIN  PB2
#define SENSOR4_OUT_PIN  PB3
#define SENSOR5_OUT_PIN  PB4
#define SENSOR6_OUT_PIN  PB5
#define SENSOR1_IN_PIN   PC0
#define SENSOR2_IN_PIN   PC1
#define SENSOR3_IN_PIN   PC2
#define SENSOR4_IN_PIN   PC3
#define SENSOR5_IN_PIN   PC4
#define SENSOR6_IN_PIN   PC5
#define SENSOR_OUT_PORT  PORTB
#define SENSOR_OUT_DDR   DDRB
#define SENSOR_IN_DDR    DDRC
#define SENSOR_INPUT     PINC

#define SENSOR_MARGIN   8

void Sensor_Init(){
  SENSOR_OUT_DDR |= ((1 << SENSOR1_OUT_PIN)|(1 << SENSOR2_OUT_PIN)|(1 << SENSOR3_OUT_PIN)|
                     (1 << SENSOR4_OUT_PIN)|(1 << SENSOR5_OUT_PIN)|(1 << SENSOR6_OUT_PIN));
  SENSOR_IN_DDR &= ~((1 << SENSOR1_IN_PIN)|(1 << SENSOR2_IN_PIN)|(1 << SENSOR3_IN_PIN)|
                     (1 << SENSOR4_IN_PIN)|(1 << SENSOR5_IN_PIN)|(1 << SENSOR6_IN_PIN));
}

void Sensor_Out(uint8_t sensor){
  SENSOR_OUT_PORT &= ~((1 << SENSOR1_OUT_PIN)|(1 << SENSOR2_OUT_PIN)|(1 << SENSOR3_OUT_PIN)|
                      (1 << SENSOR4_OUT_PIN)|(1 << SENSOR5_OUT_PIN)|(1 << SENSOR6_OUT_PIN));
  SENSOR_OUT_PORT |= (1 << sensor);
}

void Sensot_Clear(uint8_t sensor){
  SENSOR_OUT_PORT &= ~(1 << sensor);
}

uint8_t Sensor_Check(uint8_t sensor){
  uint8_t p = SENSOR_INPUT;
  return (p & (1 << sensor));
}

// Button
#define BUTTON_PIN   PD6
#define BUTTON_INPUT PIND
#define BUTTON_DDR   DDRD

void Button_Init(){
  BUTTON_DDR &= ~(1 << BUTTON_PIN);
}

uint8_t Button_Check(uint8_t value){
  return (value & (1 << BUTTON_PIN));
}

// Init Port
void InitPorts(){
  Led_Init();
  Sensor_Init();
  Button_Init();
}

// Global
int val[] = {0,0,0,0,0,0};
int comp[] = {-1,-1,-1,-1,-1,-1};

void ledBlink(){
  uint8_t cnt;
  uint8_t ledno = 0;

  for(cnt = 0;cnt < 8;cnt ++){
    if((cnt%2) == 0){
      ledno = 0;
    }else{
      ledno = 5;
    }
    Led_On(ledno);
    delay(50);
    Led_Off(ledno);
    delay(50);
  }
}

void calibration(){
  uint8_t sensor;
  uint8_t cnt;
  ledBlink();

  for(cnt = 0;cnt < 32;cnt ++){
    for(sensor = 0;sensor < SENSOR_NUM; sensor ++){
      measure1(sensor);
      comp[sensor] = val[sensor] + SENSOR_MARGIN;
    }
    delay(10);
  }

  ledBlink();
}

void setup( ) {
  InitPorts();
  calibration();
}

void measure1(uint8_t sensor){
  uint8_t p;
  int cnt;

  cnt = 0;
  Sensor_Out(sensor);
  while(1){
    if(Sensor_Check(sensor) !=0){
      break;
    }
    cnt ++;
  }
  Sensot_Clear(sensor);
  val[sensor] +=  (cnt - val[sensor])/2;
}
  
void loop( ) {
  uint8_t sensor;
  uint8_t p;

  p = BUTTON_INPUT;
  
  // Button Input
  if(Button_Check(p) !=0){
    calibration();
  }
  
  for(sensor = 0;sensor < SENSOR_NUM; sensor ++){
    measure1(sensor);
    if(val[sensor] > comp[sensor]){
      Led_On(sensor);
    }else{
      Led_Off(sensor);
    }
  }
  delay(10);  
}


