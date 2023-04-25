/*
 * app_gpio_test.c
 *
 *  Created on: 2020Äê6ÔÂ11ÈÕ
 *      Author: User
 */

#include "drv_gpio.h"
#include "drv_uart.h"

#define TEST_GPIO_ENABLE		0

#if TEST_GPIO_ENABLE
void gpio_cb_print(uint8_t pin)
{
	uart_printf("GPIO %d Interrupt\r\n", pin);
}

void app_gpio_output_test_init(void)
{
	int i;

	for (i = 0; i < GPIO_NUM; i++)
	{
		hal_gpio_set_pinmux_function(i, 0);
		hal_gpio_set_direction(i, GPIO_DIRECTION_OUTPUT);
		hal_gpio_set_output(i, 0);
	}
}

void app_gpio_output_test(void)
{
	int i;

	for (i = 0; i < GPIO_NUM; i++)
	{
		hal_gpio_toggle_pin(i);
	}
}

void app_gpio_input_test_init(void)
{
	int i;

	for (i = 0; i < GPIO_NUM; i++)
	{
		hal_gpio_set_pinmux_function(i, 0);
		hal_gpio_set_direction(i, GPIO_DIRECTION_INPUT);
		hal_gpio_pull_up(i);
	}
}

void app_gpio_input_test(void)
{
	uint8_t i;

	for (i = 0; i < GPIO_NUM; i++)
	{
		uart_printf("GPIO[%02d] (%d)\r\n", i, hal_gpio_get_input(i));
	}
}

#define GPIO_TRIGGER_TYPE	GPIO_TRIGGER_RISING_EDGE
//#define GPIO_TRIGGER_TYPE HAL_GPIO_TRIGGER_FALLING_EDGE
//#define GPIO_TRIGGER_TYPE HAL_GPIO_TRIGGER_HIGH_LEVEL
//#define GPIO_TRIGGER_TYPE HAL_GPIO_TRIGGER_LOW_LEVEL
void app_gpio_irq_test(void)
{
	int i;

//	hal_gpio_set_pinmux_function(0, 0);
//	hal_gpio_set_direction(0, GPIO_DIRECTION_OUTPUT);

	gpio_intr_cfg_t cfg = {
		.pin = GPIO_6,
		.isrTrigger = GPIO_TRIGGER_FALLING_EDGE,
		.cb = gpio_cb_print
	};
	for (i = 6; i < 16; i++)
	{
//		hal_gpio_set_pinmux_function(i, 0);
//		hal_gpio_set_direction(i, GPIO_DIRECTION_INPUT);
		cfg.pin = i;
		gpio_cfg_intr(cfg);
	}

	gpio_enable_intr(GPIO_678_INTR);
	gpio_enable_intr(GPIO_9ABCDEF);
}

void app_gpio_test_init(void)
{
//	app_gpio_output_test_init();
	app_gpio_input_test_init();
	app_gpio_irq_test();
}

void app_gpio_test()
{
//	app_gpio_output_test();
//	app_gpio_input_test();
//	hal_gpio_toggle_pin(0);
}
#endif
