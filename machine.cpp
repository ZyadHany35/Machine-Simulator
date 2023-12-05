#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

int hexTOint(const string& hexString) {
    int intValue = stoi(hexString, nullptr, 16);
    return intValue;
}

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
    friend class Bus;
};
class CPU {
    Memory Registers;
    Memory* memory;
    int PC;
    string IR;
    bool execution;
    string screen;

public:
    CPU(int size, Memory& mem) : Registers(size), memory(&mem),PC(0), IR("0000"), execution(true) {}

    void fetch(int pc) {
        IR = memory->getmemData(pc) + memory->getmemData(pc + 1);
    }

    int decode() {
        string opcode = IR.substr(0, 1);
        string operand = IR.substr(1, 3);
        switch (hexTOint(opcode)) {
        case 1:
            return 1;
            break;
        case 2:
            return 2;
            break;
        case 3: {
            if (operand.substr(1, 2) == "00")
                return 32;
            else return 31;
            break;
        }
        case 4:
            return 4;
            break;
        case 5:
            return 5;
            break;
        case 11:
            return 11;
            break;
        case 12:
            return 12;
            break;
        }
    }
    void execute(int insCode) {
        string operand = IR.substr(1, 3);
        int result;
        stringstream ss;
        string hexString;
        switch (insCode) {
        case 1:
            Registers.storemem(hexTOint(operand.substr(0, 1)), memory->getmemData(hexTOint(operand.substr(1, 2))));
            break;
        case 2:
            Registers.storemem(hexTOint(operand.substr(0, 1)), operand.substr(1, 2));
            break;
        case 31: {
            memory->storemem(hexTOint(operand.substr(1, 2)), Registers.getmemData(hexTOint(operand.substr(0, 1))));
            break;
        }
        case 32:
            screen = Registers.getmemData(hexTOint(operand.substr(0, 1)));
            break;
        case 4:
            Registers.storemem(hexTOint(operand.substr(2, 1)), Registers.getmemData(hexTOint(operand.substr(1, 1))));
            break;

        case 5:
            result = (hexTOint(Registers.getmemData(hexTOint(operand.substr(1, 1)))) +
                hexTOint(Registers.getmemData(hexTOint(operand.substr(2, 1)))));
            if (result > 255)
                result = 255;
            ss << hex << result;
            hexString = ss.str();
            while (hexString.length() < 2) {
                hexString = "0" + hexString;
            }

            Registers.storemem(hexTOint(operand.substr(0, 1)), hexString);
            break;

        case 11:
            if (Registers.getmemData(hexTOint(operand.substr(0, 1))) == Registers.getmemData(0)) {
                PC = (hexTOint(operand.substr(1, 2)));
            }
            break;
        case 12:
            execution = false;
            break;
        default:
            cout << "Invalid opcode: " << IR.substr(0, 1) << endl;
            execution = false;
            break;
        }
    }
    void run() {
        int pc = 0;
        while (execution) {
            fetch(pc);
            pc += 2;
            execute(decode());
        }
        cout << "Screen: " << screen << endl;
    }

    friend class Machine;
};


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


int main() {
    Machine MyVoleMachine;
    MyVoleMachine.load("program.txt");
    MyVoleMachine.run();
    MyVoleMachine.displayStatus();
}
