#include "../Libraries/RF24L01/RF24L01.hpp"

int main() {
    //Kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;

    //Pins for the RF24L01
    auto clk = hwlib::target::pin_out(hwlib::target::pins::d3);
    auto mosi = hwlib::target::pin_out(hwlib::target::pins::d2);
    auto miso = hwlib::target::pin_in(hwlib::target::pins::d4);
    auto RX_TX = hwlib::target::pin_out(hwlib::target::pins::d6);
    auto minion_select = hwlib::target::pin_out(hwlib::target::pins::d8);

    //SPI-bus for the RF24L01
    auto spi_bus = hwlib::spi_bus_bit_banged_sclk_mosi_miso(clk, mosi, miso);

    //std::array containing the ADDRESS
    const std::array<uint8_t, 5> address = {0xe7, 0xe7, 0xe7, 0xe7, 0xe7};
    IPASS::RF24L01 chip(spi_bus, RX_TX, minion_select, address, address, 0x11);

    //Set datarate to 1Mbps
    chip.setting_disable(IPASS::RF24L01::SETTING::RF_DR);
    hwlib::wait_ms(100);

    hwlib::cout << "CD: " << chip.register_read(IPASS::RF24L01::REGISTER::CD);
    hwlib::cout << '\n'<< chip.test() << '\n';
}
