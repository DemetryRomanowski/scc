//
// Created by Demetry on 10/4/2017.
// Copyright 2017 Cognizant Software Solutions
//

#include <iostream>

#include <cstring>
#include <fstream>

#include "headers/FileUtils.h"

std::ifstream in_file;
std::string input_file;
std::string output_file;

/**
 * Compile file declaration
 */
void compile_file();

/**
 * Compile the wild card files
 */
void compile_wild_card(const std::string & /*path*/, int /*i*/, std::vector<std::string> * /*compiled_output*/);

/**
 * Main entry point to the program
 * @param argc The count of arguments
 * @param argv The array pointer containing all the arguments
 * @return The program status after finishing
 */
int main(int argc, char *argv[]) {
    if (argc == 1) {

        //see if the output file already exists
        if (FileUtils::exists(std::string("main.sql")))
        {
            std::wcerr << "Default output file already exists please delete or move it. " << std::endl;
        }

        //look for default files
        if (!FileUtils::exists(std::string("makesqlc"))) {
            if (!FileUtils::exists(std::string("make.sqlc"))) {
                std::cout << "Auto mode cannot find your sqlc make script exiting ..." << std::endl;
                return EXIT_FAILURE;
            }
            input_file = "make.sqlc";
        } else input_file = "makesqlc";

        output_file = "main.sql";

        compile_file();

        return EXIT_SUCCESS;
    }

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            if (argc < (i + 1)) {
                std::cout << "Cannot define -o switch without adding argument" << std::endl;
                return EXIT_SUCCESS;
            }
            output_file = argv[i + 1];

            if (FileUtils::exists(output_file))
            {
                std::wcerr << "Output file will be over written" << std::endl;
                if( remove(output_file.c_str()) != 0)
                {
                    perror("Error with deleting the file");
                    return EXIT_FAILURE;
                }
            }

        }
    }

    if (output_file.empty() || output_file == " ") {
        std::cout << "Usage: scc main.sqlc -o db.sql" << std::endl;
        return EXIT_SUCCESS;
    }

    input_file = argv[1];

    compile_file();

    return EXIT_SUCCESS;
}

/**
 * Compile the file
 */
void compile_file() {
    try {
        in_file = std::ifstream(input_file);

        std::vector<std::string> lines = std::vector<std::string>();
        std::string tmp;
        std::vector<std::string> compiled_output = std::vector<std::string>();

        bool compiling_script = false;

        //Push all the lines into the vector
        while (std::getline(in_file, tmp))
            lines.push_back(tmp);

        //Line number
        int line_num = 0;

        //Read the lines of the file
        for (std::string line : lines) {
            line_num++;

            if (!line.empty()) {
                //Ignore comment line
                if (line.find('-', 0) != line.npos && line.find('-', 1) != line.npos) continue;

                //Check if compiling script
                if (compiling_script) {
                    if (line.find("endscript") != line.npos) {
                        compiling_script = false;
                        continue;
                    }

                    compiled_output.push_back(line);
                    continue;
                }

                //Check for include tag
                if (line.find("include") != line.npos) {
                    //Check if there is a wild card
                    if (line.find('*') != line.npos) {
                        std::string include_path_raw = StringUtils::split(line, ' ')[1];
                        std::string include_path = StringUtils::split(include_path_raw, '*')[0];

                        if (include_path == "*.sql") {
                            compile_wild_card(FileUtils::get_current_directory(), line_num, &compiled_output);
                        } else {
                            compile_wild_card(include_path, line_num, &compiled_output);
                        }
                    //Include the file in the output
                    } else {
                        std::string inc_line;
                        std::string input_file_path = StringUtils::trim_copy(StringUtils::split(line, ' ')[1]);

                        std::ifstream inc_stream(input_file_path);

                        if (inc_stream.is_open()) {
                            while (std::getline(inc_stream, inc_line))
                                compiled_output.push_back(inc_line + "\n");
                        } else {
                            std::cerr << "Could not open include file at line: " << line_num << std::endl;
                            break;
                        }
                    }
                //Check for script tag
                } else if (line.find("script") != line.npos) {
                    compiling_script = true;
                    continue;
                }
            }
        }

        //Create a file stream
        std::ofstream sqlfile;

        //Open the output file
        sqlfile.open(output_file);

        //Write to the file
        for (const auto &output : compiled_output) {
            std::cout << output << std::endl;
            sqlfile << output;
        }

        //Close the file
        sqlfile.close();
    }
    catch (std::exception &e) {
        //Some kind of error occurred
        std::cerr << "Unknown error has occurred: " << e.what() << std::endl;
        in_file.close();
    }
}

/**
 * Compile the wild card files
 * @param path The path to the the files
 * @param compiled_output The compiled output vector
 */
void compile_wild_card(const std::string &path, int i, std::vector<std::string> *compiled_output) {
    std::vector<std::string> files = FileUtils::get_files(path, ".sql");

    if (files.empty()) {
        std::cerr << "Could not load include directory at line: " << std::endl;
    }

    for (std::string &input_file_path : files) {
        std::string inc_line;

        std::ifstream inc_stream(path + input_file_path);

        if (inc_stream.is_open()) {
            while (std::getline(inc_stream, inc_line)) {
                inc_line += "\n";
                compiled_output->push_back(inc_line);
            }
        } else {
            std::cerr << "Could not open include file at line: " << i << std::endl;
        }
    }
}