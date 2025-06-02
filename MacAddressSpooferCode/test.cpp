#include <iostream>
#include <cstdio>
#include <string>
#include <algorithm>
#include <regex>
#include <windows.h>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

std::string runPowerShellCommand(const std::string& command) {
    std::string fullCommand = "powershell -Command \"& { " + command + " }\"";

    FILE* pipe = _popen(fullCommand.c_str(), "r");
    if (!pipe) {
        std::cerr << "Failed to run PowerShell command!" << std::endl;
        return "";
    }

    char buffer[256];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    _pclose(pipe);
    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 6) {
        std::cerr << "Usage: " << argv[0] << " <MacAddressToTest> <AdapterName> <DeviceInstanceID>\n";
        return 1;
    }

    std::string macAddressToTest = argv[1];
    std::string NetCfgInstanceId = argv[2];
    std::string deviceID = argv[3];
    std::string NameofAdapter = argv[4];
    std::string IfRunning = argv[5];

    std::string quotedNameOfAdapter = "'" + NameofAdapter + "'";
    std::string quotedDeviceID = "'" + deviceID + "'";
    std::string quotedNetCfgInstanceId = "'" + NetCfgInstanceId + "'";

    std::string disableCommand = "pnputil /disable-device " + quotedDeviceID;
    std::string enableCommand  = "pnputil /enable-device " + quotedDeviceID;
    


    
    
    

    // fix later to run depending on if i want to or not (add another condition/ variable entry)
    runPowerShellCommand(disableCommand);
    Sleep(8000);
    runPowerShellCommand(enableCommand);

    std::string psQuery = "Get-WmiObject Win32_NetworkAdapter | Where-Object { $_.Name -eq " + quotedNameOfAdapter + " } ";
    std::string psOutput = runPowerShellCommand(psQuery);

    if (psOutput.empty()) {
        std::cerr << "Failed to get adapter info from PowerShell.\n";
        return 1;
    }

    // Use regex to extract MAC address
    std::regex macRegex(R"(([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2}))");
    std::smatch match;

    std::string foundMac ="";
    if (std::regex_search(psOutput, match, macRegex)) {
        foundMac = match.str();
        std::string normalizedMac = foundMac;
        normalizedMac.erase(std::remove(normalizedMac.begin(), normalizedMac.end(), ':'), normalizedMac.end());
        normalizedMac.erase(std::remove(normalizedMac.begin(), normalizedMac.end(), '-'), normalizedMac.end());

        bool same = (normalizedMac == macAddressToTest);
        json resultJson;
        resultJson["original_input"] = macAddressToTest;
        resultJson["found_mac"] = normalizedMac;
        resultJson["match"] = same;
        resultJson["NetCfgInstanceId"] = NetCfgInstanceId;
        resultJson["AdapterName"] = NameofAdapter;
        

        std::cout << "\n" << resultJson.dump(4) << std::endl;
    } else {
        std::cerr << "No MAC address found in PowerShell output.\n";
        return 1;
    }

    return 0;
}
