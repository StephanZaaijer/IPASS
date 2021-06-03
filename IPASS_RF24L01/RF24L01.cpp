//
// Created by stephan on 2-6-2021.
//
#include "RF24L01.hpp"

namespace IPASS {
    RF24L01::RF24L01(hwlib::spi_bus_bit_banged_sclk_mosi_miso &bus, hwlib::pin_out &modusPin, bool modus,
                     uint8_t channel, hwlib::pin_out &minion_select)
            : bus(bus), minion_select(minion_select), modus_Pin(modusPin), modus(modus), channel(channel) {
        modus_Pin.write(1);       //make CE pin high
        if (modus) {
            std::array<uint8_t, 5> current_value;
            register_read(0x00, current_value);
            uint8_t current_value2 = current_value[0];
            current_value2 |= 0x03;
            register_write(0x00, current_value2);
        } else {
            std::array<uint8_t, 5> current_value;
            register_read(0x00, current_value);
            uint8_t current_value2 = current_value[0];
            current_value2 &= 0xfd;
            current_value2 |= 0x01;
            register_write(0x00, current_value2);


        }
    }

    void RF24L01::register_write(const uint8_t &address, const uint8_t &value) {
        uint8_t command = address | 0x20;
        hwlib::spi_bus::spi_transaction spi_trans = ((hwlib::spi_bus *) (&bus))->transaction(minion_select);
        spi_trans.write(command);
        spi_trans.write(value);
        spi_trans.~spi_transaction();
    }

    void RF24L01::register_write(const uint8_t &address, const std::array<uint8_t, 5> &value) {
        uint8_t command = address | 0x20;
        hwlib::spi_bus::spi_transaction spi_trans = ((hwlib::spi_bus *) (&bus))->transaction(minion_select);
        spi_trans.write(command);
        std::array<uint8_t, 5> send_value;
        for (unsigned int i = 0; i < 5; i++) {
            send_value[i] = value[4 - i];
        }
        spi_trans.write(send_value);
        spi_trans.~spi_transaction();
    }

    void RF24L01::register_read(const uint8_t &address, std::array<uint8_t, 5> &data_out) {
        hwlib::spi_bus::spi_transaction spi_trans = ((hwlib::spi_bus *) (&bus))->transaction(minion_select);
        spi_trans.write(address);
        spi_trans.read(data_out);
        spi_trans.~spi_transaction();
    }

    void RF24L01::read_rx(std::array<uint8_t, 32> &Data) {
        if (modus) {
            hwlib::spi_bus::spi_transaction spi_trans = ((hwlib::spi_bus *) (&bus))->transaction(minion_select);
            spi_trans.write(0x61);
            std::array<uint8_t, 32> data_recieved;
            spi_trans.read(data_recieved);
            for (unsigned int i = 0; i < 32; i++) {
                Data[i] = data_recieved[31 - i];
            }
            spi_trans.~spi_transaction();
        }
    }

    void RF24L01::write_tx(std::array<uint8_t, 32> &Data) {
        if (!modus) {
            std::array<uint8_t, 32> data_send;
            for (unsigned int i = 0; i < 32; i++) {
                data_send[i] = Data[31 - i];
            }
            hwlib::spi_bus::spi_transaction spi_trans = ((hwlib::spi_bus *) (&bus))->transaction(minion_select);
            spi_trans.write(0xa0);
            spi_trans.write(data_send);
            spi_trans.~spi_transaction();
        }
    }

    void RF24L01::flush_rx_tx() {
        hwlib::spi_bus::spi_transaction spi_trans = ((hwlib::spi_bus *) (&bus))->transaction(minion_select);
        if (modus) {
            spi_trans.write(0xE2);
        } else {
            spi_trans.write(0xE1);
        }
        spi_trans.~spi_transaction();
    }

}