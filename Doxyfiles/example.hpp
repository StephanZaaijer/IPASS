/// @file

/// \page examples Examples
/// This page contains multiple examples and their explanation, circuit schematic
/// Those example are:
/// - \ref Example_RF24L01_HC_SR04
///
/// - \ref Example_RF24L01_APA102
///
///
/// <BR>
/// \section Example_RF24L01_HC_SR04 Example 1: RF24L01 in combination with HC-SR04
///
/// =========================================================================
/// This file is an example to which is used to Transmit and recieve the distance measured by a HC-SR04
///
/// This example concists of 2 parts:
/// - The first part is the main_tx()-function is an example which:
///     Measures the data HC-SR04
///
/// - The second part is the main_rx()-function is an example which:
///     Retrieves the data and sends it to the console
/// In the circuitschema the 3.3V-pin is displayed as a external powersource because of a mistake in the Arduino Due scheme in Fritzing
///
/// \subsection Example_HC_SR04_Transmit_circuit Transmit circuit schematic
///
/// <img src="example_HC_SR04_TX_schema.png" alt="Circuitscheme Tx" align="center">
///
/// \subsection Example_HC_SR04_Recieve_circuit Recieve circuit schematic
/// 
/// <img src="example_HC_SR04_RX_schema.png" alt="Circuitscheme Rx" align="center">
///
/// \subsection Example_HC_SR04_Transmit_code Transmit code example
/// 
/// \snippet "Example_RF24L01_HCSR04.cpp" Example_HC-SR04_TX
///
/// \subsection Example_HC_SR04_Recieve_code Recieve code example
/// 
/// \snippet "Example_RF24L01_HCSR04.cpp" Example_HC-SR04_RX
///
/// =========================================================================
///
/// \section Example_RF24L01_APA102 Example 2: RF24L01 in combination with APA102
///
/// =========================================================================
/// This file is an example to which is used to Transmit and recieve RGB and brightness data from 4 Potentiometers
///
/// This example concists of 2 parts:
/// - The first part is the main_tx()-function is an example which:
///   checks if one of the buttons is pressed
///   -# if the first button connected to D52 is pressed:
///      Measures the data from the potentiometers and sends it
///    -# if the second button connected to D11 is pressed:
///      Send {0,0,0,0,0} to turn of the leds
///    -# if the third button connected to D13 is pressed:
///     Send {0,0,0,0,255} to turn on the random_colors function off the APA102-library which sends pseudo-random values to the apa102
///
/// - The second part is the main_rx()-function is an example which:
///     Retrieves the data and sends it to the APA102
/// In the circuitschema the 3.3V-pin is displayed as a external powersource because of a mistake in the Arduino Due scheme in Fritzing
///
/// \subsection Example_APA102_Transmit_circuit Transmit circuit schematic
///
/// <img src="Ipass_Sketch_TX_schema.png" alt="Circuitscheme Tx" align="center">
///
/// \subsection Example_APA102_Recieve_circuit Recieve circuit schematic
///
/// <img src="Ipass_Sketch_RX_schema.png" alt="Circuitscheme Rx" align="center">
///
/// \subsection Example_APA102_Transmit_code Transmit code example
/// 
/// \snippet "Example_RF24L01_APA102.cpp" Example_RF24L01_APA102_TX
///
/// \subsection Example_APA102_Recieve_code Recieve code example
/// 
/// \snippet "Example_RF24L01_APA102.cpp" Example_RF24L01_APA102_RX
///
/// =========================================================================
///