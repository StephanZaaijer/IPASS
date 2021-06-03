/*
int main(void) {

    namespace target = hwlib::target;

    auto scl = target::pin_oc(target::pins::scl);
    auto sda = target::pin_oc(target::pins::sda);
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
    auto oled = hwlib::glcd_oled(i2c_bus, 0x3c);

    auto w1 = hwlib::part(
            oled,
            hwlib::xy( 0, 44 ),
            hwlib::xy( 64, 128));

    auto f1 = hwlib::font_default_16x16();
    auto d1 = hwlib::terminal_from( w1, f1 );
    int percentage = 0;
    int rows_printed = 0;
    for (;;) {
        oled.clear();
        for (uint16_t x = 10; x < oled.size.x-25; x++) {
            for (uint16_t y = 10; y < oled.size.y-20; y++){
                oled.write(hwlib::xy(x, y));
            }
            rows_printed++;
            if (x%2) {
                oled.flush();
                percentage = (rows_printed / (1.08));
                d1 << "\f" << percentage << "%" << hwlib::flush;
            }
        }
        for (uint16_t x = oled.size.x-25; x < oled.size.x-10; x++) {
            for (uint16_t y = 20; y < oled.size.y-30; y++){
                oled.write(hwlib::xy(x, y));
            }
            rows_printed++;
            if (x%2) {
                oled.flush();
                percentage = (rows_printed / (1.08));
                d1 << "\f" << percentage << "%" << hwlib::flush;
            }
        }
        hwlib::wait_ms(1000);
        rows_printed=0;


    }
}
*/


#include "RF24L01.hpp"
int main(){
    auto clk  = hwlib::target::pin_out(hwlib::target::pins::d0);
    auto mosi = hwlib::target::pin_out(hwlib::target::pins::d2);
    auto miso = hwlib::target::pin_in(hwlib::target::pins::d4);
    auto RX_TX = hwlib::target::pin_out( hwlib::target::pins::d6);
    auto minion_select = hwlib::target::pin_out( hwlib::target::pins::d6);
    auto spi_bus = hwlib::spi_bus_bit_banged_sclk_mosi_miso(clk, mosi, miso);
    IPASS::RF24L01 test(spi_bus, RX_TX, true, minion_select);
}