#include <iostream>
#include <cstdio>
#include <string>
#include <regex>
#include <windows.h>
#include "json.hpp"

using json = nlohmann::json;

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
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <AdapterName>\n";
        return 1;
    }

    std::string adapterName = argv[1];
    std::string quotedName = "'" + adapterName + "'";
    std::string psCommand =
        "Get-WmiObject Win32_NetworkAdapter | Where-Object { $_.Name -eq " + quotedName +
        " } | Select-Object Name, MACAddress, NetEnabled";

    std::string output = runPowerShellCommand(psCommand);

    if (output.empty()) {
        std::cerr << "No output from PowerShell.\n";
        return 1;
    }

    // Use regex to find MAC address (format: XX:XX:XX:XX:XX:XX or with dashes)
    std::regex macRegex(R"(([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2}))");
    std::smatch match;

    json result;
    if (std::regex_search(output, match, macRegex)) {
        std::string mac = match.str();
        std::replace(mac.begin(), mac.end(), ':', '-');  // Normalize to dashes
        result[adapterName] = mac;
    } else {
        result["error"] = "No MAC address found";
    }

    std::cout << result.dump(4) << std::endl;
    return 0;
}
