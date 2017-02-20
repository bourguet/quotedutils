// showargs.cpp

#include <iomanip>
#include <iostream>
#include <locale>
#include <stdlib.h>
#include <string>

#include "cquote.hpp"

int main(int argc, char **argv)
{
    int status = EXIT_SUCCESS;
    try {
        bool useLocale = false;
        int done = 0;

        for (int i = 1; !done && i < argc; ++i) {
            char* ptr;
            if (argv[i][0] != '-')
                break;
            if (argv[i][1] == '-')
                break;
            for (ptr = argv[i] + 1; !done && *ptr != '\0'; ++ptr) {
                switch (*ptr) {
                case 'l':
                    useLocale = true;
                    break;
                default:
                    done = 1;
                    break;
                }
            }
        }

        if (useLocale)
            std::locale::global(std::locale(""));

        for (int i = 1; i < argc; ++i) {
            std::wstring quoted;
            try {
                quoted = (useLocale ? L"L\"" : L"\"") + cquote(argv[i], useLocale) + L"\"";
            } catch (...) {
                quoted = L"encoding error \"" + cquote(argv[i], false) + L"\"";
            }
            std::wcout << std::setw(2) << i << L": " << quoted << L"\n";
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
