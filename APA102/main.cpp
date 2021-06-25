#include "../Libraries/APA102/APA102.hpp"

int main() {
    auto clock_pin = hwlib::target::pin_out(hwlib::target::pins::d11);
    auto data_pin = hwlib::target::pin_out(hwlib::target::pins::d13);
    auto SPI_bus = hwlib::spi_bus_bit_banged_sclk_mosi_miso(clock_pin, data_pin, hwlib::pin_in_dummy);
    auto ledstrip = IPASS::APA102(SPI_bus);
    for(;;)
    {
        ledstrip.write(IPASS::APA102::red);
        hwlib::wait_ms(500);
        ledstrip.write(IPASS::APA102::white);
        hwlib::wait_ms(500);
        ledstrip.write(IPASS::APA102::blue);
        hwlib::wait_ms(500);
    }
}