#include <windows.h>
#include <iostream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <map>
#include "json.hpp"


struct MapInfo 
{
    std::string OriginalNetworkAddress = "";
    std::string DriverDesc = "";
    std::string NetCfgInstanceId = "";
    std::string NameOfSubKey ="";
    bool HasMac;
    std::string DeviceInstanceID ="";
    std::string InterFaceTypeConnection = "";
    std::string LinkSpeed = "";
    std::string Status = "";
    std::string ActiveMac ="";
};

struct FinalOutPut
{
    std::string OriginalNetworkAddress = "";
    std::string DriverDesc = "";
    std::string NetCfgInstanceId = "";
    std::string NameOfSubKey ="";
    bool HasMac = false;
    std::string DeviceInstanceID ="";
    std::string InterFaceTypeConnection = "";
    std::string LinkSpeed = "";
    std::string Status = "";
    std::string ActiveMac ="";
};

using json = nlohmann::json;
using MacInfo = std::unordered_map<std::string, MapInfo>;   
using FinalInfo = std::unordered_map<std::string, FinalOutPut>;   


void FindRegistryValueaa(HKEY hKeyRoot, LPCSTR subKey, LPCSTR valueName) {
    HKEY hKey;
    LONG lResult;
    DWORD dwType;
    char data[1024];
    DWORD dataSize = sizeof(data);

    // Open the registry key
    lResult = RegOpenKeyExA(hKeyRoot, subKey, 0, KEY_READ, &hKey);
    if (lResult != ERROR_SUCCESS) {
        
        return;
    }

    // Query the registry value
    lResult = RegQueryValueExA(hKey, valueName, nullptr, &dwType, (LPBYTE)data, &dataSize);
    if (lResult == ERROR_SUCCESS) {
        
    } else {
        
    }

    // Close the registry key
    RegCloseKey(hKey);
}

void Hello(HKEY hkey1, int NumberOfSubKeys) {
    LONG aap;
    DWORD lpcSubKeys = NumberOfSubKeys;
    DWORD lpcValues;
    DWORD subKeyNameSize =0;
    for(DWORD i = 0; i < NumberOfSubKeys; i++) {
        subKeyNameSize = sizeof(i);
        aap = RegQueryInfoKeyA(hkey1, NULL, NULL, NULL,&subKeyNameSize,NULL,NULL,&lpcValues,NULL,NULL,NULL,NULL);
        if (aap == ERROR_SUCCESS) {
            
            
        }
        else {
            
        }
    }
    
}
    

bool getNumberOfSubKeys (HKEY hkey, DWORD* lpcSubKeys) {
    LONG status = RegQueryInfoKeyA(hkey,NULL,NULL,NULL,lpcSubKeys,NULL,NULL,NULL,NULL,NULL,NULL,NULL );

    return (status == ERROR_SUCCESS);

}

bool getNumberOfValues (HKEY hkey, DWORD* lpcValues) {
    LONG status = RegQueryInfoKeyA(hkey,NULL,NULL,NULL,NULL,NULL,NULL,lpcValues,NULL,NULL,NULL,NULL );

    return (status == ERROR_SUCCESS);

}





void GetValues(HKEY hKey, DWORD Index, MacInfo& macInfo, std::string& currentMacKey) {

    
    CHAR  lpValueName[256];
    DWORD lpcchValueName;
    DWORD lpType;
    BYTE  lpData[256];
    DWORD lpcbData;


   


    lpcchValueName = sizeof(lpValueName); 
    lpcbData = sizeof(lpData);
        
        LONG ttest = RegEnumValueA(hKey, Index,lpValueName, &lpcchValueName, NULL, &lpType, lpData, &lpcbData);

        if(ttest == ERROR_SUCCESS) {
          
            if (lpType == REG_SZ) {
                
                if(strcmp(lpValueName,"DriverDesc") == 0)  {

                macInfo[currentMacKey].DriverDesc = reinterpret_cast<CHAR*>(lpData);
                }

                if(strcmp(lpValueName,"DeviceInstanceID" ) == 0) {
                    macInfo[currentMacKey].DeviceInstanceID = reinterpret_cast<CHAR*>(lpData);
                }

                if(!(macInfo[currentMacKey].DriverDesc.empty())) {
            
                    if(strcmp(lpValueName,"NetCfgInstanceId" ) == 0) {

                        macInfo[currentMacKey].NetCfgInstanceId = reinterpret_cast<CHAR*>(lpData);
                        
                    }
                    
                    
                    if(strcmp(lpValueName,"OriginalNetworkAddress" ) == 0) {
                    macInfo[currentMacKey].OriginalNetworkAddress = reinterpret_cast<CHAR*>(lpData);
                    macInfo[currentMacKey].HasMac = true;
                    macInfo[currentMacKey].NameOfSubKey = currentMacKey;
                    }
            }

            } else if (lpType == REG_DWORD) {
                DWORD dwValue;
                memcpy(&dwValue, lpData, sizeof(DWORD));
                
                

        
            }
         
            else {
                
            } 

        }
    
         
        
    
    else {
        
    }
    }

    

    void getNameOfSubKeys(HKEY hkey, DWORD dwIndex, MacInfo& macInfo) {
        CHAR     lpName[256];
        DWORD   lpcchName = sizeof(lpName);
        DWORD   lpReserved;
        CHAR lpClass[256];
        DWORD   lpcchClass = sizeof(lpClass);
        FILETIME lpftLastWriteTime;
        
        CHAR TTEST[512];
        HKEY HKeyForDerivatedSubKeys;
         
        LONG a = RegEnumKeyExA(hkey, dwIndex, lpName, &lpcchName,NULL, lpClass, &lpcchClass , &lpftLastWriteTime);
        snprintf(TTEST, sizeof(TTEST), "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}\\%s", lpName );
        LONG status = RegOpenKeyExA(HKEY_LOCAL_MACHINE, TTEST, 0, KEY_READ, &HKeyForDerivatedSubKeys);
        if(a == ERROR_SUCCESS){
            std::string currentMacKey = lpName;

            
            DWORD lpcValues;
            if(getNumberOfValues(HKeyForDerivatedSubKeys, &lpcValues)){
                for(DWORD i=0; i<= lpcValues; i++) {
                    GetValues(HKeyForDerivatedSubKeys, i, macInfo, currentMacKey);
                }
                
            }
            
            
        }
        else {
           
        }
    }










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


