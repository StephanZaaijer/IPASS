#include "RF24L01.hpp"

namespace IPASS {
    [[maybe_unused]] void RF24L01::write(const uint8_t &command, const uint8_t &value) {
        const std::array<uint8_t, 2> spi_data = {command, value};
        bus.transaction(minion_select).write(spi_data);
    }


    [[maybe_unused]] void RF24L01::read(const uint8_t &address, uint8_t &data_out) {
        hwlib::spi_bus::spi_transaction spi_trans = ((hwlib::spi_bus *) (&bus))->transaction(minion_select);
        spi_trans.write(address);
        std::array<uint8_t, 1> data_received = {};
        spi_trans.read(data_received);
        data_out = data_received[0];
        spi_trans.~spi_transaction();
    }

    [[maybe_unused]] uint8_t RF24L01::read(const uint8_t &address) {
        hwlib::spi_bus::spi_transaction spi_trans = ((hwlib::spi_bus *) (&bus))->transaction(minion_select);
        spi_trans.write(address);
        std::array<uint8_t, 1> data_received = {};
        spi_trans.read(data_received);
        spi_trans.~spi_transaction();
        return data_received[0];
    }

    RF24L01::RF24L01(hwlib::spi_bus_bit_banged_sclk_mosi_miso &bus, hwlib::pin_out &CE_Pin,
                     hwlib::pin_out &minion_select, hwlib::pin_in &IRQ, std::array<uint8_t, 5> RX_ADDR_P0, std::array<uint8_t, 5> TX_ADDR,
                     uint8_t channel, bool data_rate, bool CRC, bool CRC_width) :
            bus(bus), minion_select(minion_select), CE_pin(CE_Pin), IRQ(IRQ){
        start_up(RX_ADDR_P0, TX_ADDR, channel, CRC, CRC_width, data_rate);
        uint8_t Value_AW = register_read(REGISTER::SETUP_AW);
        if(Value_AW != 0x01 and Value_AW != 0x02 and Value_AW != 0x03){
            for(;;){
                hwlib::cout << "RF24L01 not connected to SPI-bus";
            }
        }
    }

    [[maybe_unused]] void
    RF24L01::change_ADDR(const uint8_t &register_address, const std::array<uint8_t, 5> &address_value) {
        if (register_address == REGISTER::RX_ADDR_P0 or register_address == REGISTER::RX_ADDR_P1 or
            register_address == REGISTER::TX_ADDR) {
            write(register_address, address_value);
        } else {
            write(register_address, address_value[4]);
        }
    }

    [[maybe_unused]] void RF24L01::change_arc(uint8_t arc) {
        if (arc < 0x10) {
            register_write(REGISTER::SETUP_RETR, ((register_read(REGISTER::SETUP_RETR) & 0xf0) | arc));
        }
    }

    [[maybe_unused]] void RF24L01::change_ard(uint8_t ard) {
        if (ard < 0x10) {
            ard <<= 4;
            register_write(REGISTER::SETUP_RETR, ((register_read(REGISTER::SETUP_RETR) & 0x0f) | ard));
        }
    }

    [[maybe_unused]] void RF24L01::change_aw(uint8_t aw) {
        if (aw == 0x00 or aw > 0x03) {
            hwlib::cout << aw << " is not an allowed value the allowed values for aw are between 0x01 and 0x03";
        } else {
            register_write(REGISTER::SETUP_AW, ((register_read(REGISTER::SETUP_AW) & 0xfc) | aw));
        }
    }

    [[maybe_unused]] void RF24L01::change_rf_pwr(uint8_t rf_pwr) {
        if (rf_pwr <= 0x03) {
            rf_pwr <<= 1;
            register_write(REGISTER::RF_SETUP, ((register_read(REGISTER::RF_SETUP) & 0xf9) | rf_pwr));
        }
    }

    [[maybe_unused]] void RF24L01::change_RX_PW_P(const uint8_t &pipe, const uint8_t &value) {
        if (pipe <= 5 and pipe >= 0) {
            uint8_t address = 0x11 + pipe;
            if (value < 0x20) {
                register_write(address, value);
            }
        }
    }

    [[maybe_unused]] void RF24L01::flush_rx_tx() {
        write_command(COMMAND::FLUSH_RX);
        write_command(COMMAND::FLUSH_TX);
    }

    [[maybe_unused]] bool RF24L01::packet_received() {
        return !setting_read(SETTING::RX_EMPTY);
    }

    [[maybe_unused]] bool RF24L01::packet_send() {
        return !setting_read(SETTING::TX_EMPTY);
    }

    [[maybe_unused]] uint8_t RF24L01::register_read(const uint8_t &address) {
        return read(address);
    }

    [[maybe_unused]] void RF24L01::register_read(const uint8_t &address, uint8_t &data_out) {
        read(address, data_out);
    }

    void RF24L01::register_write(const uint8_t &address, const uint8_t &value) {
        uint8_t command = address | COMMAND::W_REGISTER;
        write(command, value);
    }

