
//======================================================================================================================
/**
 *  @file      RF24L01.hpp
 *  @brief     IPASS-project: Interface for the RF24L01.
 */
//======================================================================================================================
#ifndef IPASS_RF24L01_H
#define IPASS_RF24L01_H

// this file contains Doxygen lines
#ifndef HWLIB_INC_HPP
#define HWLIB_INC_HPP

#include "hwlib.hpp"

#endif //HWLIB_INC_HPP

/**
 * @brief
 * IPASS namespace to prevent name clashes
 */

namespace IPASS {

    /**
     * @brief
     * Class for the RF24L01
     * @details
     * Class that contains all the functions, variables and subclasses for the RF24L01
    */
    class RF24L01 {
    private:
        /**
         * @brief
         * SPI-Bus to read and write to the chip
         */
        hwlib::spi_bus_bit_banged_sclk_mosi_miso &bus;
        /**
         * @brief
         * hwlib::pin_out object which contains the pin connected to CSN pin of the RF24L01
         */
        hwlib::pin_out &minion_select;
        /**
         * @brief
         * hwlib::pin_out object which contains the pin connected to CE pin of the RF24L01
         */
        hwlib::pin_out &CE_pin;

        /**
         * @brief
         * hwlib::pin_in object which contains the pin connected to IRQ pin of the RF24L01
         */
        hwlib::pin_in &IRQ;

        /**
         * boolean that indicates if the SPI-command COMMAND::Activate is active which means that the REGISTER::ACK_PLD, REGISTER::DYNPD, REGISTER::FEATURE are writable
         */
        bool Active = false;

        /**
         * @brief
         * Private read function
         * @details
         * Read function that writes the command and than reads the same amount of bytes as the length of value
         * @tparam amount variable that controls the size of std::array Data
         * @param address uint8_t that contains the SPI-command
         * @param data_out std::array uint8_t of variable size that's read after the SPI-command
         */
        template<size_t amount>
        [[maybe_unused]] void read(const uint8_t &address, std::array<uint8_t, amount> &data_out) {
            hwlib::spi_bus::spi_transaction spi_trans = ((hwlib::spi_bus *) (&bus))->transaction(minion_select);
            spi_trans.write(address);
            std::array<uint8_t, amount> data_received = {};
            spi_trans.read(data_received);
            for (unsigned int i = 0; i < amount; i++) {
                data_out[i] = data_received[amount - i - 1];
            }
            spi_trans.~spi_transaction();
        }

        /**
         * @brief
         * Private read function
         * @details
         * Write function that writes the command and the value
         * @param address uint8_t that contains the register address
         * @return returns the uint8_t that read from the register
         */
        [[maybe_unused]] uint8_t read(const uint8_t &address);

        /**
         * @brief
         * Private read function
         * @details
         * Write function that writes the command and the value
         * @param address uint8_t that contains the register address
         * @param data_out uint8_t that's read after the SPI-command
         */
        [[maybe_unused]] void read(const uint8_t &address, uint8_t &data_out);

        /**
         * @brief
         * Startup function
         * @details
         * startup function that sets the value of RX_ADDR_P0, TX_ADDR, channel and CRC
         * @param RX_ADDR_P0 variable that contains the value for the REGISTER::RX_ADDR_P0 register
         * @param TX_ADDR variable that contains the value for the REGISTER::TX_ADDR register
         * @param channel variable that contains the rf-channel the RF24L01 is going to work on
         * @param CRC boolean that contains if CRC is on or off
         * @param CRC_width boolean that contains if the CRC is 1 or 2 bytes
         * @param data_rate boolean that contains if the data rate is 1 or 2 Mbps
         */
        void start_up(std::array<uint8_t, 5> RX_ADDR_P0, std::array<uint8_t, 5> TX_ADDR, uint8_t channel, bool CRC,
                      bool CRC_width, bool data_rate);

        /**
         * @brief
         * Private write function
         * @details
         * Write function that writes the command and the values of a variable size
         * @tparam amount variable that controls the size of std::array Data
         * @param command uint8_t that contains the SPI-command
         * @param value std::array of uint8_t of variable size
         */
        template<size_t amount>
        [[maybe_unused]] void write(const uint8_t &command, const std::array<uint8_t, amount> &value) {
            std::array<uint8_t, amount + 1> spi_data = {command};
            for (unsigned int index = 1; index < amount + 1; index++) {
                spi_data[index] = value[amount - index];
            }
            bus.transaction(minion_select).write(spi_data);
        }

