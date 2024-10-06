#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

// Function to load NASA Earth Observation Data from a .txt file in the root directory
std::map<std::string, float> loadEarthObservationData(const std::string& filename) {
    std::map<std::string, float> data;
    std::ifstream file(filename);

    // Debug statement to confirm the file path
    std::cout << "Attempting to open file for reading: " << filename << std::endl;

    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << filename << std::endl;
        return data;
    }

    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string date;
        float value;
        if (!(iss >> date >> value)) {
            break; // Error parsing line
        }
        data[date] = value;
    }
    file.close();
    return data;
}

// Function to compute average soil moisture
float computeAverageMoisture(const std::map<std::string, float>& data) {
    float total = 0.0;
    int count = 0;

    for (const auto& entry : data) {
        total += entry.second;
        ++count;
    }
    return count ? total / count : 0.0;
}

// Function to identify drought or flood conditions
std::string analyzeMoistureData(float avgMoisture) {
    if (avgMoisture < 20.0) {
        return "Drought Condition Detected!";
    } else if (avgMoisture > 80.0) {
        return "Flood Condition Detected!";
    } else {
        return "Normal Soil Moisture Levels.";
    }
}

// Function to suggest actions based on soil moisture levels
void suggestAgriculturalActions(const std::string& condition) {
    if (condition == "Drought Condition Detected!") {
        std::cout << "Suggested Action: Implement irrigation strategies to conserve water.\n";
    } else if (condition == "Flood Condition Detected!") {
        std::cout << "Suggested Action: Implement drainage solutions to avoid crop damage.\n";
    } else {
        std::cout << "Suggested Action: Maintain regular crop monitoring.\n";
    }
}

// Function to export data to CSV in the main project directory
void exportDataToCSV(const std::map<std::string, float>& data, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open output file: " << filename << std::endl;
        return;
    }

    outFile << "Date,SoilMoisture\n";
    for (const auto& entry : data) {
        outFile << entry.first << "," << entry.second << "\n";
    }

    outFile.close();
}

int main() {
    std::map<std::string, float> soilMoistureData;
    std::string inputFilename;
    std::string outputFilename;

    // Get the root project directory using std::filesystem
    fs::path projectRoot = fs::current_path().parent_path();

    // Load data from 'nasa_soil_moisture_data.txt' file in the root directory
    inputFilename = projectRoot / "nasa_soil_moisture_data.txt";
    std::cout << "Loading data from: " << inputFilename << std::endl;
    soilMoistureData = loadEarthObservationData(inputFilename);

    if (soilMoistureData.empty()) {
        std::cerr << "No data found or failed to load data from: " << inputFilename << ". Exiting program." << std::endl;
        return 1;
    }

    // Calculate average soil moisture
    float avgMoisture = computeAverageMoisture(soilMoistureData);
    std::cout << "Average Soil Moisture: " << avgMoisture << "%" << std::endl;

    // Analyze the moisture data
    std::string moistureCondition = analyzeMoistureData(avgMoisture);
    std::cout << "Condition Analysis: " << moistureCondition << std::endl;

    // Suggest actions based on the analysis
    suggestAgriculturalActions(moistureCondition);

    // Export data to CSV file in the main project directory
    outputFilename = projectRoot / "soil_moisture_output.csv";
    exportDataToCSV(soilMoistureData, outputFilename);
    std::cout << "Data exported to: " << outputFilename << std::endl;

    return 0;
}
