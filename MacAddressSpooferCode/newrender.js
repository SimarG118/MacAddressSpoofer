window.electronAPI.sendCppRun(); 

const orginalMacAddressTitle = document.querySelector(".original-mac");
const ActiveMacAddressTitle = document.querySelector(".active-mac");

const linkStatus = document.querySelector(".link-status");
const speedID = document.querySelector(".speed-id");
const ActiveMac = document.querySelector(".active-mac-address");
const titleForInfo = document.querySelector(".title-for-info");
const orginalMacAddress = document.querySelector(".orginal-mac-address");
const ChangeMacConfirmButton = document.getElementById('ChangeMacConfirm');
const ClickBox = document.getElementById("click-box-button");
const RandomButton = document.querySelector(".RandomButtonReal");


function clickedOnMac() {
    document.querySelector(".original-mac").classList.remove("hidden");
    document.querySelector(".active-mac").classList.remove("hidden");
    document.querySelector(".Change-Mac-Title").classList.remove("hidden");
    document.querySelector(".RandomButtonReal").classList.remove("hidden");
    document.querySelector(".ChangeMacConfirm").classList.remove("hidden");
    document.querySelector(".boxSearch").classList.remove("hidden");
    
}

function hideLoadingScreen(){
    document.getElementById("loading-screen").style.display = "none";
    
}

function yolo(output){
    const parsed = typeof output === "string" ? JSON.parse(output) : output;
    if (parsed.AdapterName !== "Microsoft Kernel Debug Network Adapter") {
        window.electronAPI.MacChecker({ str1: parsed.AdapterName });
    } else {
        alert("error");
    }
}



window.electronAPI.onMacChecker((output) => {
    console.log("MacChecker Output (testmacaddress):", output);

    if (!output || output.trim() === '') {
        console.log("Empty output received from MacChecker");
        return;
    }

    try {
        const parsedResult = JSON.parse(output);
        const adapterNames = Object.keys(parsedResult);

        if (adapterNames.length > 0) {
            const adapterName = adapterNames[0];
            const mac = parsedResult[adapterName];
            if (ActiveMac.innerText !== mac) {
                ActiveMac.innerText = mac;
                alert("Successfully changed mac address");
            }
            else {
                alert("failed, try changing the first octet to 02 or network adapter mac address is related to another similar one ");
            }
        } else {
            console.warn("No adapter names found in output.");
        }

    } catch (error) {
        alert("Error: Output is not valid JSON.\n\n" + output);
        console.error("Failed to parse testmacaddress output:", error);
    }
});





const Name = document.querySelector(".link-status");
const flexTable = document.querySelector(".flexTable")

function clearAllHighlights() {
    document.querySelectorAll(".Start").forEach(el => el.classList.remove("hover-for-start"));
    document.querySelectorAll(".Data").forEach(el => el.classList.remove("hover-for-data"));
    document.querySelectorAll(".End").forEach(el => el.classList.remove("hover-for-end"));
}

let parsedDataFromTestResults = null;
window.electronAPI.onTestResults((output)=> {
    console.log("Raw Test Results:", output);
    try{
        const parsedDataa = JSON.parse(output);
        parsedDataFromTestResults = parsedDataa;
        yolo(parsedDataFromTestResults);
    }

    catch(error) {
        alert("errrrror:\n" + error.message + "\n\nRaw Output:\n" + output);
    console.error("JSON parse error:", error);
    }
});