        /**
         * @brief
         * Private write function
         * @details
         * Write function that writes the command and the value
         * @param command uint8_t that contains the SPI-command
         * @param value uint8_t that's written after the SPI-command
         */
        [[maybe_unused]] void write(const uint8_t &command, const uint8_t &value);

    public:
        /**
         * @brief
         * SUBCLASS COMMAND
         * @details
         * SUBCLASS COMMAND that contains only out of static const uint8_t with the addresses of the RF24L01-registers
         * @warning the commands are just mask and because of that they can not be used without some change so read datasheet before calling these commands
         * you can crash your program by executing command without knowing what is does.
         * */
        class COMMAND {
        public:
            /**
            * @brief
             * const static uint8_t that contains the value of the read_register command of the RF24L01
             */
            [[maybe_unused]] static const uint8_t R_REGISTER;
            /**
             * @brief
             * const static uint8_t that contains the value of the write_register command of the RF24L01
             * @details
             * used to write data to the registers of the RF24L01
             * @note
             * can only be used in standby or power down mode
             */
            [[maybe_unused]] static const uint8_t W_REGISTER;
            /**
             * @brief
             * const static uint8_t that contains the value of the read_receive_payload command of the RF24L01
             * @details
             * Read 1-32 bytes from the RX-payload. It starts at byte 0 and the payload is deleted from FIFO after it was read
             */
            [[maybe_unused]] static const uint8_t R_RX_PAYLOAD;
            /**
             * @brief
             * const static uint8_t that contains the value of the write_transmit_payload command of the RF24L01
             * @details
             * Write 1-32 bytes to the TX-payload
             */
            [[maybe_unused]] static const uint8_t W_TX_PAYLOAD;
            /**
             * @brief
             * const static uint8_t that contains the value of the flush_transmit command of the RF24L01
             * @details
             * Flush the TX-fifo
             */
            [[maybe_unused]] static const uint8_t FLUSH_TX;
            /**
             * @brief
             * const static uint8_t that contains the value of the flush_receive command of the RF24L01
             * @details
             * Flush the RX-fifo
             */
            [[maybe_unused]] static const uint8_t FLUSH_RX;
            /**
             * @brief
             * const static uint8_t that contains the value of the re-use transmit payload command of the RF24L01
             * @details
             * Can be used in TX-mode to retransmit last send package as long as CE is high or W_TX_payload / FLUSH_TX executed
             * @warning
             * Should note be activated or deactivated during packet transmission.
             */
            [[maybe_unused]] static const uint8_t REUSE_TX_PL;
            /**
             * @brief
             * const static uint8_t that contains the value of the activate command of the RF24L01
             * @details
             * Used to activate or deactivate the R_RX_PL_WID, W_ACK_PAYLOAD, W_TX_PAYLOAD_NOACK features
             */
            [[maybe_unused]] static const uint8_t ACTIVATE;
            /**
             * @brief
             * const static uint8_t that contains the value of the R_RX_PL_WID command of the RF24L01
             * @details
             * read the payload width for the next R_RX_PAYLOAD in RX_FIFO
             */
            [[maybe_unused]] static const uint8_t R_RX_PL_WID;
            /**
             * @brief
             * const static uint8_t that contains the value of the W_ACK_PAYLOAD command of the RF24L01
             * @details
             * Write 1-32 bytes to TX_FIFO with acknowledge on a specific pipe
             */
            [[maybe_unused]] static const uint8_t W_ACK_PAYLOAD;
            /**
             * @brief
             * const static uint8_t that contains the value of the W_TX_PAYLOAD_NO_ACK command of the RF24L01
             * @details
             * Write 1-32 bytes to TX_FIFO to be send without a acknowledge on this package
             */
            [[maybe_unused]] static const uint8_t W_TX_PAYLOAD_NO_ACK;
            /**
             * @brief
             * const static uint8_t that contains the value of the NOP command of the RF24L01
             * @details
             * No operation implemented can be used to read the status register
             */
            [[maybe_unused]] static const uint8_t NOP;
        };

