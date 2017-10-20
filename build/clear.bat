cls
IF EXIST .vs rmdir /s /q .vs
IF EXIST obj rmdir /s /q obj
IF EXIST Debug rmdir /s /q Debug
IF EXIST Release rmdir /s /q Release
IF EXIST ..\bin rmdir /s /q ..\bin
IF EXIST *vcxproj* del /F *vcxproj*
IF EXIST *sln del /F *sln
IF EXIST *db del /F *db