const macInput = document.getElementById('inputvalue');
let apple = 0;
ClickBox.addEventListener('click', () => {
    apple++;
    if(apple % 2) {
        const name ="X"
        ClickBox.innerHTML = name;
        macInput.value = "02";
    }
    else {
    const name =""
    ClickBox.innerHTML = name;
    macInput.value = ""; 
    }
    
    
});
function getRandomInt(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

RandomButton.addEventListener('click', () => {
    macInput.value = '02';
    for(let i = 0; i < 5; i++) {
        let randomNumber = getRandomInt(10, 99);
        macInput.value =  macInput.value + '-' + randomNumber;

    }
    
});
macInput.addEventListener('input', () => {
    
    let value =  macInput.value.replace(/[^a-fA-F0-9]/g, ''); // only hex numbers
    
    let formatted = '';
    
    for (let i = 0; i < value.length; i++) {
        if (i > 0 && i % 2 === 0) {
            formatted += '-';
        }
        formatted += value[i];
    }

    macInput.value = formatted.toUpperCase(); // make it uppercase
});










let selectedEntry = null;



window.electronAPI.onCppOutput((output) => {
    hideLoadingScreen();
    
    console.log("Raw Output:", output);
    
    try {
        const parsedData = JSON.parse(output);
        console.log("Parsed Data Type:", typeof parsedData);
        console.log("Is Array?", Array.isArray(parsedData));
        console.log("Parsed Data:", parsedData);
        if (!Array.isArray(parsedData)) {
        throw new Error("Expected an array but got: " + typeof parsedData);
    }
        parsedData.forEach(entry => {
           
            const newColumn = document.createElement("div");
            newColumn.className = "column";
            const startbox = document.createElement("div");
            const databox = document.createElement("div");
            const endbox = document.createElement("div");

            startbox.className = "rows Start";
            databox.className = "rows Data";
            endbox.className = "rows End";
            
            newColumn.appendChild(startbox);
            newColumn.appendChild(databox);
            newColumn.appendChild(endbox);
            flexTable.appendChild(newColumn);
            
            databox.innerText = entry.InterFaceTypeConnection;
            
            const ai = document.createElement("i");
            startbox.appendChild(ai);
            ai.className = "fa-solid fa-circle";
            if(entry.Status == "Up") {
                ai.classList.add('colorGreen');
            }
            else if (entry.Status == "Disconnected") {
                ai.classList.add('colorRed');
            }
            else if (entry.Status == "Not Present") {
                ai.classList.add('colorYellow');
            }


            
            databox.addEventListener("click", ()=> {
                clickedOnMac();
                clearAllHighlights();
                startbox.classList.toggle("hover-for-start");
                databox.classList.toggle("hover-for-data");
                endbox.classList.toggle("hover-for-end");

            });
            startbox.addEventListener("click", ()=> {
                clickedOnMac();
                clearAllHighlights();
                startbox.classList.toggle("hover-for-start");
                databox.classList.toggle("hover-for-data");
                endbox.classList.toggle("hover-for-end");

            });
            endbox.addEventListener("click", ()=> {
                clickedOnMac();
                clearAllHighlights();
                startbox.classList.toggle("hover-for-start");
                databox.classList.toggle("hover-for-data");
                endbox.classList.toggle("hover-for-end");
                
            });
            
            [databox,startbox,endbox].forEach(box => {
                
                box.addEventListener("click", ()=>{
                    clearAllHighlights();
                    startbox.classList.toggle("hover-for-start");
                    databox.classList.toggle("hover-for-data");
                    endbox.classList.toggle("hover-for-end");
                    
                    
                    if(startbox.classList.contains("hover-for-start")) {
                    selectedEntry = entry;
                    titleForInfo.innerText = entry.DriverDesc;
                    orginalMacAddress.innerText = entry.OriginalNetworkAddress;
                    linkStatus.innerText = "Link Status: "+ selectedEntry.Status;
                    speedID.innerText = "Speed: " + selectedEntry.LinkSpeed;
                    if(!(selectedEntry.ActiveMac == "")){
                        ActiveMac.innerText = selectedEntry.ActiveMacAddress;
                    }

                    

                }

                });

                
            });

              
              

        });
        
        
    }
    catch (error) {
        console.error("Error parsing JSON:", error);
        
    }
});


    ChangeMacConfirmButton.addEventListener('click', ()=> {
                       
    
        if(macInput.value.length == 17) {
            
            const FinalNewMacAddress = macInput.value.replace(/-/g,"");
            const currentSubKey = selectedEntry.NameOfSubKey;
            const NetCfgInstanceId = selectedEntry.NetCfgInstanceId;
            const DeviceInstanceID = selectedEntry.DeviceInstanceID;
            const NameofAAdapter = selectedEntry.DriverDesc;
            alert(currentSubKey + " " + NetCfgInstanceId + " " + DeviceInstanceID + " " + FinalNewMacAddress);
                            
            window.electronAPI.runCppWithArgs({ str1: FinalNewMacAddress, str2: currentSubKey });
            window.electronAPI.TestResults({str1:FinalNewMacAddress, str2:NetCfgInstanceId, str3:DeviceInstanceID,str4:NameofAAdapter, str5:"yes" });
                     
                        
                    }
                        else {
                         alert("Invalid Mac Address"); 
                        }
                       
                    });








   



