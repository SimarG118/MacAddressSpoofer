export {}; // fix to window.electronAPI. error

declare global {
    interface Window {
        electronAPI: {
            sendCppRun: () => void;
            onCppOutput: (callback: (output: string) => void) => void;
            runCppWithArgs: (args: { str1: string; str2: string }) => void;
            TestResults: (args: { str1: string; str2: string, str3:string, str4:string, str5:string }) => void;
            onTestResults: (callback: (Value: string) => void) => void;
            MacChecker: (args: { str1: string; }) => void;
            onMacChecker: (callback: (Values: string) => void) => void;
        };
    }
}
