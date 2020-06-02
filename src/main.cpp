#include "STM32F10x_Drivers_Lib/inc/gpio.h"

int main()
{
	constexpr stm32f10x_driver_lib::Gpio::Config OUTPUT{
	    stm32f10x_driver_lib::Gpio::Mode::OUTPUT_PUSH_PULL,
	    stm32f10x_driver_lib::Gpio::Speed::_2mhz};
	stm32f10x_driver_lib::Gpio run(GPIOB, 12, OUTPUT);

	while(true) {
		run.reset();
		for(uint32_t i = 0; i < 100000; ++i) {
			__NOP();
		}
		run.set();
		for(uint32_t i = 0; i < 100000; ++i) {
			__NOP();
		}
	}
}