        /**
         * @brief
         * SUBCLASS REGISTER
         * @details
         * SUBCLASS REGISTER that contains only out of static const uint8_t with the addresses of the RF24L01-registers
         */
        class REGISTER {
        public:
            /**
            * @brief
            * const static uint8_t that contains the CONFIG-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t CONFIG;
            /**
            * @brief
            * const static uint8_t that contains the EN_AA-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t EN_AA;
            /**
            * @brief
            * const static uint8_t that contains the EN_RXADDR-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t EN_RXADDR;
            /**
            * @brief
            * const static uint8_t that contains the SETUP_AW-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t SETUP_AW;
            /**
            * @brief
            * const static uint8_t that contains the SETUP_RETR-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t SETUP_RETR;
            /**
            * @brief
            * const static uint8_t that contains the RF_CH-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t RF_CH;
            /**
            * @brief
            * const static uint8_t that contains the RF_SETUP-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t RF_SETUP;
            /**
            * @brief
            * const static uint8_t that contains the STATUS-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t STATUS;
            /**
            * @brief
            * const static uint8_t that contains the OBSERVE_TX-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t OBSERVE_TX;
            /**
            * @brief
            * const static uint8_t that contains the CD-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t CD;
            /**
            * @brief
            * const static uint8_t that contains the RX_ADDR_P0-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t RX_ADDR_P0;
            /**
            * @brief
            * const static uint8_t that contains the RX_ADDR_P1-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t RX_ADDR_P1;
            /**
            * @brief
            * const static uint8_t that contains the RX_ADDR_P2-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t RX_ADDR_P2;
            /**
            * @brief
            * const static uint8_t that contains the RX_ADDR_P3-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t RX_ADDR_P3;
            /**
            * @brief
            * const static uint8_t that contains the RX_ADDR_P4-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t RX_ADDR_P4;
            /**
            * @brief
            * const static uint8_t that contains the RX_ADDR_P5-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t RX_ADDR_P5;
            /**
            * @brief
            * const static uint8_t that contains the TX_ADDR-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t TX_ADDR;
            /**
            * @brief
            * const static uint8_t that contains the RX_PW_P0-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t RX_PW_P0;
            /**
            * @brief
            * const static uint8_t that contains the RX_PW_P1-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t RX_PW_P1;
            /**
            * @brief
            * const static uint8_t that contains the RX_PW_P2-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t RX_PW_P2;
            /**
            * @brief
            * const static uint8_t that contains the RX_PW_P3-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t RX_PW_P3;
            /**
            * @brief
            * const static uint8_t that contains the RX_PW_P4-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t RX_PW_P4;
            /**
            * @brief
            * const static uint8_t that contains the RX_PW_P5-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t RX_PW_P5;
            /**
            * @brief
            * const static uint8_t that contains the FIFO_STATUS-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t FIFO_STATUS;
            /**
            * @brief
            * const static uint8_t that contains the DYNPD-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t DYNPD;
            /**
            * @brief
            * const static uint8_t that contains the FEATURE-register address of the RF24L01
            */
            [[maybe_unused]] static const uint8_t FEATURE;

        };

        /**
         * @brief
         * SUBCLASS SETTING
         * @details
         * SUBCLASS SETTING for changing the settings of the RF24L01 chip
         */
        class SETTING {
        public:
            /**
             * @brief
             * Struct Setting
             * @details
             * Struct that contains the name of the register and the mask of the settings
             */
            struct Setting {
                ///const uint8_t that contains the address of the register
                const uint8_t &REGISTER_ADDR;
                ///const uint8_t that contains the mask to identify the bit of the setting
                const uint8_t &Mask;
                ///const boolean that contains the standard value to reset the function
                const bool &Reset_Value;

