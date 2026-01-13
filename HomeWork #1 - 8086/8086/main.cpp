#include <iostream>
#include <fstream>
#include <map>
using namespace std;


struct Register {
    char highByte = 0;
    char lowByte = 0;
};


//struct ProcessorMemory {
//    std::map<Register>
//};

std::map<char, std::string> wideRegistersNames = {{0b0, "AX"},
                                                 {0b001, "CX"},
                                                 {0b010, "DX"},
                                                 {0b011, "BX"},
                                                 {0b100, "SP"},
                                                 {0b101, "BP"},
                                                 {0b110, "SI"},
                                                 {0b111, "DI"}};

std::map<char, std::string> byteRegisterNames = {{0b0, "AL"},
                                                {0b001, "CL"},
                                                {0b010, "DL"},
                                                {0b011, "BL"},
                                                {0b100, "AH"},
                                                {0b101, "CH"},
                                                {0b110, "DH"},
                                                {0b111, "BH"}};

enum Mode {
    MemMode = 0b00,
    MemMode8Bit = 0b00000001,
    MemMode16Bit = 0b00000010,
    RegMode = 0b00000011
};

namespace Instructions {
    const char MOV = 0b00100010;
}

std::map<char, std::string> instructionToString = {{Instructions::MOV,"MOV"}};


int main(int argc, char** argv)
{
    if (argc <= 1) {
        std::cout << "Please provide bin file in 16 bit architecture" << std::endl;
    }
    char* fileName = argv[1];


    ifstream filestream(fileName, std::ios::binary);
    if (filestream) {
        char inputHigh;
        char inputLow;
        while (filestream >> inputHigh) {
            filestream >> inputLow;

            bool dest = (inputHigh &= 2) == 1;
            bool wide = (inputHigh &= 1) == 1;

            char srcReg;
            char destReg;

            if (dest) {
                srcReg = (inputLow >> 3) & 7;
                destReg = inputLow & 7;
            } else {
                destReg = (inputLow >> 3) & 7;
                srcReg = inputLow & 7;
            }
            char mode = inputLow << 6;


            std::string srcRegName;
            std::string destRegName;
            if (mode == Mode::RegMode) {
                if (wide) {
                    srcRegName = wideRegistersNames[srcReg];
                    destRegName = wideRegistersNames[destReg];
                } else {
                    srcRegName = byteRegisterNames[srcReg];
                    destRegName = byteRegisterNames[destReg];
                }
            }
            char instruction = inputHigh >> 2;
            std::string instructionName = instructionToString[instruction];
            std::cout << instructionName << " " << destRegName << "," << srcRegName << std::endl;

            switch (instruction) {
                case Instructions::MOV:
                    break;
//                default:
//                    std::cout << "unknown instruction" << std::endl;
            }
        }

    }

//    cout << "Hello World!" << endl;
    return 0;
}
