//
// Created by stephan on 2-6-2021.
//

#ifndef IPASS_RF24L01_H
#define IPASS_RF24L01_H

#include "hwlib.hpp"

namespace IPASS {
    class RF24L01 {
    private:
        hwlib::spi_bus_bit_banged_sclk_mosi_miso &bus;
        hwlib::pin_out &minion_select;
        hwlib::pin_out &modus_Pin;
        bool modus; //boolean that is set on setup to indicate rx (1) or tx mode(0);
        uint8_t channel;

        void register_write(const uint8_t &address, const uint8_t &value);

        void register_write(const uint8_t &address, const std::array<uint8_t, 5> &value);

        void register_read(const uint8_t &address, std::array<uint8_t, 5> &data_out);

        void read_rx(std::array<uint8_t, 32> &Data);

        void write_tx(std::array<uint8_t, 32> &Data);

        void flush_rx_tx();


    public:
        RF24L01(hwlib::spi_bus_bit_banged_sclk_mosi_miso &bus, hwlib::pin_out &modusPin, bool modus, uint8_t channel,
                hwlib::pin_out &minion_select = hwlib::pin_out_dummy);
    };
}

#endif //IPASS_RF24L01_H
