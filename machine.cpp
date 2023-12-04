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
