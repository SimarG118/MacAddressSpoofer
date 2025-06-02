if (process.env.NODE_ENV === 'development') {
  try {
    require('electron-reload')(__dirname, {
      electron: require(`${__dirname}/node_modules/electron`)
    });
  } catch (err) {
    console.log('electron-reload not found, skipping reload in production');
  }
}


const { app, BrowserWindow, ipcMain } = require('electron/main');
const path = require('node:path')
const { exec, execFile } = require('child_process'); // Add this line


function createWindow () {
  const win = new BrowserWindow({
    
    autoHideMenuBar: true,
    frame:true,
    width: 800,
    height: 600,
    resizable:false,

    webPreferences: {
        preload: path.join(__dirname, 'preload.js'), // ✅ Load the Preload Script
        contextIsolation: true, // ✅ Security Best Practice
        enableRemoteModule: false,
        nodeIntegration: false
    }
  })

  win.loadFile('index.html');
}


app.whenReady().then(() => {
  createWindow()

  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) {
      createWindow()
    }
  })
})

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit()
  }
})


ipcMain.on('run-cpp', (event) => {
    const executablePath = path.join(__dirname, 'search.exe');
    console.log(`Running: ${executablePath}`);

    execFile(executablePath, (error, stdout, stderr) => {
        if (error) {
            console.error(`Exec Error: ${error.message}`);
            event.reply('cpp-output', JSON.stringify({ error: error.message }));
            return;
        }
        if (stderr) {
            console.error(`Exec Stderr: ${stderr}`);
            event.reply('cpp-output', JSON.stringify({ error: stderr }));
            return;
        }

        console.log(`Exec Output: ${stdout}`);
        event.reply('cpp-output', stdout || 'No output received.');
        
    });
});



ipcMain.on('ChangeMacAddressFile', (event, args) => {
  const executablePath = path.join(__dirname, 'changeMacAddress.exe');

  // Pass arguments to the C++ program
  const child = execFile(executablePath, [args.str1, args.str2], (error, stdout, stderr) => {
      if (error) {
          console.error(`ExecFile Error: ${error.message}`);
          event.reply('cpp-output', `Error: ${error.message}`);
          return;
      }
      if (stderr) {
          console.error(`ExecFile Stderr: ${stderr}`);
          event.reply('cpp-output', `Stderr: ${stderr}`);
          return;
      }

      console.log(`ExecFile Output: ${stdout}`);
      event.reply('cpp-output', stdout || 'No output received.');
  });
});



ipcMain.on('TestFinalResult', (event, args) => {
  const executablePath = path.join(__dirname, 'test.exe');

  // Pass arguments to the C++ program
  const child = execFile(executablePath, [args.str1, args.str2, args.str3, args.str4, args.str5], (error, stdout, stderr) => {
      if (error) {
          console.error(`ExecFile Error: ${error.message}`);
          event.reply('test-results', `Error: ${error.message}`);
          return;
      }
      if (stderr) {
          console.error(`ExecFile Stderr: ${stderr}`);
          event.reply('test-results', `Stderr: ${stderr}`);
          return;
      }

      console.log(`ExecFile Output: ${stdout}`);
      event.reply('test-results', stdout || 'No output received.');
  });
});

ipcMain.on('TestMacAddressF', (event, args) => {
  const executablePath = path.join(__dirname, 'TestMacAddress.exe');

  // Pass arguments to the C++ program
  const child = execFile(executablePath, [args.str1], (error, stdout, stderr) => {
      if (error) {
          console.error(`ExecFile Error: ${error.message}`);
          event.reply('TestMacAddress-results', `Error: ${error.message}`);
          return;
      }
      if (stderr) {
          console.error(`ExecFile Stderr: ${stderr}`);
          event.reply('TestMacAddress-results', `Stderr: ${stderr}`);
          return;
      }

      console.log(`ExecFile Output: ${stdout}`);
      event.reply('TestMacAddress-results', stdout || 'No output received.');
  });
});