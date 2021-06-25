#include "HC_SR04.hpp"
namespace IPASS {
    HC_SR04::HC_SR04(hwlib::pin_out &trigger_pin, hwlib::pin_in &echo_pin) :
            trigger_pin(trigger_pin),
            echo_pin(echo_pin), {
    }

    uint_fast64_t HC_SR04::get_time_distance() {
        trigger_pin.write(true);
        hwlib::wait_us(10);
        trigger_pin.write(false);
        while (not echo_pin.read()) {}
        int starttime = hwlib::now_us();
        while (echo_pin.read()) {}
        uint_fast64_t finishtime = hwlib::now_us();
        uint_fast64_t time_start_object_start = finishtime - starttime;
        uint_fast64_t time_start_object = time_start_object_start / 2;
        return time_start_object;
    }

    float HC_SR04::get_distance() {
        return (get_time_distance() / 1000000) * 343;
    }
}