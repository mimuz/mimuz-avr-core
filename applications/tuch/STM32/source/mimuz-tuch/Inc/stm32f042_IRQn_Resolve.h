// stm2f042_IRQn_Resolve.h
// For SWFSTM32 Error
// include from stm32f0xx_hal.h

#ifndef __STM32F042_IRQn_RESOLVE_H__
#define __STM32F042_IRQn_RESOLVE_H__

#define   NonMaskableInt_IRQn        ((IRQn_Type)-14)    /*!< 2 Non Maskable Interrupt                                        */
#define   HardFault_IRQn             ((IRQn_Type)-1)     /*!< 3 Cortex-M0 Hard Fault Interrupt                                */
#define   SVC_IRQn                   ((IRQn_Type)-5)     /*!< 11 Cortex-M0 SV Call Interrupt                                  */
#define   PendSV_IRQn                ((IRQn_Type)-2)     /*!< 14 Cortex-M0 Pend SV Interrupt                                  */
#define   SysTick_IRQn               ((IRQn_Type)-1)     /*!< 15 Cortex-M0 System Tick Interrupt                              */
#define   WWDG_IRQn                  ((IRQn_Type)0)      /*!< Window WatchDog Interrupt                               */
#define   PVD_VDDIO2_IRQn            ((IRQn_Type)1)      /*!< PVD & VDDIO2 Interrupt through EXTI Lines 16 and 31             */
#define   RTC_IRQn                   ((IRQn_Type)2)      /*!< RTC Interrupt through EXTI Lines 17, 19 and 20                  */
#define   FLASH_IRQn                 ((IRQn_Type)3)      /*!< FLASH global Interrupt                                          */
#define   RCC_CRS_IRQn               ((IRQn_Type)4)      /*!< RCC & CRS global Interrupt                                      */
#define   EXTI0_1_IRQn               ((IRQn_Type)5)      /*!< EXTI Line 0 and 1 Interrupt                                     */
#define   EXTI2_3_IRQn               ((IRQn_Type)6)      /*!< EXTI Line 2 and 3 Interrupt                                     */
#define   EXTI4_15_IRQn              ((IRQn_Type)7)      /*!< EXTI Line 4 to 15 Interrupt                                     */
#define   TSC_IRQn                   ((IRQn_Type)8)      /*!< Touch Sensing Controller Interrupts                             */
#define   DMA1_Channel1_IRQn         ((IRQn_Type)9)      /*!< DMA1 Channel 1 Interrupt                                        */
#define   DMA1_Channel2_3_IRQn       ((IRQn_Type)10)     /*!< DMA1 Channel 2 and Channel 3 Interrupt                          */
#define   DMA1_Channel4_5_IRQn       ((IRQn_Type)11)     /*!< DMA1 Channel 4 and Channel 5 Interrupt                          */
#define   ADC1_IRQn                  ((IRQn_Type)12)     /*!< ADC1 Interrupt                                                  */
#define   TIM1_BRK_UP_TRG_COM_IRQn   ((IRQn_Type)13)     /*!< TIM1 Break, Update, Trigger and Commutation Interrupt           */
#define   TIM1_CC_IRQn               ((IRQn_Type)14)     /*!< TIM1 Capture Compare Interrupt                                  */
#define   TIM2_IRQn                  ((IRQn_Type)15)     /*!< TIM2 global Interrupt                                           */
#define   TIM3_IRQn                  ((IRQn_Type)16)     /*!< TIM3 global Interrupt                                           */
#define   TIM14_IRQn                 ((IRQn_Type)19)     /*!< TIM14 global Interrupt                                          */
#define   TIM16_IRQn                 ((IRQn_Type)21)     /*!< TIM16 global Interrupt                                          */
#define   TIM17_IRQn                 ((IRQn_Type)22)     /*!< TIM17 global Interrupt                                          */
#define   I2C1_IRQn                  ((IRQn_Type)23)     /*!< I2C1 Event Interrupt & EXTI Line23 Interrupt (I2C1 wakeup)      */
#define   SPI1_IRQn                  ((IRQn_Type)25)     /*!< SPI1 global Interrupt                                           */
#define   SPI2_IRQn                  ((IRQn_Type)26)     /*!< SPI2 global Interrupt                                           */
#define   USART1_IRQn                ((IRQn_Type)27)     /*!< USART1 global Interrupt & EXTI Line25 Interrupt (USART1 wakeup) */
#define   USART2_IRQn                ((IRQn_Type)28)     /*!< USART2 global Interrupt                                         */
#define   CEC_CAN_IRQn               ((IRQn_Type)30)     /*!< CEC and CAN global Interrupts & EXTI Line27 Interrupt           */
#define   USB_IRQn                   ((IRQn_Type)31)      /*!< USB global Interrupt  & EXTI Line18 Interrupt                   */

#endif

