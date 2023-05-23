#include <stm32f0xx_it.h>
#include <stm32f0xx_hal.h>
#include <hardware/config.h>
#include <hardware/ch1115.h>

#define _BUTTON_INTERNAL
#include <sys/button.h>
#undef _BUTTON_INTERNAL

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

void EXTI4_15_IRQHandler(void) {

  GPIO_PinState stat_a = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN_KEY_A);
  GPIO_PinState stat_b = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN_KEY_B);

  if(stat_a != button_list[btnid_a].intr_status) {
    button_list[btnid_a].intr_status = stat_a;
    button_list[btnid_a].intr_changed = true;
    button_changed = true;
    HAL_GPIO_EXTI_IRQHandler(BUTTON_PIN_KEY_A);
  }

  if(stat_b != button_list[btnid_b].intr_status) {
    button_list[btnid_b].intr_status = stat_b;
    button_list[btnid_b].intr_changed = true;
    button_changed = true;
    HAL_GPIO_EXTI_IRQHandler(BUTTON_PIN_KEY_B);
  }
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
