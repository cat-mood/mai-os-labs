#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::string filename;
    std::string outname;
    std::cout << "Enter filename: ";
    std::cin >> filename;
    std::cout << "Enter path to save: ";
    std::cin >> outname;
    std::ifstream in(filename);
    std::ofstream out(outname, std::ios::binary | std::ios::out);
    out.seekp(sizeof(int), std::ios::beg);
    std::string word;
    int count = 0;
    while (in >> word) {
        out.write(word.c_str(), sizeof(char) * 5);
        ++count;
    }
    out.seekp(0, std::ios::beg);
    out.write(reinterpret_cast<char*>(&count), sizeof(int));
    in.close();
    out.close();
    std::ifstream test(outname, std::ios::binary | std::ios::in);
    int num;
    test.read(reinterpret_cast<char*>(&num), sizeof(int));
    std::cout << num << " words has been written" << std::endl;
    char raw_word[5];
    test.read(reinterpret_cast<char*>(raw_word), sizeof(char) * 5);
    std::string new_word(raw_word);
    std::cout << "First word is " << new_word << std::endl;
    test.close();

    return 0;
}
