#include "../Libraries/RF24L01/RF24L01.hpp"
#include "../Libraries/APA102/APA102.hpp"


int main() {
    //Kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;


    //Pins for the nrf24l01
    auto IRQ = hwlib::target::pin_in(hwlib::target::pins::d0);
    auto clk = hwlib::target::pin_out(hwlib::target::pins::d3);
    auto mosi = hwlib::target::pin_out(hwlib::target::pins::d2);
    auto miso = hwlib::target::pin_in(hwlib::target::pins::d4);
    auto RX_TX = hwlib::target::pin_out(hwlib::target::pins::d6);
    auto minion_select = hwlib::target::pin_out(hwlib::target::pins::d8);
    auto led = hwlib::target::pin_out(hwlib::target::pins::d13);

    //SPI-bus for the nrf24L01
    auto spi_bus = hwlib::spi_bus_bit_banged_sclk_mosi_miso(clk, mosi, miso);

    //Pins for the apa102
    auto clk2 = hwlib::target::pin_out(hwlib::target::pins::d10);
    auto mosi2 = hwlib::target::pin_out(hwlib::target::pins::d12);

    //SPI-bus for the APA102
    auto spi_bus2 = hwlib::spi_bus_bit_banged_sclk_mosi_miso(clk2, mosi2, hwlib::pin_in_dummy);
    auto ledstrip = IPASS::APA102(spi_bus2);

    const std::array<uint8_t, 5> address = {0xe7, 0xe7, 0xe7, 0xe7, 0xe7};
    IPASS::RF24L01 chip(spi_bus, RX_TX, minion_select, IRQ, address, address, 0x11, false);

    //Set packagesize of pipe 0 to 5
    chip.change_RX_PW_P(0, 5);

    hwlib::wait_ms(200);

    //Enable RX mode on RF24L01
    chip.start_RX();
    std::array<uint8_t, 5> data_in = {0x00, 0x00, 0x00, 0x00};
    bool random_color = false;
    uint8_t counter = 0;
    for (;;) {

        //check if packet is recieved
        if (chip.packet_recieved()) {
            led.write(1);
            //read rx_data to data in;
            chip.read_rx(data_in);
            if(data_in[4] == 0xff){
                random_color=true;
            }
            else {
                random_color = false;
                counter=0;
                //write recieved data to the APA102
                ledstrip.write({data_in[0], data_in[1], data_in[2]}, data_in[3]);
            }
        }
        if(random_color){
            if(counter%7==0){
                ledstrip.random_colors();
            }
            counter++;
        }
        hwlib::wait_ms(100);
    }
}
