#ifndef IPASS_APA102_H
#define IPASS_APA102_H

#ifndef HWLIB_INC_HPP
#define HWLIB_INC_HPP
#include "hwlib.hpp"
#endif //HWLIB_INC_HPP

/** @file APA102.hpp
 *  @brief
 *  IPASS-project: Limited interface for the APA102 LED strip
 */


namespace IPASS {
    /**
     * @brief
     * limited OO class to control APA102 LED strip
     */
    class APA102 {
    private:
        /**
         * @brief
         * Bit banged spi bus object used to write data to the APA102
         */
        hwlib::spi_bus_bit_banged_sclk_mosi_miso &SPI_bus;
        /**
         * @brief
         * Variable that contains the amount of leds on the strip
         */
        uint8_t amount_of_leds;
    public:
        /**
         * @brief
         * Struct color to write RGB-values to the APA102
         */
        struct color {
            /**
             * @brief
             * red value of the RGB-color
             */
            uint8_t red;
            /**
             * @brief
             * green value of the RGB-color
             */
            uint8_t green;

            /**
             * @brief
             * blue value of the RGB-color
             */
            uint8_t blue;
        };
        /**
         * @brief
         * constexpr which contains the RGB-value of the color Black
         */
        constexpr static color black = {0x00, 0x00, 0x00};
        /**
         * @brief
         * constexpr which contains the RGB-value of the color Red
         */
        constexpr static color red = {0xff, 0x00, 0x00};
        /**
         * @brief
         * constexpr which contains the RGB-value of the color Green
         */
        constexpr static color green = {0x00, 0xff, 0x00};
        /**
         * @brief
         * constexpr which contains the RGB-value of the color Blue
         */
        constexpr static color blue = {0x00, 0x00, 0xff};
        /**
         * @brief
         * constexpr which contains the RGB-value of the color Yellow
         */
        constexpr static color yellow = {0xff, 0xff, 0};
        /**
         * @brief
         * constexpr which contains the RGB-value of the color Cyan
         */
        constexpr static color cyan = {0x00, 0xff, 0xff};
        /**
         * @brief
         * constexpr which contains the RGB-value of the color Magenta
         */
        constexpr static color magenta = {0xff, 0x00, 0xff};
        /**
         * @brief
         * constexpr which contains the RGB-value of the color White
         */
        constexpr static color white = {0xff, 0xff, 0xff};

        /**
         * @brief
         * Default constructor APA102
         * @details
         * constructor that takes a spi_bus and a uint8_t as parameters
         * @param SPI_bus SPI_BUS used to communicate with the LED
         * @param amount_of_leds UINT8_T used to determine how much leds need to be written default value = 8
         */
        APA102(hwlib::spi_bus_bit_banged_sclk_mosi_miso &SPI_bus, uint8_t amount_of_leds=8);

        /**
         * @brief
         * write function that uses a std::array of std::array<std::array<3, amount_of_leds> long
         * @tparam template_amount_of_leds template variable used to determine how much leds need to be written
         * @param colors std::array of std::array<std::array<3, amount_of_leds> that contains the color of all the specific LEDS
         * @param brightness uint8_t that controls the brighness of led default = 0x1f which is also the maximum value of the APA102-brightness
         */
        template<size_t template_amount_of_leds>
        void write(std::array<std::array<uint8_t, 3>, template_amount_of_leds> colors, uint8_t brightness = 0x1f) {
            auto transaction = SPI_bus.transaction(hwlib::pin_out_dummy);
            std::array<uint8_t, template_amount_of_leds*2+8> data = {0x00, 0x00, 0x00, 0x00};
            brightness >>=3;
            for (unsigned int i = 0; i < template_amount_of_leds; i++) {
                data[4 + i * 4] = (brightness | 0xe0);
                data[4 + i * 4 + 1] = colors[i][2];
                data[4 + i * 4 + 2] = colors[i][1];
                data[4 + i * 4 + 3] = colors[i][0];

            }
            data[4 + amount_of_leds * 4] = 0xff;
            data[4 + amount_of_leds * 4 + 1] = 0xFF;
            data[4 + amount_of_leds * 4 + 2] = 0xFF;
            data[4 + amount_of_leds * 4 + 3] = 0xFF;
            transaction.write(data);
        }
        /**
         * @brief
         * write function that writes the RGB-values from the Color struct to the amount of leds specified in the amount_of_leds of the constructor
         * @param kleur color struct that contains the RGB_Value for the leds
         * @param brightness uint8_t that controls the brighness of led default = 0x1f which is also the maximum value of the APA102-brightness
         */
        void write(color kleur, uint8_t brightness = 0x1f);
        /**
         * @brief
         * function to write pseudo-random colors to the apa102 by using the pseudo-random number function rand() of hwlib
         */
        void random_colors();

    };
}

#endif //IPASS_APA102_H
