#include <iostream>
#include <fstream> 
#include <string>
#include <boost/asio.hpp>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <thread>
#include <chrono>

using boost::asio::ip::tcp;

std::string response;

bool indicatorReadBuffer;

std::string executeRadamsa() {
    std::string command = "radamsa -n 1 < input.txt";
    char buffer[1024];
    std::string result = "";

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("Failed to execute Radamsa");
    }

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }

    pclose(pipe);

    return result;
}

void threadReadBuffer(tcp::socket& socket) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    char response_buffer[102400];
    size_t bytes_received = socket.read_some(boost::asio::buffer(response_buffer));
    response.assign(response_buffer, bytes_received);
    indicatorReadBuffer = false;
}

int main() {
    try {
	std::cout<<"Start"<<std::endl;

        boost::asio::io_context io_context;

        tcp::resolver resolver(io_context);

        tcp::resolver::results_type endpoints = resolver.resolve("10.0.2.16", "23");

        tcp::socket socket(io_context);

        boost::asio::connect(socket, endpoints);

        std::string input_data;
        std::srand(std::time(0));

        std::ofstream logFile("log.txt");

        while (true) {
	    indicatorReadBuffer = false;
            auto start_time = std::chrono::high_resolution_clock::now();
            auto end_time = start_time;
            auto duration = std::chrono::seconds(0);
            std::string mutated_data = executeRadamsa();

            logFile << "Sending to Embox: " << std::endl;
            if (mutated_data.size() < 256) logFile << mutated_data << std::endl;
            else mutated_data = "";

            boost::asio::write(socket, boost::asio::buffer(mutated_data + "\r\n"));
            
            std::this_thread::sleep_for(std::chrono::seconds(1));
            
	    start_time = std::chrono::high_resolution_clock::now();
	    indicatorReadBuffer = true;

            std::thread newThread(threadReadBuffer, std::ref(socket));
            newThread.detach();

	    while (indicatorReadBuffer == true)
            {
            end_time = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
	    if (duration.count() >= 15) break;
	    }
            
	    bool indicator = false;
            if (duration.count() >= 15) indicator = true;

	    if (indicator) break;

            logFile << "Got from Embox: " << std::endl;
            logFile << response << std::endl;

            response = "";
        }
        socket.close();
        logFile.close();
	std::cout<<"Error"<<std::endl;
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 1;
}

