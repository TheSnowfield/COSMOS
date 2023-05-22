#include <stm32f0xx_it.h>
#include <stm32f0xx_hal.h>
#include <hardware/config.h>
#include <hardware/ch1115.h>

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void) {
  while (1);
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void) {
  while (1);
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void) {
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void) {
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void) {
  HAL_IncTick();

  extern void scheduler_tick_inc();
  scheduler_tick_inc();
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

void DMA1_Channel2_3_IRQHandler(void) {
  HAL_DMA_IRQHandler(&dma_i2c1);
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
  if(hi2c == &i2c1) {
    extern ch1115_display_ready_t __ch1115_display_ready;
    if(__ch1115_display_ready) __ch1115_display_ready();
  }
}

void I2C1_IRQHandler(void) {
  if (i2c1.Instance->ISR & (I2C_FLAG_BERR | I2C_FLAG_ARLO | I2C_FLAG_OVR)) {
    HAL_I2C_ER_IRQHandler(&i2c1);
  } else {
    HAL_I2C_EV_IRQHandler(&i2c1);
  }
}
