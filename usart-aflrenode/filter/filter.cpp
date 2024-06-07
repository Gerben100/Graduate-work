#include <iostream>
#include <fstream>
#include <string>

std::string get_file_encoding(const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        return ""; // Обработка ошибки открытия файла
    }

    // Определение кодировки с использованием библиотеки
    // Дополнительные библиотеки для определения кодировки могут потребоваться
    // В данном примере используется заглушка "UTF-8"
    return "UTF-8";
}

void split_file(const std::string& input_filename) {
    std::string encoding = get_file_encoding(input_filename);

    std::ifstream input_file(input_filename, std::ios::in | std::ios::binary);
    if (!input_file.is_open()) {
        std::cerr << "Failed to open input file." << std::endl;
        return;
    }

    std::ofstream output_file;
    std::string line;
    std::string output_content;
    int output_file_number = 1;

    while (std::getline(input_file, line)) {
        if (line.find(" ASSERTION FAILED") != std::string::npos) {
            if (output_file.is_open()) {
                output_file << output_content;
                output_file.close();
            }

            output_file_number++;
            output_file.open(std::to_string(output_file_number) + ".txt");
            output_content = line + "\n";
        } else {
            output_content += line + "\n";
        }
    }

    if (output_file.is_open()) {
        output_file << output_content;
        output_file.close();
    }
}

int main() {
    std::string input_filename = "output.log";
    split_file(input_filename);

    return 0;
}
