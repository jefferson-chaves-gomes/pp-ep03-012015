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
#include <cmath>        // for std::pow, std::ceil
#include <iomanip>      // for std::setprecision
#include <stdlib.h>     // for std:atoi
#include <mpi.h>        // for MPI

//
// temp start
//
#include <unistd.h>
//
// temp end
//

// Constants definitions
// -----------------------------------------------------
#define BREAK_LINE "\n"
#define TAB "\t"
#define MAX_ATTEMPTS 5
#define PROCESS_MASTER 0
#define GLOBAL_LISTEN_TAG 0
#define BLOCKS_LISTEN_TAG 1

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
void readInputParams(int, char**, const int);
int readNumbersCount();
OutputType readOutputType();
void readNumbersArray(float*, const int);
void printUsage();
OutputType stringToOutputType(std::string);
void printResult(OutputType, Result);
void allowAnotherAttempt(const std::string, int&);

// -----------------------------------------------------
// main Function
// -----------------------------------------------------
int main(int argc, char **argv) {

    // Input variables
    OutputType outputType = NONE;
    int numbersCount = 0;
    float *numbersArray = NULL;

    // MPI variables
    int processRank, processCount = 0;
    int processSource, processTarget;
    int level, currentLevel, nextLevel;
    float levelsCount = 0;
    double processedNumber;
    static double partialSum = 0;
    static double totalSum = 0;
    float receivedNumber;
    float *receivedNumbersArray = NULL;
    MPI_Status status;
    std::vector<float> processPartNumbersVector;

    // MPI initialization
    // ----------------------------
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);

    if (processCount == 1) {

    }

    int blockSize = 0;
    if (processRank == PROCESS_MASTER) {

        // Reading input data
        readInputParams(argc, argv, processCount);
        outputType = readOutputType();
        numbersCount = readNumbersCount();
        numbersArray = new float[numbersCount];
        readNumbersArray(numbersArray, numbersCount);

        //
        // temp begin
        //
        std::cout << "Output type: " << outputType << BREAK_LINE;
        std::cout << "Number of process: " << processCount << BREAK_LINE;
        std::cout << "Number of numbers: " << numbersCount << BREAK_LINE;
        std::cout << "Number of levels: " << levelsCount << BREAK_LINE;
        //
        // temp end
        //

        // Step 1 - Element Distribution - SENDING
        // ----------------------------
        int *blockSizesArray = new int[processCount];
        std::fill_n(blockSizesArray, processCount, 0);
        for (int i = 0, processDest = 0; i < numbersCount; i++) {

            if (processDest == PROCESS_MASTER) {
                processPartNumbersVector.push_back(numbersArray[i]);
            } else {
                MPI_Send(&numbersArray[i], 1, MPI_FLOAT, processDest, GLOBAL_LISTEN_TAG, MPI_COMM_WORLD);
            }
            blockSizesArray[processDest]++;
            if (processDest == (processCount - 1)) {
                processDest = 0;
            } else {
                processDest++;
            }
        }
        for (int i = 1; i < processCount; i++) {
            MPI_Send(&blockSizesArray[i], 1, MPI_INT, i, BLOCKS_LISTEN_TAG, MPI_COMM_WORLD);
        }
        blockSize = blockSizesArray[processRank];
        delete[] blockSizesArray;
    } else {
        // Step 1 - Element Distribution - RECEIVING
        // ----------------------------
        MPI_Recv(&blockSize, 1, MPI_INT, 0, BLOCKS_LISTEN_TAG, MPI_COMM_WORLD, &status);
        for (int i = 0; i < blockSize; i++) {
            MPI_Recv(&receivedNumber, 1, MPI_FLOAT, 0, GLOBAL_LISTEN_TAG, MPI_COMM_WORLD, &status);
            processPartNumbersVector.push_back(receivedNumber);
        }
    }
    //
    // temp begin
    //
    std::cout << "rank: " << processRank << " -> ";
    for (int i = 0; i < processPartNumbersVector.size(); i++) {
        std::cout << " | " << processPartNumbersVector[i];
    }
    std::cout << BREAK_LINE;
    //
    // temp end
    //
    delete[] receivedNumbersArray;
    delete[] numbersArray;

    //
    // temp start
    //
    usleep(1000000);
    //
    // temp end
    //

    // Step 2 - Reduction Tree Simulation
    // ----------------------------
    partialSum = processPartNumbersVector[0];
    int numbersToSendCount = blockSize - 1;
    if (numbersToSendCount > 0) {
        processTarget = processRank == processCount - 1 ? PROCESS_MASTER : processRank + 1;
        std::cout << "rank: " << processRank << " sending '" << numbersToSendCount << "' numbers to proc " << processTarget << "\n";
        MPI_Send(&numbersToSendCount, 1, MPI_INT, processTarget, BLOCKS_LISTEN_TAG, MPI_COMM_WORLD);
        for (int i = 1; i < processPartNumbersVector.size(); i++) {
//            std::cout << "rank: " << processRank << " sending '" << processPartNumbersVector[i] << "' to proc " << processTarget << "\n";
            MPI_Send(&processPartNumbersVector[i], 1, MPI_FLOAT, processTarget, GLOBAL_LISTEN_TAG, MPI_COMM_WORLD);
        }
        usleep(1000000);
        processSource = processRank == PROCESS_MASTER ? processCount - 1 : processRank - 1;
        int numbersToReceiveCount;
        MPI_Recv(&numbersToReceiveCount, 1, MPI_INT, processSource, BLOCKS_LISTEN_TAG, MPI_COMM_WORLD, &status);
        std::cout << "rank: " << processRank << " receiving '" << numbersToReceiveCount << "' numbers to proc " << processSource << "\n";
        for (int i = 0; i < numbersToReceiveCount; i++) {
            MPI_Recv(&receivedNumber, 1, MPI_FLOAT, processSource, GLOBAL_LISTEN_TAG, MPI_COMM_WORLD, &status);
            partialSum += receivedNumber;
        }
    }

    //
    // temp begin
    //
    usleep(1000000);
    std::cout << "rank: " << processRank << " partial sum -> " << partialSum;
    std::cout << BREAK_LINE;
    //
    // temp end
    //

    // Step 3 - Reduction Tree
    // ----------------------------
    totalSum = partialSum;
    levelsCount = log2(processCount);
    for (currentLevel = 0; currentLevel < levelsCount; currentLevel++) {
        level = (int) (std::pow(2, currentLevel));
        if ((processRank % level) == 0) {
            nextLevel = (int) (pow(2, (currentLevel + 1)));
            if ((processRank % nextLevel) == 0) {
                processSource = processRank + level;
                MPI_Recv(&processedNumber, 1, MPI_DOUBLE, processSource, GLOBAL_LISTEN_TAG, MPI_COMM_WORLD, &status);
                totalSum += processedNumber;
            } else {
                processTarget = processRank - level;
                MPI_Send(&totalSum, 1, MPI_DOUBLE, processTarget, GLOBAL_LISTEN_TAG, MPI_COMM_WORLD);
            }
        }
    }
    // Printing the result
    // ----------------------------
    if (processRank == PROCESS_MASTER) {
        Result result;
        result.sum = totalSum;
        result.processTime = 7;
        for (int i = 0; i < numbersCount; i++) {
            std::cout << "Number " << (i + 1) << ": " << numbersArray[i] << BREAK_LINE;
        }
        // Result result = startParallelProcess(vecRelations);
        printResult(outputType, result);
    }
    // MPI finalization
    // ----------------------------
    MPI_Finalize();
    return EXIT_SUCCESS;
}