                /**
                 * @brief
                 * equal operator for struct Setting
                 * @details
                 * operator that checks of the content of two structs is equal
                 */
                bool operator==(const Setting &rhs) const &{
                    return (REGISTER_ADDR == rhs.REGISTER_ADDR and Mask == rhs.Mask);
                }
            };

            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the Mask_RX_DR-setting
             * @details
             * The Mask_RX_DR setting manages if the RX_DR interrupt is reflected on the IRQ-pin
             */
            [[maybe_unused]] static Setting Mask_RX_DR;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the Mask_TX_DS-setting
             * @details
             * The Mask_TX_DS setting manages if the TX_DS interrupt is reflected on the IRQ-pin
             */
            [[maybe_unused]] static Setting Mask_TX_DS;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the Mask_MAX_RT-setting
             * @details
             * The Mask_MAX_RT setting manages if the MAX_RT interrupt is reflected on the IRQ-pin
             */
            [[maybe_unused]] static Setting Mask_MAX_RT;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the EN_CRC-setting
             * @details
             * The EN_CRC setting manages if CRC is on or off.
             * @note
             * Function is forced high if on off the bits in EN_AA is high
             */
            [[maybe_unused]] static Setting EN_CRC;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the CRCO-setting
             * @details
             * The CRCO setting manages if the CRCO is 1 or 2 bytes long
             */
            [[maybe_unused]] static Setting CRCO;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the PWR_UP-setting
             * @details
             * The PWR_UP setting manages if the RF24L01 is in Power up or Power down state
             * - High is Power up
             * - Low is Power down
             */
            [[maybe_unused]] static Setting PWR_UP;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the PRIM_RX-setting
             * @details
             * The PRIM_RX setting controls if the device is in TX or RX mode
             * - High is RX-mode
             * - Low is TX-mode
             */
            [[maybe_unused]] static Setting PRIM_RX;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the ENAA_P5-setting
             * @details
             * The ENAA_P5 setting controls if auto acknowledgement is enabled on pipe 5
             * - High is enabled
             * - Low is disabled
             */
            [[maybe_unused]] static Setting ENAA_P5;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the ENAA_P4-setting
             * @details
             * The ENAA_P4 setting controls if auto acknowledgement is enabled on pipe 4
             * - High is enabled
             * - Low is disabled
             */
            [[maybe_unused]] static Setting ENAA_P4;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the ENAA_P3-setting
             * @details
             * The ENAA_P3 setting controls if auto acknowledgement is enabled on pipe 3
             * - High is enabled
             * - Low is disabled
             */
            [[maybe_unused]] static Setting ENAA_P3;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the ENAA_P2-setting
             * @details
             * The ENAA_P2 setting controls if auto acknowledgement is enabled on pipe 2
             * - High is enabled
             * - Low is disabled
             */
            [[maybe_unused]] static Setting ENAA_P2;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the ENAA_P1-setting
             * @details
             * The ENAA_P1 setting controls if auto acknowledgement is enabled on pipe 1
             * - High is enabled
             * - Low is disabled
             */
            [[maybe_unused]] static Setting ENAA_P1;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the ENAA_P0-setting
             * @details
             * The ENAA_P0 setting controls if auto acknowledgement is enabled on pipe 0
             * - High is enabled
             * - Low is disabled
             */
            [[maybe_unused]] static Setting ENAA_P0;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the ERX_P5-setting
             * @details
             * The ERX_P5 setting controls if pipe 5 is enabled
             */
            [[maybe_unused]] static Setting ERX_P5;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the ERX_P4-setting
             * @details
             * The ERX_P4 setting controls if pipe 4 is enabled
             */
            [[maybe_unused]] static Setting ERX_P4;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the ERX_P3-setting
             * @details
             * The ERX_P3 setting controls if pipe 3 is enabled
             */
            [[maybe_unused]] static Setting ERX_P3;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the ERX_P2-setting
             * @details
             * The ERX_P2 setting controls if pipe 2 is enabled
             */
            [[maybe_unused]] static Setting ERX_P2;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the ERX_P1-setting
             * @details
             * The ERX_P1 setting controls if pipe 1 is enabled
             */
            [[maybe_unused]] static Setting ERX_P1;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the ERX_P0-setting
             * @details
             * The ERX_P0 setting controls if pipe 0 is enabled
             */
            [[maybe_unused]] static Setting ERX_P0;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the PLL_LOCK-setting
             * @details
             * The PLL_LOCK setting is used to Force a PLL lock signal<BR>
             * For more info: <a href="https://nl.wikipedia.org/wiki/Phase-locked_loop">PPL wiki page</a>
             * @warning
             * Only used for testing
             */
            [[maybe_unused]] static Setting PLL_LOCK;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the RF_DR-setting
             * @details
             * The RF_DR setting is used to control the Air Data Rate
             * - High is 2 Mbps
             * - Low is 1 Mbps
             */
            [[maybe_unused]] static Setting RF_DR;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the LNA_HCURR-setting
             * @details
             * The LNA_HCURR setting is used to enable LNA gain<BR>
             * For more info: <a href=https://en.wikipedia.org/wiki/Low-noise_amplifier#Gain> LNA gain info</a>
             */
            [[maybe_unused]] static Setting LNA_HCURR;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the RX_DR-setting
             * @details
             * The RX_DR setting is an Data Ready RX FIFO interrupt. Which is reset when new data arrives
             */
            [[maybe_unused]] static Setting RX_DR;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the TX_DS-setting
             * @details
             * The TX_DS setting is an Data Send TX FIFO interrupt. Which is reset when new data is send.
             * IF AUTO_ACK is activated this bit is only high when the acknowledgement is received
             */
            [[maybe_unused]] static Setting TX_DS;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the MAX_RT-setting
             * @details
             * The MAX_RT setting is an interrupt which is enabled when the maximum number of retries is reached
             */
            [[maybe_unused]] static Setting MAX_RT;

            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the CD-setting
             * @details
             * The CD setting is a setting which is check for a Carried detect
             * if HIGH the device is in a jam situation which means the channel should be changed.
             * if LOW the the device is either out-of-range or jammed by broadband signals like WLAN),
             * the device can stay on the same channel but should perform a dummy write to RF_CH to reset the PLOS_CNT
             * @note
             * Only recommended to use if the TX device does not succeed to get packets through
             */
            [[maybe_unused]] static Setting CD;

            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the TX_REUSE-setting
             * @details
             * The MAX_RT setting is a setting which is used to retransmit the last send package while CE is HIGH
             * The function is enabled with the COMMAND::REUSE_TX_PL command and disabled by a W_TX_PAYLOAD or FLUSH_TX command
             * @note
             * read only
             */
            [[maybe_unused]] static Setting TX_REUSE;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the TX_FULL-setting
             * @details
             * The TX_FULL setting is a setting which is HIGH if the TX FIFO is full
             * @note
             * read only
             */
            [[maybe_unused]] static Setting TX_FULL;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the TX_EMPTY-setting
             * @details
             * The TX_EMPTY setting is a setting which is HIGH if the TX FIFO is empty
             * @note
             * read only
             */
            [[maybe_unused]] static Setting TX_EMPTY;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the RX_FULL-setting
             * @details
             * The RX_FULL setting is a setting which is HIGH if the RX FIFO is full
             * @note
             * read only
             */
            [[maybe_unused]] static Setting RX_FULL;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the RX_EMPTY-setting
             * @details
             * The RX_EMPTY setting is a setting which is HIGH if the RX FIFO is empty
             * @note
             * read only
             */
            [[maybe_unused]] static Setting RX_EMPTY;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the DPL_P5-setting
             * @details
             * The DPL_P5 function enables dynamic payload length on data pipe 5
             * @note
             * Requires:
             * EN_DPL and ENAA_P5 to be HIGH
             * To change this value you first need to write COMMAND::ACTIVATE to the RF24L01
             */
            [[maybe_unused]] static Setting DPL_P5;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the DPL_P4-setting
             * @details
             * The DPL_P4 function enables dynamic payload length on data pipe 4
             * @note
             * Requires:
             * EN_DPL and ENAA_P4 to be HIGH
             * To change this value you first need to write COMMAND::ACTIVATE to the RF24L01
             */
            [[maybe_unused]] static Setting DPL_P4;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the DPL_P3-setting
             * @details
             * The DPL_P3 function enables dynamic payload length on data pipe 3
             * @note
             * Requires:
             * EN_DPL and ENAA_P3 to be HIGH
             * To change this value you first need to write COMMAND::ACTIVATE to the RF24L01
             */
            [[maybe_unused]] static Setting DPL_P3;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the DPL_P2-setting
             * @details
             * The DPL_P2 function enables dynamic payload length on data pipe 2
             * @note
             * Requires:
             * EN_DPL and ENAA_P2 to be HIGH
             * To change this value you first need to write COMMAND::ACTIVATE to the RF24L01
             */
            [[maybe_unused]] static Setting DPL_P2;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the DPL_P1-setting
             * @details
             * The DPL_P1 function enables dynamic payload length on data pipe 1
             * @note
             * Requires:
             * EN_DPL and ENAA_P1 to be HIGH
             * To change this value you first need to write COMMAND::ACTIVATE to the RF24L01
             */
            [[maybe_unused]] static Setting DPL_P1;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the DPL_P0-setting
             * @details
             * The DPL_P0 function enables dynamic payload length on data pipe 0
             * @note
             * Requires:
             * EN_DPL and ENAA_P0 to be HIGH
             * To change this value you first need to write COMMAND::ACTIVATE to the RF24L01
             */
            [[maybe_unused]] static Setting DPL_P0;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the EN_DPL-setting
             * @details
             * The EN_DPL setting enables Dynamic Payload Length
             * @note
             * To change this value you first need to write COMMAND::ACTIVATE to the RF24L01
             */
            [[maybe_unused]] static Setting EN_DPL;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the EN_ACK_PAY-setting
             * @details
             * The EN_ACK_PAY setting enables Payload with acknowledgement
             * @note
             * To change this value you first need to write COMMAND::ACTIVATE to the RF24L01
             */
            [[maybe_unused]] static Setting EN_ACK_PAY;
            /**
             * @brief
             * static Settings that contains the Register_name and the bitmask of the EN_DYN_ACK-setting
             * @details
             * The EN_DYN_ACK enables the W_TX_PAYLOAD_NO_ACK command
             * @note
             * To change this value you first need to write COMMAND::ACTIVATE to the RF24L01
             */
            [[maybe_unused]] static Setting EN_DYN_ACK;
        };

