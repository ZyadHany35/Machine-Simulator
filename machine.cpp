#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
class Memory {
protected:
    int size;
    vector <string> memoryData;
public:
    Memory(int size) :size(size) {
        memoryData.resize(size, "00");
    }
    void storemem(int adress, const string& data) {
        memoryData[adress] = data;
    }
    void clearAll() {
        fill(memoryData.begin(), memoryData.end(), "00");
    }
    const string& getmemData(int adress) {
        return memoryData[adress];
    }
    int getSize() {
        return size;
    }

class Machine {
    Memory ram;
    CPU cpu;
    string screen = "";
public:
    Machine() :ram(256), cpu(16, ram) {}
    void load(string fileName) {
        ifstream file(fileName);
        int address = 0;
        string hexValue;
        int intValue;
        int i = 0;
        while (getline(file, hexValue)) {
            if (hexValue.size() == 4) {
                ram.storemem(i, hexValue.substr(0, 2));
                ram.storemem(i + 1, hexValue.substr(2, 2));
                i += 2;
            }
            else {
                cerr << "Invalid hex string: " << hexValue << endl;
            }
        }
        file.close();
    }
    void displayStatus() {
        // عرض حالة المسجلات
        cout << "Registers:" << endl;
        for (int i = 0; i < cpu.Registers.getSize(); i++) {
            cout << "Register " << hex << i << ": " << cpu.Registers.getmemData(i) << endl;
        }

        cout << "Memory:" << endl;
        for (int i = 0; i < ram.getSize(); i++) {
            cout << "Address " << hex << i << ": " << ram.getmemData(i) << endl;
        }

    }
    void run() {
        cpu.run();
    }
};
