#include <STM32F10x_Drivers_Lib/inc/gpio.h>
#include <STM32F10x_Drivers_Lib/inc/systick.h>
#include <STM32F10x_Drivers_Lib/inc/rtc.h>
#include <STM32F10x_Drivers_Lib/inc/watchdog.h>
#include <STM32F10x_Drivers_Lib/inc/uart.h>
#include <STM32F10x_Drivers_Lib/inc/utils/clock_setup.h>
#include <project_version.h>

int main()
{
	stm32f10x_driver_lib::ClockSetup::getInstance().inHse8MHzOut72MHz();

	auto& systick = stm32f10x_driver_lib::Systick::getInstance();
	systick.init(72'000'000, 1000); // Configure 1 tick - 1 msec

	stm32f10x_driver_lib::Gpio run(
	    GPIOA,
	    1,
	    {stm32f10x_driver_lib::Gpio::Mode::OUTPUT_PUSH_PULL,
	     stm32f10x_driver_lib::Gpio::Speed::_2mhz});
	run.reset();

	// auto& iwdg = stm32f10x_driver_lib::Watchdog::getInstance();
	// iwdg.init(2000);
	// iwdg.start();

	auto* uart1 = stm32f10x_driver_lib::Uart::getInstance(USART1);
	// Config for USART
	uart1->init({
	    stm32f10x_driver_lib::Uart::WordLength::_8B,
	    stm32f10x_driver_lib::Uart::StopBits::_1B,
	    stm32f10x_driver_lib::Uart::Parity::NO,
	    stm32f10x_driver_lib::Uart::HardwareFlowControl::NONE,
	    stm32f10x_driver_lib::Uart::Mode::TX_RX,
	    115'200,
	    72'000'000,
	});
	uart1->createInterrupt();

	while(true) {
		run.reset();
		systick.delay(1000);
		run.set();
		systick.delay(1000);
	}
}