        /**
         * @brief
         * Default constructor for RF24L01
         * @details
         * Default constructor for the RF24L01
         * @param bus hwlib::spi_bus_bit_banged_sclk_mosi_miso object that contains the spi_bus for the RF24L01
         * @param modusPin hwlib::pin_out object that controls the CE-pin of the RF24L01
         * @param minion_select hwlib::pin_out object that controls the Chip select of the RF24L01
         * @param IRQ hwlib::pin_out object that is connected to IRQ-pin of the RF24L01
         * @param RX_ADDR_P0 std::array<uint8_t, 5> that contains the RX-address of pipe 0
         * @param TX_ADDR std::array<uint8_t, 5> that contains the TX-address of the RF24L01
         * @param channel uint8_t which sets the rf-channel of the RF24L01
         * @param data_rate boolean that controls if setup with data rate of 1 or 2 Mbps
         * @param CRC boolean that controls of CRC is enabled or disabled
         * @param CRC_width that controls of CRC is 1 or 2 bytes
         */
        RF24L01(hwlib::spi_bus_bit_banged_sclk_mosi_miso &bus,
                hwlib::pin_out &modusPin,
                hwlib::pin_out &minion_select,
                hwlib::pin_in &IRQ,
                std::array<uint8_t, 5> RX_ADDR_P0,
                std::array<uint8_t, 5> TX_ADDR,
                uint8_t channel = 0x02,
                bool data_rate = true,
                bool CRC = true,
                bool CRC_width = true);