void getExtraInfo(std::string NetCfgInstanceId, std::string NameOfSubKey, FinalInfo& finalInfo) {
    std::string quotedNetCfgInstanceId = "'" + NetCfgInstanceId + "'";
    
    std::string Command = "Get-NetAdapter -IncludeHidden | Where-Object { $_.InterfaceGuid -eq " + quotedNetCfgInstanceId + " } | Select-Object Name, LinkSpeed, Status, MacAddress | ConvertTo-Json -Compress";
    std::string powershellOutput  = runPowerShellCommand(Command);

    if (powershellOutput.empty()) {
        std::cerr << "[ERROR] PowerShell output is empty for NetCfgInstanceId: " << NetCfgInstanceId << std::endl;
        return;
    }

    try {
        json adapterInfo = json::parse(powershellOutput);

        finalInfo[NameOfSubKey].InterFaceTypeConnection = adapterInfo.value("Name", "");
        finalInfo[NameOfSubKey].LinkSpeed = adapterInfo.value("LinkSpeed", "");
        finalInfo[NameOfSubKey].Status = adapterInfo.value("Status", "");
        finalInfo[NameOfSubKey].ActiveMac = adapterInfo.value("MacAddress", "");

    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Failed to parse JSON: " << e.what() << std::endl;
        std::cerr << "Raw PowerShell Output: " << powershellOutput << std::endl;
    }
}






int main() {
    MacInfo macInfo;
    FinalInfo finalInfo;
    HKEY hKey;
    HKEY hKeyTest;
    DWORD subKeyCount;
    LPCSTR NAME = "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}\\0012";
    LPCSTR NAMETest = "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}";
    LONG status = RegOpenKeyExA(HKEY_LOCAL_MACHINE, NAME, 0, KEY_READ, &hKey);
    LONG statusa = RegOpenKeyExA(HKEY_LOCAL_MACHINE, NAMETest, 0, KEY_READ, &hKeyTest);
    

    if (status == ERROR_SUCCESS){
        
        
        if(getNumberOfSubKeys (hKeyTest, &subKeyCount)) {
            for(DWORD i =0 ; i <= subKeyCount; i++){
                getNameOfSubKeys(hKeyTest, i, macInfo);
                
                
            }
            
        }

        else  {
            
        }
        

        RegCloseKey(hKey);


    }
    else{
        
    }
    
    
    for (const auto& [mac, info] : macInfo) {
        if(info.HasMac == true) {
            

                  finalInfo[info.NameOfSubKey].OriginalNetworkAddress = info.OriginalNetworkAddress;
                  finalInfo[info.NameOfSubKey].DriverDesc = info.DriverDesc ;
                  finalInfo[info.NameOfSubKey].NetCfgInstanceId = info.NetCfgInstanceId;
                  finalInfo[info.NameOfSubKey].NameOfSubKey = info.NameOfSubKey;
                  finalInfo[info.NameOfSubKey].HasMac = true; 
                  finalInfo[info.NameOfSubKey].DeviceInstanceID = info.DeviceInstanceID ;
                  getExtraInfo(info.NetCfgInstanceId, info.NameOfSubKey, finalInfo);
            

        }
        
    }

    
    json jsonData = json::array(); // Change to array format

    for (const auto& [key, value] : finalInfo) {
        json jsonEntry = {
            {"OriginalNetworkAddress", value.OriginalNetworkAddress},
            {"DriverDesc", value.DriverDesc},
            {"NetCfgInstanceId", value.NetCfgInstanceId},
            {"NameOfSubKey", value.NameOfSubKey},
            {"HasMac", value.HasMac},
            {"DeviceInstanceID", value.DeviceInstanceID},
            {"InterFaceTypeConnection",value.InterFaceTypeConnection},
            {"LinkSpeed",value.LinkSpeed},
            {"Status",value.Status},
            {"ActiveMacAddress",value.ActiveMac},

            
        };
        jsonData.push_back(jsonEntry); // Store each entry in an array
    }
    
    std::cout << jsonData.dump(4) << std::endl; // Output JSON



    



}


