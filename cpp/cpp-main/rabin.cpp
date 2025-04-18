#include <iostream>
#include <string>

long long calculateHash(const std::string& s) {
    long long hashValue = 1;
    for (char c : s) {
        hashValue *= static_cast<long long>(c);
    }
    return hashValue;
}

void rabin(const std::string& text, const std::string& pattern) {
    long long patternHash = calculateHash(pattern);
    int patternLen = pattern.size();
    int textLen = text.size();

    for (int i = 0; i <= textLen - patternLen; ++i) {
        std::string substring = text.substr(i, patternLen);
        long long substringHash = calculateHash(substring);

        if (substringHash == patternHash) {
            if (substring == pattern) {
                std::cout << "nah" << i << std::endl;
            }
        }
    }
}

int main() {
    std::string text = "hello world";
    std::string pattern = "world";
    rabin(text, pattern);
    return 0;
}
