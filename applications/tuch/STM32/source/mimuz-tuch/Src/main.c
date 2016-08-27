//
// mi:muz:tuch#1 STM32 Version (ver.0.2 or later)
//
//  (CC at)2016 by D.F.Mac. @TripArts Music
//
//
// PIN Configuration
//
// PA0 : CH#1 Sensor (OUT)
// PA1 : CH#1 LED    (OUT)
// PA2 : CH#1 Sensor (IN)
// PA3 : CH#2 Sensor (OUT)
// PA4 : CH#2 LED    (OUT)
// PA5 : CH#2 Sensor (IN)
// PA6 : CH#3 Sensor (OUT)
// PA7 : CH#3 LED    (OUT)
// PB0 : CH#3 Sensor (IN)
// PB1 : Calibration Button (IN)
// PA8 : CH#4 Sensor (OUT)
// PA9 : CH#4 LED    (OUT)
// PA10: CH#4 Sensor (IN)
// PA11: USB D-
// PA12: USB D+
// PA13: SWDIO
// PA14: SWCLK
// PA15: CH#5 Sensor (OUT)
// PB3 : CH#5 LED    (OUT)
// PB4 : CH#5 Sensor (IN)
// PB5 : CH#6 Sensor (OUT)
// PB6 : CH#6 LED    (OUT)
// PB7 : CH#6 Sensor (IN)
// PB8 : BOOT0
// PF0 : USB DATA LED (OUT)
// PF1 : N/A

#include "stm32f0xx_hal.h"
#include "usb_device.h"
#include "usbd_midi_if.h"
#include "eeprom.h"
#include "main.h"


// MIDI
uint16_t ledTime;

void led_on(void){
  ledTime = LEDTIMEMAX;
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_SET);
}
void led_off(void){
  ledTime = 0;
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_RESET);
}

static void onNoteOn(uint8_t ch, uint8_t note, uint8_t vel){
  led_on();
}

static void onNoteOff(uint8_t ch, uint8_t note, uint8_t vel){
  led_off();
}

int old_pin = GPIO_PIN_RESET;

// EEPROM
uint16_t VirtAddVarTab[NB_OF_VAR] = {
		0x1000, 0x1001, 0x1002, 0x1003,
		0x1004, 0x1005, 0x1006, 0x1007
};

uint16_t VarDataTab[NB_OF_VAR] = {0, 0, 0, 0, 0, 0, 0, 0};

////////////////
// touch Sensor
///////////////
int val[] = {0,0,0,0,0,0};
int comp[] = {-1,-1,-1,-1,-1,-1};
uint8_t notes[] = {24,20,21,22,23,25};
uint8_t onoff[] = {0,0,0,0,0,0};
uint8_t seq[] = {0,0,0,0,0,0};

uint8_t mode = 0; // 0: none 1: write 2: read

static void onCtlChange(uint8_t ch, uint8_t num, uint8_t value){
  uint8_t func;

  led_on();
  if(ch == 15){
    func = num - 110;
    if(func == 9){        // 119
      mode = value;
    }else if(func < 6){  // Preset-number 110-115
      if(mode == 1){
    	EE_WriteVariable(VirtAddVarTab[func],value);
        notes[func] = value;
      }else{  // mode == 2
        sendCtlChange(15,119,1);
        processMidiMessage();
        sendCtlChange(15,num,notes[func]);
        processMidiMessage();
      }
      mode = 0;
    }
  }
}

void loadNums(){
  uint8_t cnt;
  uint8_t cntmax = TOUCH_CHANNELS; // note numbers
  uint16_t chk;
  chk = EE_ReadVariable(VirtAddVarTab[7],&VarDataTab[7]);
  if(chk == 0){
    for(cnt = 0;cnt < cntmax;cnt ++){
   	  EE_ReadVariable(VirtAddVarTab[cnt],&VarDataTab[cnt]);
   	  notes[cnt] = (uint8_t)VarDataTab[cnt];
	}
  }else{
    for(cnt = 0;cnt < cntmax;cnt ++){
      EE_WriteVariable(VirtAddVarTab[cnt],(uint16_t)(notes[cnt]));
    }
    EE_WriteVariable(VirtAddVarTab[7],8);
  }
}

// Tuch Sensor GPIO Table
TTouchGPIO ttouchGpio[TOUCH_CHANNELS]={
  {GPIOA, GPIOA, GPIOA, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2}, // PA0, PA1, PA2 (CH#1)
  {GPIOA, GPIOA, GPIOA, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5}, // PA3, PA4, PA5 (CH#2)
  {GPIOA, GPIOA, GPIOB, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_0}, // PA6, PA7, PB0 (CH#3)
  {GPIOA, GPIOA, GPIOA, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10},// PA8, PA9, PA10(CH#4)
  {GPIOA, GPIOB, GPIOB, GPIO_PIN_15,GPIO_PIN_3, GPIO_PIN_4}, // PA15,PB3, PB4 (CH#5)
  {GPIOB, GPIOB, GPIOB, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7}  // PB5, PB6, PB7 (CH#6)
};

void sensorLEDOn(int ch){
  HAL_GPIO_WritePin(ttouchGpio[ch].pLEDGPIOPort, ttouchGpio[ch].LedPin, GPIO_PIN_SET);
}

void sensorLEDOff(int ch){
  HAL_GPIO_WritePin(ttouchGpio[ch].pLEDGPIOPort, ttouchGpio[ch].LedPin, GPIO_PIN_RESET);
}

void sensorPinOut(int ch){
  HAL_GPIO_WritePin(ttouchGpio[ch].pOutGPIOPort, ttouchGpio[ch].OutPin, GPIO_PIN_SET);
}

