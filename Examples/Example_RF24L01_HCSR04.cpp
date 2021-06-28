#include "../Libraries/RF24L01/RF24L01.hpp"
#include "../Libraries/HC_SR04/HC_SR04.hpp"

// This file is an example to which is used to Transmit and recieve distance data from the HC-SR04

// The example in this file concists of 2 parts:
// - The first part is the main_tx()-function is an example which:
//     Measures data using HC-SR04, convert it to 4 uint8_t bytes and sends it.
// - The second part is the main_rx()-function is an example which:
//     Retrieves the data, convert it to a float and print that distance to the console



//! [Example_HC-SR04_RX]


// Function to convert recieved distance of 4 bytes into a float distance
float decode_distance(const std::array <uint8_t, 4> &data){
    int distance_m = data[0]<<8|data[1];
    int distance_cm = data[2]<<8|data[3];
    return (distance_m+distance_cm/100);
}

int main_rx() {
    //Kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;

    //Pins connected to the RF24L01
    auto IRQ = hwlib::target::pin_in(hwlib::target::pins::d0);
    auto clk = hwlib::target::pin_out(hwlib::target::pins::d3);
    auto mosi = hwlib::target::pin_out(hwlib::target::pins::d2);
    auto miso = hwlib::target::pin_in(hwlib::target::pins::d4);
    auto CE = hwlib::target::pin_out(hwlib::target::pins::d6);
    auto minion_select = hwlib::target::pin_out(hwlib::target::pins::d8);

    //SPI-bus to send data to the RF24L01
    auto spi_bus = hwlib::spi_bus_bit_banged_sclk_mosi_miso(clk, mosi, miso);

    //const std::array<uint8_t, 5> containing the Address of the RF24L01
    const std::array<uint8_t, 5> address = {0xe7, 0xe7, 0xe7, 0xe7, 0xe7};
    IPASS::RF24L01 chip(spi_bus, CE, minion_select, IRQ, address, address, 0x11, false);

    //Set packagesize of pipe 0 to 4
    chip.change_RX_PW_P(0, 4);

    //Set datarate to 1Mbps
    chip.setting_disable(IPASS::RF24L01::SETTING::RF_DR);

    hwlib::wait_ms(200);

    //Enable RX mode on RF24L01
    chip.start_RX();
    std::array<uint8_t, 4> data_in = {0x00, 0x00, 0x00, 0x00};

    for (;;) {

        //check if packet is recieved
        if (chip.packet_recieved()) {

            //read rx_data to data in;
            chip.read_rx(data_in);

            hwlib::cout << "distance: " << decode_distance(data_in) << " meter" <<'\n';
        }
        hwlib::wait_ms(1000);
    }
}

//! [Example_HC-SR04_RX]


//! [Example_HC-SR04_TX]


// Function to convert float distance into a array of 4 uint8_t elements
std::array <uint8_t, 4> send_distance(SR04 &sensor){
    std::array <uint8_t, 4> data = {};
    float distance = sensor.get_distance();
    int distance_m = distance;
    int distance_cm = (distance - distance_m)*100;
    data[0]=(distance_m>>8);
    data[1]=(distance_m&0xff);
    data[2]=(distance_cm>>8);
    data[3]=(distance_cm&0xff);
    return data;
}

int main(){
    //Kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;

    //Pins connected to the RF24L01
    auto IRQ = hwlib::target::pin_in(hwlib::target::pins::d0);
    auto clk = hwlib::target::pin_out(hwlib::target::pins::d3);
    auto mosi = hwlib::target::pin_out(hwlib::target::pins::d2);
    auto miso = hwlib::target::pin_in(hwlib::target::pins::d4);
    auto RX_TX = hwlib::target::pin_out(hwlib::target::pins::d6);
    auto minion_select = hwlib::target::pin_out(hwlib::target::pins::d8);

    //SPI-bus to send data to the RF24L01
    auto spi_bus = hwlib::spi_bus_bit_banged_sclk_mosi_miso(clk, mosi, miso);

    //const std::array<uint8_t, 5> containing the Address of the RF24L01
    const std::array<uint8_t, 5> address = {0xe7, 0xe7, 0xe7, 0xe7, 0xe7};
    IPASS::RF24L01 chip(spi_bus, RX_TX, minion_select, IRQ, address, address, 0x11, false);

    //pins connected to the HC-SR04
    auto trigger = hwlib::target::pin_out(hwlib::target::pins::d20);
    auto echo = hwlib::target::pin_in(hwlib::target::pins::d22);
    auto distance_sensor = SR04(trigger, echo);

    //retrieve distance and convert that to sendable data
    std::array<uint8_t, 4> data = send_distance(distance_sensor, chip);
    for(;;){
        //write the data to the transmit-data register
        chip.write_tx(data);
        //Send data from the transmit-data register
        chip.send_packages();
        hwlib::wait_ms(250);
    }
}

//! [Example_HC-SR04_TX]