    [[maybe_unused]] void RF24L01::reset_arc() {
        register_write(REGISTER::SETUP_RETR, ((register_read(REGISTER::SETUP_RETR) & 0xf0) | 0x03));
    }


    [[maybe_unused]] void RF24L01::reset_ard() {
        register_write(REGISTER::SETUP_RETR, (register_read(REGISTER::SETUP_RETR) & 0x0f));
    }

    [[maybe_unused]] void RF24L01::reset_aw() {
        register_write(REGISTER::SETUP_AW, ((register_read(REGISTER::SETUP_AW) & 0xfc) | 0x03));
    }


    [[maybe_unused]] void RF24L01::reset_rf_pwr() {
        register_write(REGISTER::RF_SETUP, ((register_read(REGISTER::RF_SETUP) & 0xf9) | 0x06)
        );
    }

    [[maybe_unused]] void RF24L01::reset_RX_PW_P(const uint8_t &pipe) {
        uint8_t address = 0x11 + pipe;
        register_write(address, 0);
    }

    [[maybe_unused]] void RF24L01::send_packages() {
        CE_pin.write(true);
        setting_disable(SETTING::PRIM_RX);
        CE_pin.write(false);
    }

    [[maybe_unused]] void RF24L01::set_channel(uint8_t new_channel) {
        if (new_channel < 128) {
            register_write(REGISTER::RF_CH, new_channel);
        } else {
            hwlib::cout << new_channel << "is more than the channel-value limit of 127";
        }
    }

    [[maybe_unused]] void RF24L01::setting_disable(SETTING::Setting registerFunction) {
        register_write(registerFunction.REGISTER_ADDR,
                       (register_read(registerFunction.REGISTER_ADDR) & ~registerFunction.Mask));
    }

    [[maybe_unused]] void RF24L01::setting_enable(SETTING::Setting registerFunction) {
        register_write(registerFunction.REGISTER_ADDR,
                       (register_read(registerFunction.REGISTER_ADDR) | registerFunction.Mask));
    }

    [[maybe_unused]] bool RF24L01::setting_read(SETTING::Setting registerFunction){
        uint8_t RegisterValue = register_read(registerFunction.REGISTER_ADDR);
        RegisterValue&=registerFunction.Mask;
        return RegisterValue==registerFunction.Mask;
    }

    [[maybe_unused]] void RF24L01::setting_reset(SETTING::Setting registerFunction) {
        if (registerFunction.Reset_Value) {
            setting_enable(registerFunction);
        } else {
            setting_disable(registerFunction);
        }
    }

    [[maybe_unused]] void RF24L01::start_RX() {
        setting_enable(SETTING::PWR_UP);
        setting_enable(SETTING::PRIM_RX);
        CE_pin.write(true);
    }

    [[maybe_unused]] void RF24L01::start_up(std::array<uint8_t, 5> RX_ADDR_P0, std::array<uint8_t, 5> TX_ADDR,
                      uint8_t channel, bool CRC, bool CRC_width, bool datarate) {
        change_ADDR(REGISTER::RX_ADDR_P0, RX_ADDR_P0);
        change_ADDR(REGISTER::TX_ADDR, TX_ADDR);
        set_channel(channel);
        if (CRC) {
            setting_enable(SETTING::EN_CRC);
            if (CRC_width) {
                setting_enable(SETTING::CRCO);
            } else {
                setting_disable(SETTING::CRCO);
            }
        } else {
            setting_disable(SETTING::EN_CRC);
        }
        if (datarate) {
            setting_enable(SETTING::RF_DR);
        } else {
            setting_disable(SETTING::RF_DR);
        }
        setting_enable(SETTING::PWR_UP);
    }

    [[maybe_unused]] void RF24L01::stop_RX() {
        setting_disable(SETTING::PRIM_RX);
        CE_pin.write(false);
    }

    [[maybe_unused]] bool RF24L01::test() {
        uint8_t Value_AW = register_read(REGISTER::SETUP_AW);
        if (Value_AW != 0x01 and Value_AW != 0x02 and Value_AW != 0x03) {
            hwlib::cout << "register_read not working";
            return false;
        }
        setting_enable(SETTING::PWR_UP);
        if (not(register_read(REGISTER::STATUS) & 0x02)) {
            hwlib::cout << "register_write not working";
            return false;
        }
        std::array<uint8_t, 4> test_data = {0x90, 0x20, 0x20};
        write_tx(test_data);
        if (register_read(REGISTER::FIFO_STATUS) & 0x10) {
            hwlib::cout << "write_tx not working";
            return false;
        }
        flush_rx_tx();
        if (not(register_read(REGISTER::FIFO_STATUS) & 0x10)) {
            hwlib::cout << "flush_tx not working";
            return false;
        }
        write_tx(test_data);
        send_packages();
        if (not packet_send()) {
            hwlib::cout << "send_packages not working";
            return false;
        }
        return true;
    }

    [[maybe_unused]] void RF24L01::write_command(const uint8_t &command) {
        bus.transaction(minion_select).write(command);
        if(command == COMMAND::ACTIVATE){
            Active = !Active;
        }
    }
}