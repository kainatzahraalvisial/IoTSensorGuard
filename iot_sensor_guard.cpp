#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <iomanip> // For std::setw and std::setfill

#ifdef _WIN32
#include <cstdlib> // For system("cls") on Windows
#else
#include <unistd.h> // For system("clear") on Unix-based systems
#endif

class IoTSensorGuard {
private:
    std::string device_id;
    std::string key;
    time_t last_key_rotation;
    std::string location;
    double temperature;

    std::string generate_key() {
        std::string combined = device_id + location + 
                             std::to_string(static_cast<int>(temperature * 10)) + std::to_string(time(nullptr));
        int sum = 0;
        for (char c : combined) {
            sum += c;
        }
        srand(sum);
        std::string new_key;
        const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        for (int i = 0; i < 4; ++i) {
            new_key += chars[rand() % chars.length()];
        }
        return new_key;
    }

    int calculate_checksum(const std::string& data) {
        int sum = 0;
        for (char c : data) {
            sum += c;
        }
        return sum % 256;
    }

public:
    IoTSensorGuard(const std::string& id, const std::string& loc, double temp)
        : device_id(id), location(loc), temperature(temp), key("") {
        key = generate_key();
        last_key_rotation = time(nullptr);
    }

    std::string get_location() const {
        return location;
    }

    double get_temperature() const {
        return temperature;
    }

    std::string encrypt(const std::string& message, int& checksum) {
        time_t now = time(nullptr);
        if (now - last_key_rotation >= 300) { // Increased to 5 minutes
            key = generate_key();
            last_key_rotation = now;
            std::cout << "Key rotated for " << device_id << ".\n";
        }
        std::string ciphertext = message;
        for (size_t i = 0; i < message.length(); ++i) {
            ciphertext[i] = message[i] ^ key[i % key.length()];
        }
        checksum = calculate_checksum(message);
        std::cout << "Debug: Encryption key: " << key << "\n";
        return ciphertext; // Return raw ciphertext without filtering for now
    }

    std::string decrypt(const std::string& ciphertext, int checksum) {
        time_t now = time(nullptr);
        if (now - last_key_rotation >= 300) {
            key = generate_key();
            last_key_rotation = now;
            std::cout << "Key rotated for " << device_id << ".\n";
        }
        std::string plaintext = ciphertext;
        for (size_t i = 0; i < ciphertext.length(); ++i) {
            plaintext[i] = ciphertext[i] ^ key[i % key.length()];
        }
        int expected_checksum = calculate_checksum(plaintext);
        std::cout << "Debug: Decryption key: " << key << "\n";
        if (expected_checksum != checksum) {
            throw std::runtime_error("Data tampered! Checksum mismatch.");
        }
        return plaintext;
    }
};

std::string get_valid_string(const std::string& prompt) {
    std::string input;
    do {
        std::cout << prompt;
        if (!std::getline(std::cin, input)) {
            std::cout << "Error reading input. Try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (input.empty()) {
            std::cout << "Input cannot be empty. Try again.\n";
        }
    } while (input.empty());
    return input;
}

double get_valid_temperature(const std::string& prompt) {
    std::string input;
    double temp;
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, input)) {
            std::cout << "Error reading input. Try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.clear();
        try {
            temp = std::stod(input);
            return temp;
        }
        catch (...) {
            std::cout << "Invalid number. Try again.\n";
        }
    }
}

int get_valid_int(const std::string& prompt) {
    std::string input;
    int value;
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, input) || input.empty()) {
            std::cout << "Error reading input. Try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.clear();
        try {
            size_t pos;
            value = std::stoi(input, &pos);
            if (pos == input.length()) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
            else {
                std::cout << "Invalid number. Please enter a valid integer.\n";
            }
        }
        catch (...) {
            std::cout << "Invalid number. Please enter a valid integer.\n";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void show_menu() {
    std::cout << "\n=== IoTSensorGuard: Secure IoT Sensor Data ===\n";
    std::cout << "1. Encrypt sensor data\n";
    std::cout << "2. Decrypt sensor data\n";
    std::cout << "3. Exit\n";
    std::cout << "Enter choice (1-3): ";
}

int main() {
    std::cout << "=== Welcome to IoTSensorGuard ===\n";
    std::cout << "Protects IoT sensor data (temperature and location) from attacks.\n\n";

    std::cout << "Configure IoT devices:\n";
    std::string sensor_id = get_valid_string("Enter sensor device ID (e.g., sensor1): ");
    std::string server_id = get_valid_string("Enter server device ID (e.g., server1): ");
    std::string location = get_valid_string("Enter sensor's location name (e.g., NewYork): ");
    double temperature = get_valid_temperature("Enter temperature at this location (e.g., 25.5): ");

    IoTSensorGuard sensor(sensor_id, location, temperature);
    IoTSensorGuard server(server_id, location, temperature);

    while (true) {
        show_menu();
        std::cout << "Debug: About to read choice...\n";
        int choice = get_valid_int("");
        std::cout << "Debug: Choice entered: " << choice << "\n";
        if (choice == 3) {
            std::cout << "\nExiting IoTSensorGuard. Goodbye!\n";
            break;
        }

        try {
            if (choice == 1) {
                std::string message = "Temp: " + std::to_string(static_cast<int>(sensor.get_temperature() * 10) / 10.0) + "C, Loc: " + sensor.get_location();
                std::cout << "\nEncrypting sensor data: " << message << "\n";
                int checksum;
                std::string ciphertext = sensor.encrypt(message, checksum);
                std::cout << "Ciphertext: " << ciphertext << "\n";
                std::cout << "Checksum (for integrity): " << checksum << "\n";
                std::cout << "Encryption successful! Save the ciphertext and checksum for decryption.\n";
                std::cout << "Debug: Encryption completed successfully.\n";
            }
            else if (choice == 2) {
                clear_screen();
                std::cout << "=== IoTSensorGuard: Decryption Mode ===\n";
                std::string ciphertext = get_valid_string("Enter the ciphertext: ");
                int checksum = get_valid_int("Enter the checksum: ");
                std::cout << "\nDecrypting data...\n";
                std::string decrypted = server.decrypt(ciphertext, checksum);
                std::cout << "Decrypted data: " << decrypted << "\n";
                std::cout << "Decryption successful! Data verified.\n";
            }
            else {
                std::cout << "Invalid choice. Please select 1, 2, or 3.\n";
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
            std::cout << "Press Enter to continue...\n";
            std::cin.get(); // Pause on error
        }
    }

    std::cout << "Press Enter to exit...\n";
    std::cin.get();
    return 0;
}