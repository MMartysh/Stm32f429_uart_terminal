/******************************************************************************
 * File Name   : MX_Device.h
 * Date        : 27/11/2019 00:26:25
 * Description : STM32Cube MX parameter definitions
 * Note        : This file is generated by STM32CubeMX (DO NOT EDIT!)
 ******************************************************************************/

#ifndef __MX_DEVICE_H
#define __MX_DEVICE_H

/*---------------------------- Clock Configuration ---------------------------*/

#define MX_LSI_VALUE                            32000
#define MX_LSE_VALUE                            32768
#define MX_HSI_VALUE                            16000000
#define MX_HSE_VALUE                            25000000
#define MX_EXTERNAL_CLOCK_VALUE                 12288000
#define MX_PLLCLKFreq_Value                     96000000
#define MX_SYSCLKFreq_VALUE                     16000000
#define MX_HCLKFreq_Value                       16000000
#define MX_FCLKCortexFreq_Value                 16000000
#define MX_CortexFreq_Value                     16000000
#define MX_AHBFreq_Value                        16000000
#define MX_APB1Freq_Value                       16000000
#define MX_APB2Freq_Value                       16000000
#define MX_APB1TimFreq_Value                    16000000
#define MX_APB2TimFreq_Value                    16000000
#define MX_48MHZClocksFreq_Value                48000000
#define MX_EthernetFreq_Value                   16000000
#define MX_LCDTFTFreq_Value                     12250000
#define MX_I2SClocksFreq_Value                  96000000
#define MX_SAI_AClocksFreq_Value                12250000
#define MX_SAI_BClocksFreq_Value                12250000
#define MX_RTCFreq_Value                        32000
#define MX_WatchDogFreq_Value                   32000
#define MX_MCO1PinFreq_Value                    16000000
#define MX_MCO2PinFreq_Value                    16000000

/*-------------------------------- DMA        --------------------------------*/

#define MX_DMA                                  1

/* NVIC Configuration */

/* NVIC DMA2_Stream2_IRQn */
#define MX_DMA2_Stream2_IRQn_interruptPremptionPriority 0
#define MX_DMA2_Stream2_IRQn_PriorityGroup      NVIC_PRIORITYGROUP_4
#define MX_DMA2_Stream2_IRQn_Subriority         0

/* NVIC DMA2_Stream7_IRQn */
#define MX_DMA2_Stream7_IRQn_interruptPremptionPriority 0
#define MX_DMA2_Stream7_IRQn_PriorityGroup      NVIC_PRIORITYGROUP_4
#define MX_DMA2_Stream7_IRQn_Subriority         0

/*-------------------------------- SYS        --------------------------------*/

#define MX_SYS                                  1

/* GPIO Configuration */

/*-------------------------------- USART1     --------------------------------*/

#define MX_USART1                               1

#define MX_USART1_VM                            VM_ASYNC

/* GPIO Configuration */

/* Pin PA9 */
#define MX_USART1_TX_GPIO_Speed                 GPIO_SPEED_FREQ_VERY_HIGH
#define MX_USART1_TX_Pin                        PA9
#define MX_USART1_TX_GPIOx                      GPIOA
#define MX_USART1_TX_GPIO_PuPd                  GPIO_PULLUP
#define MX_USART1_TX_GPIO_Pin                   GPIO_PIN_9
#define MX_USART1_TX_GPIO_AF                    GPIO_AF7_USART1
#define MX_USART1_TX_GPIO_Mode                  GPIO_MODE_AF_PP

/* Pin PA10 */
#define MX_USART1_RX_GPIO_Speed                 GPIO_SPEED_FREQ_VERY_HIGH
#define MX_USART1_RX_Pin                        PA10
#define MX_USART1_RX_GPIOx                      GPIOA
#define MX_USART1_RX_GPIO_PuPd                  GPIO_PULLUP
#define MX_USART1_RX_GPIO_Pin                   GPIO_PIN_10
#define MX_USART1_RX_GPIO_AF                    GPIO_AF7_USART1
#define MX_USART1_RX_GPIO_Mode                  GPIO_MODE_AF_PP

/* DMA Configuration */

/* DMA USART1_TX */
#define MX_USART1_TX_DMA_DMA_Handle             
#define MX_USART1_TX_DMA_Instance               DMA2_Stream7
#define MX_USART1_TX_DMA_FIFOMode               DMA_FIFOMODE_DISABLE
#define MX_USART1_TX_DMA_Priority               DMA_PRIORITY_LOW
#define MX_USART1_TX_DMA_Channel                DMA_CHANNEL_4
#define MX_USART1_TX_DMA_PeriphDataAlignment    DMA_PDATAALIGN_BYTE
#define MX_USART1_TX_DMA_MemDataAlignment       DMA_MDATAALIGN_BYTE
#define MX_USART1_TX_DMA_Mode                   DMA_NORMAL
#define MX_USART1_TX_DMA_Direction              DMA_MEMORY_TO_PERIPH
#define MX_USART1_TX_DMA_PeriphInc              DMA_PINC_DISABLE
#define MX_USART1_TX_DMA_MemInc                 DMA_MINC_ENABLE
#define MX_USART1_TX_DMA_IpInstance             

/* DMA USART1_RX */
#define MX_USART1_RX_DMA_DMA_Handle             
#define MX_USART1_RX_DMA_Instance               DMA2_Stream2
#define MX_USART1_RX_DMA_FIFOMode               DMA_FIFOMODE_DISABLE
#define MX_USART1_RX_DMA_Priority               DMA_PRIORITY_LOW
#define MX_USART1_RX_DMA_Channel                DMA_CHANNEL_4
#define MX_USART1_RX_DMA_PeriphDataAlignment    DMA_PDATAALIGN_BYTE
#define MX_USART1_RX_DMA_MemDataAlignment       DMA_MDATAALIGN_BYTE
#define MX_USART1_RX_DMA_Mode                   DMA_NORMAL
#define MX_USART1_RX_DMA_Direction              DMA_PERIPH_TO_MEMORY
#define MX_USART1_RX_DMA_PeriphInc              DMA_PINC_DISABLE
#define MX_USART1_RX_DMA_MemInc                 DMA_MINC_ENABLE
#define MX_USART1_RX_DMA_IpInstance             

/*-------------------------------- NVIC       --------------------------------*/

#define MX_NVIC                                 1

/*-------------------------------- GPIO       --------------------------------*/

#define MX_GPIO                                 1

/* GPIO Configuration */

#endif  /* __MX_DEVICE_H */