        /**
         * @brief
         * function to change to value in the address-register
         * @details
         * if register is pipe 2 - 5 only last byte is written, first 4 bytes are the 4 most significant bytes of pipe 1
         * @param register_address uint8_t value that contains the address of the register
         * @param address_value std::array<uint8_t, 5> that contains the new address
         */
        [[maybe_unused]] void change_ADDR(const uint8_t &register_address, const std::array<uint8_t, 5> &address_value);

        /**
         * @brief
         * Change the arc value in the SETUP_RETR-register
         * @details
         * Change the arc value to value in parameter arc
         * possible value and meanings are:
         * - '0x00': Re-Transmit disabled
         * - '0x01': Up to 1 Re-Transmit on fail of AA
         * - '0x02': Up to 2 Re-Transmits on fail of AA
         * - '0x03': Up to 3 Re-Transmits on fail of AA
         * - '0x04': Up to 4 Re-Transmits on fail of AA
         * - '0x05': Up to 5 Re-Transmits on fail of AA
         * - '0x06': Up to 6 Re-Transmits on fail of AA
         * - '0x07': Up to 7 Re-Transmits on fail of AA
         * - '0x08': Up to 8 Re-Transmits on fail of AA
         * - '0x09': Up to 9 Re-Transmits on fail of AA
         * - '0x0A': Up to 10 Re-Transmits on fail of AA
         * - '0x0B': Up to 11 Re-Transmits on fail of AA
         * - '0x0C': Up to 12 Re-Transmits on fail of AA
         * - '0x0D': Up to 13 Re-Transmits on fail of AA
         * - '0x0E': Up to 14 Re-Transmits on fail of AA
         * - '0x0F': Up to 15 Re-Transmits on fail of AA
         * @param arc uint8_t that's written as the new value for arc
         */
        [[maybe_unused]] void change_arc(uint8_t arc);

        /**
         * @brief
         * Change the ard value in the SETUP_RETR-register
         * @details
         * Change the ard value to value in parameter ard
         * possible value and meanings are:
         * - '0x00': Wait 250 μS
         * - '0x01': Wait 500 μS
         * - '0x02': Wait 750 μS
         * - '0x03': Wait 1000 μS
         * - '0x04': Wait 1250 μS
         * - '0x05': Wait 1500 μS
         * - '0x06': Wait 1750 μS
         * - '0x07': Wait 2000 μS
         * - '0x08': Wait 2250 μS
         * - '0x09': Wait 2500 μS
         * - '0x0A': Wait 2750 μS
         * - '0x0B': Wait 3000 μS
         * - '0x0C': Wait 3250 μS
         * - '0x0D': Wait 3500 μS
         * - '0x0E': Wait 3750 μS
         * - '0x0F': Wait 4000 μS
         * (Delay defined from end of a transmission to start of next transmission)
         * @param ard uint8_t that's written as the new value for ard
         */
        [[maybe_unused]] void change_ard(uint8_t ard);

