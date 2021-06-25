#ifndef IPASS_APA102_H
#define IPASS_APA102_H

#ifndef HWLIB_INC_HPP
#define HWLIB_INC_HPP
#include "hwlib.hpp"
#endif //HWLIB_INC_HPP

/** @file HC_SR04.hpp
 *  @brief
 *  IPASS-project: Limited interface for the HC_SR04 ultrasonic distance sensor
 */

namespace IPASS {
    /**
     * @brief
     * Limited oo class to control HC-SR04 ultrasonic sensor
     */
    class HC_SR04 {
    private:
        /**
         * @brief
         * hwlib::pin_out object that contains the details of the trigger pin
         */
        hwlib::pin_out &trigger_pin;
        /**
         * @brief
         * hwlib::pin_out object that contains the details of the echo pin
         */

        hwlib::pin_in &echo_pin;

        /**
         * @brief
         * private function to measure the time between sending and returning the ultrasonic puls
         * @return uint_fast64_t that contains the time in us between sending and recieving the ultrasonic puls
         */
        uint_fast64_t get_time_distance();

    public:
        /**
         * @brief
         * Standard constructor that takes two parameters
         * @param trigger_pin hwlib::pin_out object to control the trigger pin
         * @param echo_pin hwlib::pin_in object to control the echo pin
         */
        HC_SR04(hwlib::pin_out &trigger_pin, hwlib::pin_in &echo_pin);

        /**
         * @brief
         * function that returns the distance in meters
         * @return float distance in meters
         */
        float get_distance();
    };
}
#endif //IPASS_APA102_H
