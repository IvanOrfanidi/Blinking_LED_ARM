#include <STM32F10x_Drivers_Lib/inc/gpio.h>
#include <STM32F10x_Drivers_Lib/inc/systick.h>
#include <STM32F10x_Drivers_Lib/inc/rtc.h>
#include <STM32F10x_Drivers_Lib/inc/watchdog.h>
#include <STM32F10x_Drivers_Lib/inc/uart.h>

int main()
{
	constexpr stm32f10x_driver_lib::Gpio::Config OUTPUT{
	    stm32f10x_driver_lib::Gpio::Mode::OUTPUT_PUSH_PULL,
	    stm32f10x_driver_lib::Gpio::Speed::_2mhz};
	stm32f10x_driver_lib::Gpio run(GPIOB, 12, OUTPUT);

	auto& systick = stm32f10x_driver_lib::Systick::getInstance();
	systick.init(72'000'000, 1000); // Configure 1 tick - 1 msec

	auto& rtc = stm32f10x_driver_lib::Rtc::getInstance();

	auto& iwdg = stm32f10x_driver_lib::Watchdog::getInstance();
	iwdg.init(1000);
	iwdg.start();

	// Config for USART
	const stm32f10x_driver_lib::Uart::Config config = {
	    stm32f10x_driver_lib::Uart::WordLength::_8B,
	    stm32f10x_driver_lib::Uart::StopBits::_1B,
	    stm32f10x_driver_lib::Uart::Parity::NO,
	    stm32f10x_driver_lib::Uart::HardwareFlowControl::NONE,
	    stm32f10x_driver_lib::Uart::Mode::TX_RX,
	    115'200,
	    72'000'000,
	};

	auto* uart1 = stm32f10x_driver_lib::Uart::getInstance(USART1);
	uart1->init(config);
	uart1->createInterrupt();

	while(true) {
		run.reset();
		for(uint32_t i = 0; i < 100000; ++i) {
			__NOP();
		}

		iwdg.reload();

		run.set();
		for(uint32_t i = 0; i < 100000; ++i) {
			__NOP();
		}
	}
}
