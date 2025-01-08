#include <iostream>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <thread>

using namespace std;

const string INPUT_FILE = "input.txt";
const string OUTPUT_FILE = "output.txt";
const string BUFFER_FILE = "buffer.txt";
const string PRODUCE_LOCK = "produce.lock";
const string CONSUME_LOCK = "consume.lock";

void produce_and_consume() {
    ifstream inputFile(INPUT_FILE);
    string number;
    int count = 0;

    ofstream outputFile(OUTPUT_FILE, ios::trunc);
    outputFile.close();

    while (count < 10 && getline(inputFile, number)) {
        this_thread::sleep_for(chrono::milliseconds(rand() % 100 + 100));

        while (access(CONSUME_LOCK.c_str(), F_OK) == 0) {}

        ofstream bufferFile(BUFFER_FILE);
        bufferFile << number;
        bufferFile.close();
        cout << "Produced: " << number << endl;

        ofstream produceLock(PRODUCE_LOCK);
        produceLock.close();

        this_thread::sleep_for(chrono::milliseconds(rand() % 100 + 100));

        while (access(PRODUCE_LOCK.c_str(), F_OK) != 0) {}

        ifstream bufferFileRead(BUFFER_FILE);
        getline(bufferFileRead, number);
        bufferFileRead.close();

        ofstream outputFileAppend(OUTPUT_FILE, ios::app);
        outputFileAppend << number;
        if (count < 9) {
            outputFileAppend << endl;
        }
        outputFileAppend.close();
        cout << "Consumed: " << number << endl;

        remove(PRODUCE_LOCK.c_str());
        ofstream consumeLock(CONSUME_LOCK);
        consumeLock.close();

        remove(CONSUME_LOCK.c_str());

        count++;
    }

    inputFile.close();
}

bool check_files_identical(const string& file1, const string& file2) {
    ifstream f1(file1, ios::binary | ios::ate);
    ifstream f2(file2, ios::binary | ios::ate);

    if (f1.fail() || f2.fail()) {
        cerr << "Error: Could not open one or both files for comparison." << endl;
        return false;
    }

    if (f1.tellg() != f2.tellg()) {
        cout << "File sizes are different." << endl;
        return false;
    }

    f1.seekg(0, ios::beg);
    f2.seekg(0, ios::beg);

    return equal(istreambuf_iterator<char>(f1), istreambuf_iterator<char>(), istreambuf_iterator<char>(f2));
}

int main() {
    srand(time(nullptr));

    produce_and_consume();

    if (check_files_identical(INPUT_FILE, OUTPUT_FILE)) {
        cout << "Files are identical. Producer and consumer are synchronized." << endl;
    } else {
        cout << "Files are not identical. Producer and consumer are not synchronized." << endl;
    }

    return 0;
}
