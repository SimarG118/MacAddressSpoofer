#include <windows.h>
#include <iostream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <map>
#include "json.hpp"



int main(int argc, char* argv[]) {
    

    
    std::string NewMacAddress = argv[1];
    std::string subKey = argv[2];
    const char* MacChangeName = "NetworkAddress";
    

    std::cout << "Received MAC: " << NewMacAddress << std::endl;
    std::cout << "Received SubKey: " << subKey << std::endl;

    CHAR NewMacAddressChar[256];
    strcpy_s(NewMacAddressChar, sizeof(NewMacAddressChar), NewMacAddress.c_str());
    CHAR NAMETest[256];
    
    CHAR     lpName[256];
    strcpy_s(lpName, sizeof(lpName), subKey.c_str());

    snprintf(NAMETest, sizeof(NAMETest), "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}\\%s", lpName );

    HKEY hKey;
    LONG Result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, NAMETest, 0, KEY_SET_VALUE, &hKey);
    std::cout<< NAMETest;
    if(Result == ERROR_SUCCESS) {
        Result = RegSetValueExA(hKey, MacChangeName, 0,REG_SZ, (const BYTE*)NewMacAddressChar, strlen(NewMacAddressChar) + 1);
        if (Result != ERROR_SUCCESS) {
            std::cout << "RegSetValueExA failed" << std::endl;
            
        }
    }
    else {
        std::cout << "error";
    }

    return 0;
}


