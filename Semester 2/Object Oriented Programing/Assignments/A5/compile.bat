@echo off
g++ Main/main.cpp Domain/trench.cpp Repository/trenchCoatRepository.cpp Controller/trenchCoatController.cpp UI/AdminUI.cpp UI/UI.cpp UI/UserUI.cpp UI/AppUI.cpp -o main.exe -I . -std=c++17 -Wall -Wextra
echo Compilation finished. Check main.exe. 