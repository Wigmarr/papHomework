#include <iostream>
#include <fstream>
#include <map>
#include <bitset>
using namespace std;

struct Register
{
    u_char highByte = 0;
    u_char lowByte = 0;
};

// struct ProcessorMemory {
//     std::map<Register>
// };

std::map<u_char, std::string> wideRegistersNames = {{0b0, "ax"},
                                                    {0b001, "cx"},
                                                    {0b010, "dx"},
                                                    {0b011, "bx"},
                                                    {0b100, "sp"},
                                                    {0b101, "bp"},
                                                    {0b110, "si"},
                                                    {0b111, "di"}};

std::map<u_char, std::string> byteRegisterNames = {{0b0, "al"},
                                                   {0b001, "cl"},
                                                   {0b010, "dl"},
                                                   {0b011, "bl"},
                                                   {0b100, "ah"},
                                                   {0b101, "ch"},
                                                   {0b110, "dh"},
                                                   {0b111, "bh"}};

enum Mode
{
    MemMode = 0b00,
    MemMode8Bit = 0b00000001,
    MemMode16Bit = 0b00000010,
    RegMode = 0b00000011
};

namespace Instructions
{
    namespace MOV {
    const u_char MOV_REG_MEM_TO_FROM_REG = 0b100010;
    const u_char MOV_IMID_TO_REG_MEM = 0b1100011; 
    const u_char MOV_IMID_TO_REG = 0b1011;
    const u_char MOV_MEM_TO_ACC = 0b1010000;
    const u_char MOV_ACC_TO_MEM = 0b1010001;
    

    }
}

std::map<u_char, std::string> instructionToString = {{Instructions::MOV::MOV_REG_MEM_TO_FROM_REG, "mov"},
                                                    {Instructions::MOV::MOV_IMID_TO_REG_MEM, "mov"},
                                                    {Instructions::MOV::MOV_IMID_TO_REG, "mov"},
                                                    {Instructions::MOV::MOV_MEM_TO_ACC, "mov"},
                                                    {Instructions::MOV::MOV_ACC_TO_MEM, "mov"}};

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        std::cout << "Please provide bin file in 16 bit architecture" << std::endl;
    }
    char *fileName = argv[1];

    ifstream filestream(fileName, std::ios::binary);
    if (filestream)
    {
        u_char inputHigh;
        u_char inputLow;
        std::cout << "bits 16" << std::endl
                  << std::endl;
        while (filestream >> inputHigh)
        {
            filestream >> inputLow;
            std::cout << static_cast<bitset<8>>(inputHigh) << std::endl;
            std::cout << static_cast<bitset<8>>(inputLow) << std::endl;
            bool dest = (inputHigh & 2) == 1;
            bool wide = (inputHigh & 1) == 1;

            u_char srcReg;
            u_char destReg;

            if (dest)
            {
                destReg = (inputLow >> 3) & 0b111;
                srcReg = inputLow & 7;
            }
            else
            {
                srcReg = (inputLow >> 3) & 0b111;
                destReg = inputLow & 7;
            }
            u_char mode = inputLow >> 6;

            std::string srcRegName;
            std::string destRegName;
            // std::cout << static_cast<std::bitset<8>>(mode);
            if (mode == Mode::RegMode)
            {
                // std::cout << "Entered reg mode";
                if (wide)
                {
                    srcRegName = wideRegistersNames[srcReg];
                    destRegName = wideRegistersNames[destReg];
                }
                else
                {
                    srcRegName = byteRegisterNames[srcReg];
                    destRegName = byteRegisterNames[destReg];
                }
            }
            u_char instruction = inputHigh;
            for (int i = 0; i <= 8; ++i) {
                instruction >>= 1;
                if (instructionToString.find(instruction) != instructionToString.end()) {
                    break;
                }
            }
            if (instruction == 0) {
                std::cout << "unknown instruction: " << std::bitset<8>(inputHigh);
                return 0;
            }
            // std::cout << static_cast<std::bitset<8>>(inputHigh);

            // std::cout << static_cast<std::bitset<8>>(instruction);
            std::string instructionName = instructionToString[instruction];
            std::cout << instructionName << " " << destRegName << ", " << srcRegName << std::endl;

            switch (instruction)
            {
            case Instructions::MOV::MOV_REG_MEM_TO_FROM_REG:
                break;
                //                default:
                //                    std::cout << "unknown instruction" << std::endl;
            }
        }
    }

    //    cout << "Hello World!" << endl;
    return 0;
}