// Function implementations
// -----------------------------------------------------
void readInputParams(int argc, char **argv, const int processCount) {
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
        allowAnotherAttempt("Error: The output type must be equal then [sum | time | all].", badAttempts);
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

void readNumbersArray(float* numbersArray, const int numbersCount) {
    int badAttempts = 0;
    double number = 0.0;
    for (long i = 0; i < numbersCount; i++) {
        std::cin >> number;
        while (std::cin.bad() || std::cin.fail()) {
            allowAnotherAttempt("Error: The number must be a integer or a float [5 | 5.0].", badAttempts);
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
            std::cin >> number;
        }
        numbersArray[i] = number;
    }
}

void allowAnotherAttempt(const std::string msg, int &badAttempts) {
    if (++badAttempts >= MAX_ATTEMPTS) {
        printUsage();
        exit (EXIT_FAILURE);
    }
    std::cout << msg << BREAK_LINE;
    std::cout << "Try again (you can try more " << MAX_ATTEMPTS - badAttempts << " times)." << BREAK_LINE;
}

void printUsage() {
    std::cout << BREAK_LINE << BREAK_LINE;
    std::cout << "Usage:" << BREAK_LINE;
    std::cout << TAB << "mpirun [-n <number of process>] pp-ep03-012015" << BREAK_LINE << BREAK_LINE;
    std::cout << TAB << "output type         -- after run, inform the output type [sum | time | all]" << BREAK_LINE;
    std::cout << TAB << "number of threads   -- and after, inform the amount of numbers to be added" << BREAK_LINE << BREAK_LINE;
    std::cout << "Sample:" << BREAK_LINE;
    std::cout << TAB << "mpirun -n 4 pp-ep03-012015" << BREAK_LINE;
    std::cout << TAB << "all" << BREAK_LINE;
    std::cout << TAB << "8" << BREAK_LINE;
    std::cout << TAB << "1.0 2.0 3.0 4.0 5.0 6.0 7.0 8.0" << BREAK_LINE << BREAK_LINE;
    std::cout << "NOTE: if the given number for <number of process> is equal to 0, then the MPI library will set it to 2." << BREAK_LINE << BREAK_LINE;
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
            std::cout << result.processTime << BREAK_LINE;
            break;
        case SUM:
            std::cout << std::fixed;
            std::cout << std::setprecision(2) << result.sum << BREAK_LINE;
            break;
        case ALL:
            std::cout << std::fixed;
            std::cout << std::setprecision(2) << result.sum << BREAK_LINE;
            std::cout << result.processTime << BREAK_LINE;
            break;
        default:
            break;
    }
}

