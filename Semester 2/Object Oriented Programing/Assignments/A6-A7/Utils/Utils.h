#pragma once
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace Utils {
    // Removes leading and trailing whitespaces
    inline string trim(const string& stringToTrim) {
        string result(stringToTrim);
        size_t position = result.find_first_not_of(" \t\r\n");
        if (position != string::npos)
            result.erase(0, position);
        
        position = result.find_last_not_of(" \t\r\n");
        if (position != string::npos)
            result.erase(position + 1);
        
        return result;
    }

    // Splits a string into a vector of strings based on a delimiter
    inline vector<string> tokenize(const string& stringToTokenize, char delimiter) {
        vector<string> result;
        stringstream ss(stringToTokenize);
        string token;
        while (getline(ss, token, delimiter)) {
            token = trim(token);
            if (!token.empty()) {
                result.push_back(token);
            }
        }
        return result;
    }
} 