        /**
         * @brief
         * Change the aw value in the SETUP_AW-register
         * @details
         * Change the aw value to value in parameter aw
         * possible value and meanings are:
         * - '01': 3 bytes
         * - '10': 4 bytes
         * - '11': 5 bytes
         * the least significant byte is used for the address if address is less than 5 bytes
         * @param aw uint8_t that's written as the new value for aw
         */
        [[maybe_unused]] void change_aw(uint8_t aw);

        /**
         * @brief
         * Change the rf_pwr value in the RF_SETUP-register
         * @details
         * Change the rf_pwr value to value in parameter rf_pwr
         * possible value and meanings are:
         * - '0x00': -18 dBm
         * - '0x01': -12 dBm
         * - '0x02': -6  dBm
         * - '0x03':  0  dBm
         * @param rf_pwr uint8_t that's written as the new value for rf_pwr
         */
        [[maybe_unused]] void change_rf_pwr(uint8_t rf_pwr);

        /**
         * @brief
         * change the amount of bytes in the specified RX_pipes
         * @param pipe value 0-5 that indicates which pipes value is changed
         * @param value value 0-32 which indicates the amount of bytes in RX_payload on the specified pipe
         */
        [[maybe_unused]] void change_RX_PW_P(const uint8_t &pipe, const uint8_t &value);

        /**
         * @brief
         * function flush the RX and TX payload registers
         */
        [[maybe_unused]] void flush_rx_tx();

        /**
         * @brief
         * boolean to check if a packet is received
         * @return boolean value that indicates if there is a packet by reading the RX_EMPTY bit in register FIFO_STATUS
         */
        [[maybe_unused]] bool packet_received();

        /**
         * @brief
         * boolean that checks if a packet is send
         * @return boolean value that indicates if there is a packet has been send by reading the TX_EMPTY bit in register FIFO_STATUS
         */
        [[maybe_unused]] bool packet_send();

        /**
         * @brief
         * Function to read received packages
         * @tparam amount variable that controls the size of std::array Data
         * @param Data std::array uint8_t that read from the RX_PLD register
         * @return returns an std::array of variable size stored in parameter Data
         */
        template<size_t amount>
        [[maybe_unused]] void read_rx(std::array<uint8_t, amount> &Data) {
            read(COMMAND::R_RX_PAYLOAD, Data);
        }

        /**
         * @brief
         * Function to read multiple uint8_t value from an register
         * @details
         * Function to read an variable amount of value from an register
         * @tparam amount variable that controls the size of std::array Data
         * @param address uint8_t that contains the location of the register
         * @param data_out std::array of uint8_t that contains an variable amount of values to read from the register
         * @return the function returns a std::array of variable size stored in parameter data_out
         */
        template<size_t amount>
        [[maybe_unused]] void register_read(const uint8_t &address, std::array<uint8_t, amount> &data_out) {
            read(address, data_out);
        }

        /**
         * @brief
         * Function to read a uint8_t value from an register
         * @details
         * Function to read one uint8_t from an register
         * @param address uint8_t which contains the address of the register
         * @return the function returns a uint8_t
         */
        [[maybe_unused]] uint8_t register_read(const uint8_t &address);

        /**
         * @brief
         * Function to read a uint8_t value from an register
         * @details
         * Function to read one uint8_t from an register
         * @param address uint8_t which contains the address of the register
         * @param data_out uint8_t in which the output of register read is stored
         * @return the function returns a uint8_t stored in parameter data_out
         */
        [[maybe_unused]] void register_read(const uint8_t &address, uint8_t &data_out);


        /**
        * @brief
        * function to write an value to an address
        * @details
        * function to write an uint8_t value to an register
        * @param address uint8_t that contains the location of the register
        * @param value uint8_t that contains the value to write to the register
        */
        [[maybe_unused]] void register_write(const uint8_t &address, const uint8_t &value);

        /**
         * @brief
         * resets the arc value to it's default value '0x03'
         * @details
         * resets the arc value to it's default value '0x03' which means that the retransmit count is 3
         */
        [[maybe_unused]] void reset_arc();


        /**
         * @brief
         * resets the ard value to it's default value '0000'
         * @details
         * resets the ard value to it's default value '0000' which means that the delay is 250 μS
         */
        [[maybe_unused]] void reset_ard();


