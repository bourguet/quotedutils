// ccat.cpp

#include <assert.h>
#include <iomanip>
#include <iostream>
#include <locale>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <unistd.h>

#include "cquote.hpp"

void usage()
{
    std::cerr << "Usage: ccat [-hls] files...\n";
}

void help()
{
    usage();
    std::cerr <<
        "\n"
        "Shows the files content with C syntax.\n"
        "Options\n"
        "-h\tthis help\n"
        "-l\tuse locale instead of C locale\n"
        "-s\tmake valid strings\n";
}

void ccat(std::istream& is, bool useLocale, bool makeString)
{
    std::wstring prefix = makeString ? L"\"" : L"";
    std::wstring postfix = makeString ? L"\\n\"\n" : L"\n";
    std::string line;
    while (getline(is, line)) {
        std::wstring quoted = cquote(line, useLocale);
        std::wcout << prefix << cquote(line, useLocale) << postfix;
    }
}

int main(int argc, char **argv)
{
    int status = EXIT_SUCCESS;
    try {
        bool useLocale = false;
        int errcnt = 0;
        bool makeStrings = false;
        int c;

        while ((c = getopt(argc, argv, "hls")) != -1) {
            switch (c) {
            case 'l':
                useLocale = true;
                break;
            case 'h':
                help();
                return EXIT_SUCCESS;
            case 's':
                makeStrings = true;
                break;
            case '?':
                errcnt++;
                if (isprint(optopt))
                    std::cerr << "Unknown option '-" << optopt << "'\n";
                else
                    std::cerr << "Unknown option character '-\\x" << std::hex << std::setw(2) << (int) optopt << "'\n";
                break;
            default:
                assert(0);
                break;
            }
        }

        if (errcnt > 0) {
            usage();
            return EXIT_FAILURE;
        }

        if (useLocale)
            std::locale::global(std::locale(""));

        if (optind == argc) {
            ccat(std::cin, useLocale, makeStrings);
        } else {
            for (int i = optind;  i < argc; ++i) {
                std::fstream is(argv[i]);
                if (is) {
                    ccat(is, useLocale, makeStrings);
                } else {
                    std::cerr << "Unable to open '" << argv[i] << "'\n";
                }
            }
        }

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        status = EXIT_FAILURE;
    } catch (int s) {
        status = s;
    } catch (...) {
        std::cerr << "Unexpected and unknown exception.\n";
        status = EXIT_FAILURE;
    }
    return status;
}
