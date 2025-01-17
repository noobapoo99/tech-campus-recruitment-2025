#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <ctime>
#include <iomanip>

using namespace std;

// Function to extract logs for a specific date
void extractLogsByDate(const string &logFilePath, const string &targetDate, const string &outputDir = "output")
{
    string outputFilePath = outputDir + "/output_" + targetDate + ".txt";

    ifstream logFile(logFilePath);
    if (!logFile.is_open())
    {
        cerr << "Error: Unable to open the log file." << endl;
        return;
    }

    // Open the output file
    ofstream outputFile(outputFilePath);
    if (!outputFile.is_open())
    {
        cerr << "Error: Unable to create the output file." << endl;
        return;
    }

    string line;
    regex logRegex(R"((\d{4}-\d{2}-\d{2})T(\d{2}:\d{2}:\d{2})\.\d{4} - (\w+) - (.*))");

    while (getline(logFile, line))
    {
        smatch match;

        // Check if the line matches the log format
        if (regex_search(line, match, logRegex))
        {
            // Extract the date, time, log level, and message
            string logDate = match[1];    // YYYY-MM-DD
            string logTime = match[2];    // HH:MM:SS
            string logLevel = match[3];   // Log level (INFO, ERROR, etc.)
            string logMessage = match[4]; // Log message

            // If the log date matches the target date, write it to the output file
            if (logDate == targetDate)
            {
                // Reformat and write to output file in desired format
                outputFile << logDate << " " << logTime << " " << logLevel << " " << logMessage << endl;
            }
        }
    }

    cout << "Logs for " << targetDate << " have been extracted to " << outputFilePath << endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <YYYY-MM-DD>" << endl;
        return 1;
    }

    string targetDate = argv[1];

    // Validate the date format (YYYY-MM-DD)
    struct tm tm = {};
    if (strptime(targetDate.c_str(), "%Y-%m-%d", &tm) == nullptr)
    {
        cerr << "Invalid date format. Please provide the date in YYYY-MM-DD format." << endl;
        return 1;
    }

    // i was using this but it is too large to push
    string logFilePath = "./logs_2024.log";

    // Call the function to extract logs for the given date
    extractLogsByDate(logFilePath, targetDate);

    return 0;
}

// clang++ -std=c++11 -arch arm64 extract_logs.cpp -o extract_logs
// ./extract_logs 2024-12-01