        /**
         * @brief
         * resets the aw value to it's default value '11'
         * @details
         * resets the aw value to it's default value '11' which means that an address is 5 bytes
         */
        [[maybe_unused]] void reset_aw();

        /**
         * @brief
         * resets the rf_pwr value to it's default value '0x02'
         * @details
         * resets the rf_pwr value to it's default value '0x02' which means that the RF output power in TX mode is 0 dBm
         */
        [[maybe_unused]] void reset_rf_pwr();


        /**
         * @brief
         * resets the amount of bytes in RX_pipe
         * @details
         * change the value in the specified pipe to 0 which means that the pipe is not used
         * @param pipe value 0-5 that indicates which pipes value is changed
         */
        [[maybe_unused]] void reset_RX_PW_P(const uint8_t &pipe);

        /**
         * @brief
         * function that transmits the packages stored in the transmit buffer
         */
        [[maybe_unused]] void send_packages();

        /**
         * @brief
         * function to change the RF-channel of the RF24L01
         * @details
         * Actual RF-frequency is 2400+value[MHz]
         * @param channel the new channel value the maximum value is 128
         */
        [[maybe_unused]] void set_channel(uint8_t channel);

        /**
        * @brief
        * Function to enable a setting
        * @details
        * Function to disable a setting of the RF24L01
        * @param registerFunction SETTING::Setting parameter that contains the register and mask of the setting
        */
        [[maybe_unused]] void setting_disable(SETTING::Setting registerFunction);


        /**
        * @brief
        * Function to enable a setting
        * @details
        * Function to enable a setting of the RF24L01
        * @param registerFunction SETTING::Setting parameter that contains the register and mask of the setting
        */
        [[maybe_unused]] void setting_enable(SETTING::Setting registerFunction);

        /**
         * @brief
         * Function to read the current status if a setting
         * @details
         * Function to read to current status of a setting of the RF24L01
         * @param registerFunction SETTING::Setting parameter that contains the register, mask and the standard value of the setting
         */
        [[maybe_unused]] bool setting_read(SETTING::Setting registerFunction);

        /**
         * @brief
         * Function to reset a setting
         * @details
         * Function to reset a setting of the RF24L01
         * @param registerFunction SETTING::Setting parameter that contains the register, mask and the standard value of the setting
         */
        [[maybe_unused]] void setting_reset(SETTING::Setting registerFunction);

        /**
         * @brief
         * function to start the receiving mode on the RF24L01
         */
        [[maybe_unused]] void start_RX();

        /**
        * @brief
        * function to stop the receiving mode on the RF24L01
        */
        [[maybe_unused]] void stop_RX();


        /**
         * @brief
         * Test function
         * @details
         * Test function that tests reading and writing registers, writing payload, flushing payload and sending payload
         * @return returns true if pass and returns false in case of a fail. If it fails it sends an error message through the console
         */
        [[maybe_unused]] bool test();


        /**
         * @brief
         * Write one single command without value to the RF24L01
         * @param command uint8_t value to be written to the RF24L01
         * @warning the commands are just mask and because of that they can not be used without some change so read datasheet before calling these commands
         * you can crash your program by executing command without knowing what is does.
         */
        [[maybe_unused]] void write_command(const uint8_t &command);

        /**
         * @brief
         * Function to write data to TX_PLD
         * @details
         * The function does also always check if IRQ is active high and if that's true if resets IRQ to disable the lock
         * @tparam amount variable that controls the size of std::array Data
         * @param Data std::array uint8_t that written to the TX_PLD register
         * @param no_ack boolean that controls if the data is written with or without acknowledgement
         */
        template<size_t amount>
        [[maybe_unused]] void write_tx(std::array<uint8_t, amount> &Data, bool no_ack = false) {
            if(!IRQ.read()){
                register_write(REGISTER::STATUS, 0x70);
            }
            if (no_ack) {
                bool prev_active = Active;
                if(!prev_active) {
                    write_command(COMMAND::ACTIVATE);
                }
                setting_enable(SETTING::EN_DYN_ACK);
                write(COMMAND::W_TX_PAYLOAD_NO_ACK, Data);
                setting_disable(SETTING::EN_DYN_ACK);
                if(!prev_active) {
                    write_command(COMMAND::ACTIVATE);
                }
            } else {
                write(COMMAND::W_TX_PAYLOAD, Data);
            }
        }
    };
} //namespace IPASS
#endif //IPASS_RF24L01_H
