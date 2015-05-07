//============================================================================
// Name        : main.cpp
// Author      : Jefferson Chaves Gomes
// Version     : 1.0.0
// Copyright   : Academic Program
// Description : EP 03 in C++
//============================================================================

// Libraries definitions
// -----------------------------------------------------
#include <iostream>     // for std::cin, std::cout, etc.
#include <limits.h>     // for INT_MAX
#include <vector>       // for vector
#include <cmath>        // for std::pow
#include <iomanip>      // for std::setprecision
#include <stdlib.h>     // for std:atoi
#include <mpi.h>        // for MPI

// Constants definitions
// -----------------------------------------------------
#define MAX_ATTEMPTS 10
#define PROCESS_MASTER 0

// Enums definitions
// -----------------------------------------------------
enum OutputType {
    NONE = 0,
    TIME = 1,
    SUM = 2,
    ALL = 3
};

// Structs definitions
// -----------------------------------------------------
struct Result {
        long processTime;
        long double sum;
};

// Functions declarations
// -----------------------------------------------------
void readInputParams(int, char**);
int readNumbersCount();
OutputType readOutputType();
std::vector<float> readVecNumbers(const int);
void printUsage();
OutputType stringToOutputType(std::string);
void printResult(OutputType, Result);
void allowAnotherAttempt(const std::string, int&);
bool isPowerOfTwo(uint number);

// -----------------------------------------------------
// main Function
// -----------------------------------------------------
int main(int argc, char **argv) {
    int processCount;
    int processRank;
    int numberToSend;
    int currentLevel,
    int auxLevel;
    int NextLevel;
    int processSource;
    int processSourceTag;
    int processTarget;
    int processTargetTag;
    float levelsCount;
    static int totalSum = 0;
    MPI_Status status;
    readInputParams(argc, argv);
    OutputType outputType = readOutputType();
    int numbersCount = readNumbersCount();
    std::vector<float> vecNumbers = readVecNumbers(numbersCount);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);

    if (isPowerOfTwo(processCount)) {

    }

    //
    //
    //
    //
    //
    //
    //
    //
    //
    // ~~~~~~~~~~~~~~~
    Result result;
    result.sum = 0.0;
    result.processTime = 0;
    for (int i = 0; i < vecNumbers.size(); i++) {
        std::cout << "Number " << (i + 1) << ": " << vecNumbers[i] << "\n";
        result.sum += vecNumbers[i];
    }
    // ~~~~~~~~~~~~~~~
    // Result result = startParallelProcess(vecRelations);
    printResult(outputType, result);
    return EXIT_SUCCESS;
}

// Function implementations
// -----------------------------------------------------
void readInputParams(int argc, char **argv) {
    std::cout << "proc number: " << argv[0] << "\n\n";
    if (argc != 1) {
        printUsage();
        exit (EXIT_FAILURE);
    }
}

OutputType readOutputType() {
    int badAttempts = 0;
    std::string outputOption = "NONE";
    std::cin >> outputOption;
    while (stringToOutputType(outputOption) == NONE) {
        allowAnotherAttempt("Error: The out put type must be equal then [sum | time | all].", badAttempts);
        std::cin.clear();
        std::cin >> outputOption;
    }
    return stringToOutputType(outputOption);
}

int readNumbersCount() {
    int badAttempts = 0;
    int numberCount;
    std::cin >> numberCount;
    while (std::cin.bad() || std::cin.fail() || numberCount < 2) {
        allowAnotherAttempt("Error: The amont of numbers must be a valid integer greater then 1.", badAttempts);
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
        std::cin >> numberCount;
    }
    return numberCount;
}

std::vector<float> readVecNumbers(const int numberCount) {
    int badAttempts = 0;
    float number = 0.0;
    std::vector<float> vecNumbers;
    for (long i = 0; i < numberCount; i++) {
        std::cin >> number;
        while (std::cin.bad() || std::cin.fail()) {
            allowAnotherAttempt("Error: The number must be a integer or a float [5 | 5.0].", badAttempts);
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            std::cin >> number;
        }
        vecNumbers.push_back(number);
    }
    return vecNumbers;
}

void allowAnotherAttempt(const std::string msg, int &badAttempts) {
    std::cout << msg << "\n";
    std::cout << "Try again.\n";
    if (++badAttempts >= MAX_ATTEMPTS) {
        printUsage();
        exit (EXIT_FAILURE);
    }
}

bool isPowerOfTwo(uint number) {
    return (number & -number) == number;
}

void printUsage() {
    std::cout << "Usage:\n";
    std::cout << "\tmpirun [-n <number of process>] pp-ep03-012015\n\n";
    std::cout << "\toutput type         -- after run, inform the output type [sum | time | all]\n";
    std::cout << "\tnumber of threads   -- and after, inform the amount of numbers to be added\n";
    std::cout << "\nSample:\n";
    std::cout << "\tmpirun -n 4 pp-ep03-012015\n";
    std::cout << "\tall\n";
    std::cout << "\t8\n";
    std::cout << "\t1.0 2.0 3.0 4.0 5.0 6.0 7.0 8.0\n";
}

OutputType stringToOutputType(std::string input) {
    if (input == "time") {
        return TIME;
    } else if (input == "sum") {
        return SUM;
    } else if (input == "all") {
        return ALL;
    }
    return NONE;
}

void printResult(OutputType outputType, Result result) {
    std::cout << std::endl;
    switch (outputType) {
        case TIME:
            std::cout << result.processTime << "\n";
            break;
        case SUM:
            std::cout << std::fixed;
            std::cout << std::setprecision(2) << result.sum << "\n";
            break;
        case ALL:
            std::cout << std::fixed;
            std::cout << std::setprecision(2) << result.sum << "\n";
            std::cout << result.processTime << "\n";
            break;
        default:
            break;
    }
}

