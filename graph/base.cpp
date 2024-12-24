#include <iostream>
#include <print>
#include <string>
#include <fstream>
#include <unordered_map>
#include <set>

// id ∈ [0, 2^32 − 1]
// weight ∈ [0, 2^8 − 1]
int
main(int argc, char *argv[])
{
    if (argc != 6) {
        std::println("Number of cmd line args = {} = is less than 6!!", argc);
        exit(1);
    }

    std::string option = argv[1];

    if (option == "-s") {
        // serialization
        std::ifstream inputFile(argv[3]);
        if (!inputFile) {
            std::println("File {} doean't exist!!", argv[3]);
            exit(1);
        }
        std::ofstream outputFile(argv[5], std::ios_base::binary | std::ios::out | std::ios::trunc);

        uint32_t id1, id2;
        uint32_t weight;

        while (inputFile >> id1 >> id2 >> weight) {
            uint8_t w2 = static_cast<uint8_t>(weight);
            outputFile.write(reinterpret_cast<const char *>(&id1), sizeof(id1));
            outputFile.write(reinterpret_cast<const char *>(&id2), sizeof(id2));
            outputFile.write(reinterpret_cast<const char *>(&w2), sizeof(w2));
        }

        inputFile.close();
        outputFile.close();

    } else if (option == "-d") {
        // deserialization
        std::ifstream inputFile(argv[3], std::ios_base::binary);
        if (!inputFile) {
            std::println("File {} doean't exist!!", argv[5]);
            exit(1);
        }
        std::ofstream outputFile(argv[5], std::ios::out | std::ios::trunc);

        uint32_t id1, id2;
        uint8_t weight;

        while (inputFile.read(reinterpret_cast<char *>(&id1), sizeof(id1))) {

            inputFile.read(reinterpret_cast<char *>(&id2), sizeof(id2));
            inputFile.read(reinterpret_cast<char *>(&weight), sizeof(weight));

            outputFile << id1 << '\t' << id2 << '\t' << static_cast<int>(weight) << '\n';
        }

        inputFile.close();
        outputFile.close();
    } else {
        std::println("Wrong parameter: {}! It must be '-s' or '-d'!!", option);
        exit(1);
    }
}
