#include "../Libraries/RF24L01/RF24L01.hpp"


int main() {
    //Kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;

    //Pins for the RF24L01
    auto IRQ = hwlib::target::pin_in(hwlib::target::pins::d0);
    auto clk = hwlib::target::pin_out(hwlib::target::pins::d3);
    auto mosi = hwlib::target::pin_out(hwlib::target::pins::d2);
    auto miso = hwlib::target::pin_in(hwlib::target::pins::d4);
    auto RX_TX = hwlib::target::pin_out(hwlib::target::pins::d6);
    auto minion_select = hwlib::target::pin_out(hwlib::target::pins::d8);

    //SPI-bus for the RF24L01
    auto spi_bus = hwlib::spi_bus_bit_banged_sclk_mosi_miso(clk, mosi, miso);

    //ADC-Pins for the RGB and brightness values
    auto red = hwlib::target::pin_adc(hwlib::target::ad_pins::a0);
    auto green = hwlib::target::pin_adc(hwlib::target::ad_pins::a6);
    auto blue = hwlib::target::pin_adc(hwlib::target::ad_pins::a8);
    auto brightness = hwlib::target::pin_adc(hwlib::target::ad_pins::a10);

    //Pins connected to the buttons
    auto Send_value_button = hwlib::target::pin_in(hwlib::target::pins::d52);
    auto Turn_off_button = hwlib::target::pin_in(hwlib::target::pins::d11);
    auto Random_color_button = hwlib::target::pin_in(hwlib::target::pins::d13);

    //std::array containing the ADDRESS
    const std::array<uint8_t, 5> address = {0xe7, 0xe7, 0xe7, 0xe7, 0xe7};
    IPASS::RF24L01 chip(spi_bus, RX_TX, minion_select, IRQ, address, address, 0x11, false);

    std::array<uint8_t, 5> data={};
    for(;;) {
        //If send_value_button pressed read data from potentiometers and send them
        if(Send_value_button.read()){
            data[0]=red.read();
            data[1]=green.read();
            data[2]=blue.read();
            data[3]=brightness.read();
            data[4]=0x00;
            chip.write_tx(data);
            chip.send_packages();
            while(Send_value_button.read()){
                hwlib::wait_us(1);
            };
        }
        //If Turn_off_button pressed write {0,0,0,0}
        if(Turn_off_button.read()){
            data = {0x00,0x00,0x00,0x00, 0x00};
            chip.write_tx(data);
            chip.send_packages();
            while(Turn_off_button.read()){
                hwlib::wait_us(1);
            }
        }
        if(Random_color_button.read()){
            data = {0x00, 0x00, 0x00, 0x00, 0xff};
            chip.write_tx(data);
            chip.send_packages();
            while(Random_color_button.read()){
                hwlib::wait_us(1);
            }
        }
    }
}