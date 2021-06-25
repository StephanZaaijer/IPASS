#include "APA102.hpp"
namespace IPASS {
    APA102::APA102(hwlib::spi_bus_bit_banged_sclk_mosi_miso &SPI_bus, uint8_t amount_of_leds) :
            SPI_bus(SPI_bus),
            amount_of_leds(amount_of_leds){}

    void APA102::write(color kleur, uint8_t brightness) {
        auto transaction = SPI_bus.transaction(hwlib::pin_out_dummy);
        std::array<uint8_t, 40> data = {0x00, 0x00, 0x00, 0x00};
        brightness >>=3;
        for (unsigned int i = 0; i < amount_of_leds; i++) {
            data[4 + i * 4] = (brightness | 0xe0);
            data[4 + i * 4 + 1] = kleur.blue;
            data[4 + i * 4 + 2] = kleur.green;
            data[4 + i * 4 + 3] = kleur.red;

        }
        data[4 + amount_of_leds * 4] = 0xFF;
        data[4 + amount_of_leds * 4 + 1] = 0xFF;
        data[4 + amount_of_leds * 4 + 2] = 0xFF;
        data[4 + amount_of_leds * 4 + 3] = 0xFF;
        transaction.write(data);
    }

    void APA102::random_colors() {
        color Temp = {static_cast<uint8_t>(hwlib::rand() & 0xFF), static_cast<uint8_t>((hwlib::rand() & 0xFF00) >> 8), static_cast<uint8_t>((hwlib::rand() & 0xFF0000) >> 16)};
        write(Temp, static_cast<uint8_t>(hwlib::rand() & 0xFF));
    }
}