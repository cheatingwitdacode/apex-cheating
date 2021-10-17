@ECHO OFF
if exist "EasyAntiCheat_launcher.exe.bak" (
    move EasyAntiCheat_launcher.exe r5apex.exe
    move EasyAntiCheat_launcher.exe.bak EasyAntiCheat_launcher.exe
) else (
    move EasyAntiCheat_launcher.exe EasyAntiCheat_launcher.exe.bak
    move r5apex.exe EasyAntiCheat_launcher.exe
)
pause