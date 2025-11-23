/*
//      Code written from reference 2.4.8 txrx_loopback_to_file.cpp (RX)
*/
#include <boost/program_options.hpp>
#include <mutex>
#include <thread>
#include <chrono>
#include <csignal>
#include <sstream>
#include <csignal>
#include <iostream>
#include "Adc.h"
#include <cstdlib>

namespace po = boost::program_options;

//Tracks if first packet - lets us know if we want to store it to the output file
bool first_packet = true;

// Interrupt signal handler
bool stop_signal_called = false;
void sig_int_handler(int) {
    stop_signal_called = true;
}

int main(int argc, char *argv[]) {
    //INPUT HANDLING WITH BOOST
    bool motor_1, motor_2, motor_3, motor_4, motor_5, motor_6;

    // setup the program options
    po::options_description desc("Allowed options");
    // clang-format off
    desc.add_options()
        ("help", "help message")
        ("motor1", po::value<bool>(&motor_1)->default_value(false), "If motor 1 is on or off")
        ("motor2", po::value<bool>(&motor_2)->default_value(false), "If motor 2 is on or off")
        ("motor3", po::value<bool>(&motor_3)->default_value(false), "If motor 3 is on or off")
        ("motor4", po::value<bool>(&motor_4)->default_value(false), "If motor 4 is on or off")
        ("motor5", po::value<bool>(&motor_5)->default_value(false), "If motor 5 is on or off")
	("motor6", po::value<bool>(&motor_6)->default_value(false), "If motor 6 is on or off")
    ;
    // clang-format on
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    //END OF INPUT HANDLING WITH BOOST

    //Print out help message if neccessary/asked for
    if (vm.count("help")) {
        std::cout << "AIBL_MAIN " << desc << std::endl;
        return ~0;
    }

    // Set the signal handler
    std::signal(SIGINT, &sig_int_handler);
    // Keep running until CTRL-C issued
    size_t count = 0;

/*Handling global setup for the kernel - MUST BE RUN" */

    int ret = std::system("../libs/INIT/INIT");  // Runs the INIT program
    if (ret != 0) {
        std::cout << "Couldn't find initialization for this kernel instance; please ensure it exists" << std::endl;
    }

    




    while(not stop_signal_called)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // sleep for 1ms to make sure can catch CTRL-C
        count++;
        if(count % 3000 == 0)
        {
                /*pwm.enable();
                std::cout << "Motor 1:\t" <<  motor_1 << std::endl;
                std::cout << "Motor 2:\t" <<  motor_2 << std::endl;
                std::cout << "Motor 3:\t" <<  motor_3 << std::endl;
		std::cout << "Motor 4:\t" <<  motor_4 << std::endl;
		std::cout << "Motor 5:\t" <<  motor_5 << std::endl;
		std::cout << "Motor 6:\t" <<  motor_6 << std::endl;
		*/

		std::cout << "----------------------------------------------------" << std::endl;
		std::cout << "Voltage for ADC 0: " << ADC::readVoltage(0) << std::endl;
		std::cout << "Voltage for ADC 1: " << ADC::readVoltage(1) << std::endl;
		std::cout << "Voltage for ADC 2: " << ADC::readVoltage(2) << std::endl;
		std::cout << "Voltage for ADC 3: " << ADC::readVoltage(3) << std::endl;
		std::cout << "Voltage for ADC 4: " << ADC::readVoltage(4) << std::endl;
		std::cout << "Voltage for ADC 5: " << ADC::readVoltage(5) << std::endl;
		std::cout << "Voltage for ADC 6: " << ADC::readVoltage(6) << std::endl;
		
        }
    }


}

