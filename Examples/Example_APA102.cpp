#include "../Libraries/RF24L01/RF24L01.hpp"
#include "../Libraries/APA102/APA102.hpp"

/// \image html example_HC_SR04_TX_schema.png

// This file is an example to which is used to Transmit and recieve RGB and brightness data from 4 Potentiometers

// The example in this file concists of 2 parts:
// - The first part is the main_tx()-function is an example which:
//     Measures the data from the potentiometers and sends it.
// - The second part is the main_rx()-function is an example which:
//     Retrieves the data and sends it to the APA102


//! [Example_APA102_RX]


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
            // if byte 5 == 0xff set random_color on true
            if(data_in[4] == 0xff){
                random_color=true;
            }
            else {
                //else write first 4 bytes to apa102
                random_color = false;
                counter=0;
                //write recieved data to the APA102
                ledstrip.write({data_in[0], data_in[1], data_in[2]}, data_in[3]);
            }
        }
        // if random_color == true
        if(random_color){
            //change color once every 7 runs
            if(counter%7==0){
                ledstrip.random_colors();
            }
            // increase counter
            counter++;
        }
        hwlib::wait_ms(100);
    }
}

//! [Example_APA102_RX]

//! [Example_APA102_TX]


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
        // If random_color_button pressed write {0, 0, 0, 0, 255}
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

//! [Example_APA102_TX]