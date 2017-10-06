//
// Created by Demetry on 10/4/2017.
// Copyright 2017 Cognizant Software Solutions
//

#ifndef SCC_STRINGUTILS_H
#define SCC_STRINGUTILS_H

#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>

class StringUtils {
private:
    /**
     * split helper method
     * @tparam Out
     * @param s string to split
     * @param delim the deliminator
     * @para
     * m result, the output of splitting
     */
    template<typename Out>
    static inline void split(const std::string &s, char delim, Out result) {
        std::stringstream ss;
        ss.str(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            *(result++) = item;
        }
    }

public:

    /**
     * Split a string by a deliminator
     * @param s The string to split
     * @param delim The deliminator
     * @return The split strings as vector
     */
    static inline std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, std::back_inserter(elems));
        return elems;
    }

    /**
     * Trim from the start inplace
     * @param s The string to trim
     */
    static inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
            return !std::isspace(ch);
        }));
    }

    /**
     * Trim from the end inplace
     * @param s The string to trim
     */
    static inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    /**
     * Inplace trim function
     * @param s The string to trim
     */
    static inline void trim(std::string &s)
    {
        ltrim(s);
        rtrim(s);
    }

    /**
     * Trim characters copy
     * @param s
     * @return Trimmed string
     */
    static inline std::string trim_copy(std::string s)
    {
        trim(s);
        return s;
    }
};


#endif //SCC_STRINGUTILS_H