void sensorPinClear(int ch){
  HAL_GPIO_WritePin(ttouchGpio[ch].pOutGPIOPort, ttouchGpio[ch].OutPin, GPIO_PIN_RESET);
}

int sensorPinCheck(int ch){
  int pin = GPIO_PIN_RESET;
  pin = HAL_GPIO_ReadPin(ttouchGpio[ch].pInGPIOPort,ttouchGpio[ch].InPin);
  return(pin);
}

void sensorMeasure(int ch){
  int cnt;

  cnt = 0;
  sensorPinOut(ch);
  while(1){
    if(sensorPinCheck(ch) !=GPIO_PIN_RESET){
      break;
    }
    cnt ++;
  }
  sensorPinClear(ch);
  val[ch] +=  (cnt - val[ch])/4;
}

void sensorLedBlink(){
  int cnt;
  int ledno = 0;

  for(cnt = 0;cnt < 8;cnt ++){
    if((cnt%2) == 0){
      ledno = 0;
    }else{
      ledno = 5;
    }
    sensorLEDOn(ledno);
    HAL_Delay(50);
    sensorLEDOff(ledno);
    HAL_Delay(50);
  }
}

void sensorCalibration(void){
  int ch;
  int cnt;
  int calc;

  sensorLedBlink();

  for(ch = 0;ch < TOUCH_CHANNELS; ch ++){
	val[ch] = 0;
  }
  for(cnt = 0;cnt < 128;cnt ++){
    for(ch = 0;ch < TOUCH_CHANNELS; ch ++){
  	  sensorMeasure(ch);
  	  calc = SENSOR_MARGIN_DIV / (val[ch] + SENSOR_MARGIN_DIV);
  	  if(calc < 16){
  		 calc = 16;
  	  }
      comp[ch] = val[ch] + calc;
	}
	HAL_Delay(WAIT_TIME_MS);
  }
  HAL_Delay(500);

  sensorLedBlink();
}

int old_button = GPIO_PIN_RESET;

int checkCalibrationButton(){
  int pin = GPIO_PIN_RESET;
  int res = 0;
  pin = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
  if((pin == GPIO_PIN_SET)&&(pin != old_button)){
	res = 1;
  }
  old_button = pin;
  return(res);

}

void trigNoteOff(){
  if(ledTime > 0){
    ledTime --;
    if(ledTime == 0){
      led_off();
    }
  }
}


int main(void){
  int ch;

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  HAL_FLASH_Unlock();

  EE_Init();
  loadNums();

  // mimuz
  mimuz_init();
  setHdlNoteOff(onNoteOff);
  setHdlNoteOn(onNoteOn);
  setHdlCtlChange(onCtlChange);

  sensorCalibration();

  while (1){

	  // touch Sensor
	  for(ch = 0;ch < TOUCH_CHANNELS; ch ++){
	    sensorMeasure(ch);
	    if(val[ch] > comp[ch]){
	      if(seq[ch] < 5){
	        seq[ch]++;
	        if(seq[ch] > 2){
  	          if(onoff[ch] == 0){
    	  		sensorLEDOn(ch);
	  	        onoff[ch] = 1;
	  	        led_on();
	  	        sendNoteOn(0,notes[ch],100);
	  	  	    sendCtlChange(0,ch,(uint8_t)(val[ch]));
  	          }
	        }
  	      }
	    }else{
	      if(seq[ch] > 0){
	    	seq[ch]--;
	    	if(seq[ch] < 2){
	    	  if(onoff[ch] == 1){
	  		    sensorLEDOff(ch);
	  	  	    onoff[ch] = 0;
	  	  	    led_on();
	  	  	    sendNoteOff(0,notes[ch]);
	  	  	    sendCtlChange(0,ch,(uint8_t)(val[ch]));
	    	  }
	    	}
  	      }
	    }
	    processMidiMessage();
	  }

	  // Calibration
	  if(checkCalibrationButton() != 0){
	    sensorCalibration();
	  }

	  trigNoteOff();
	  HAL_Delay(WAIT_TIME_MS);
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI48;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler(4);
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler(5);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler(6);
  }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


// GPIO PIN Configuration
// PA0 : CH#1 Sensor (OUT)
// PA1 : CH#1 LED    (OUT)
// PA2 : CH#1 Sensor (IN)
// PA3 : CH#2 Sensor (OUT)
// PA4 : CH#2 LED    (OUT)
// PA5 : CH#2 Sensor (IN)
// PA6 : CH#3 Sensor (OUT)
// PA7 : CH#3 LED    (OUT)
// PB0 : CH#3 Sensor (IN)
// PB1 : Calibration Button (IN)
// PA8 : CH#4 Sensor (OUT)
// PA9 : CH#4 LED    (OUT)
// PA10: CH#4 Sensor (IN)
// PA11: USB D-
// PA12: USB D+
// PA13: SWDIO
// PA14: SWCLK
// PA15: CH#5 Sensor (OUT)
// PB3 : CH#5 LED    (OUT)
// PB4 : CH#5 Sensor (IN)
// PB5 : CH#6 Sensor (OUT)
// PB6 : CH#6 LED    (OUT)
// PB7 : CH#6 Sensor (IN)
// PB8 : BOOT0
// PF0 : USB DATA LED (OUT)
// PF1 : N/A

static void MX_GPIO_Init(void){
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4 
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9 
                          |GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : PF0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA3 PA4 
                           PA6 PA7 PA8 PA9 
                           PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4 
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9 
                          |GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA2 PA5 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_5|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB4 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB3 PB5 PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void Error_Handler(int num){
  sendCtlChange(15,4,num); // debug
  led_on();
  while(1){
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line){}
#endif
