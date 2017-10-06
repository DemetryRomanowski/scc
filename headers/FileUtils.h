//
// Created by Demetry on 10/4/2017.
// Copyright 2017 Cognizant Software Solutions
//

#ifndef SCC_FILEUTILS_H
#define SCC_FILEUTILS_H

#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <dirent.h>

#include "StringUtils.h"

#ifdef  _WIN32
    #include <io.h>
    #include <direct.h>

#define GetCurrentDir _getcwd
#elif __unix__
    #include <unistd.h>
    #include <zconf.h>
    #define GetCurrentDir getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

class FileUtils {
    //Private variables
private:

public:
    /**
     * Check if file exists
     * @param path The path of the file
     * @return True/False if it exists or not
     */
    static inline bool exists(const std::basic_string<char> &path) {
        return ( access( path.c_str(), F_OK ) != -1 );
    }

    /**
     * Get all the file names in a directory
     * @param path the path to search
     * @return
     */
    static inline std::vector<std::string> get_files(const std::basic_string<char> &path, const std::basic_string<char> &ext){
        std::vector<std::string> _ret = std::vector<std::string>();

        DIR *dir;

        struct dirent *ent;

        std::string modified_path = path;

        if(path.empty())
            modified_path = get_current_directory();

        if((dir = opendir(modified_path.c_str())) != nullptr)
        {
            while((ent = readdir(dir)) != nullptr)
            {
                std::string test_string = ent->d_name;

                if(!(test_string == "." || test_string == "..")) {
                    if(StringUtils::split(test_string, '.')[1] == ext) {
                        _ret.emplace_back(ent->d_name);
                    }
                }

            }

            closedir(dir);
        }
        else
        {
            throw errno;
        }

        return _ret;
    }

    /**
     * Get the current working directory
     * @return The current directory path
     */
    static inline std::string get_current_directory()
    {
        char cCurrentPath[FILENAME_MAX];

        if(!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
            throw errno;

        cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';

        std::string ret = cCurrentPath;

        return ret;
    }
};

#endif //SCC_FILEUTILS_H