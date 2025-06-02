const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('electronAPI', {
    sendCppRun: () => ipcRenderer.send('run-cpp'), // for search.exe
    onCppOutput: (callback) => ipcRenderer.on('cpp-output', (event, output) => callback(output)),
    runCppWithArgs: (args) => ipcRenderer.send('ChangeMacAddressFile', args), // for changeMacAddress.exe
    TestResults: (args) => ipcRenderer.send('TestFinalResult', args),
    onTestResults: (callback) => ipcRenderer.on('test-results', (event, value) => callback(value)),
    MacChecker: (args) => ipcRenderer.send('TestMacAddressF', args),
    onMacChecker: (callback) => ipcRenderer.on('TestMacAddress-results', (event, values) => callback(values)),

    
});

window.addEventListener('DOMContentLoaded', () => {
    const replaceText = (selector, text) => {
      const element = document.getElementById(selector)
      if (element) element.innerText = text
    }
  
    for (const type of ['chrome', 'node', 'electron']) {
      replaceText(`${type}-version`, process.versions[type])
    }
  })