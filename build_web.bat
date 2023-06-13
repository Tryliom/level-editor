call C:\tools\emsdk\emsdk_env.bat

mkdir bin

call C:\tools\emsdk\upstream\emscripten\em++ src\Displays\Editor.cpp src\Displays\MainMenu.cpp src\Displays\SnakeGame.cpp src\Grid.cpp src\main.cpp libs\Basic\src\AudioManager.cpp libs\Basic\src\Image.cpp libs\Basic\src\Input.cpp libs\Basic\src\Timer.cpp libs\Basic\src\Utility.cpp libs\Basic\src\Window.cpp libs\Basic\src\Display.cpp ^
    -o bin/game.js ^
    -std=c++20 ^
    -Wall -Wextra ^
    -I libs/include ^
    -I libs/Basic/include ^
    -I include ^
    libs/web/libminifb.a ^
    -fcolor-diagnostics ^
    -fansi-escape-codes ^
    -sEXPORT_NAME=my_game ^
    -sASYNCIFY ^
    --preload-file assets ^
    -sENVIRONMENT=web ^
    -sMODULARIZE=1 ^
    -sALLOW_MEMORY_GROWTH=1 ^
    -sTOTAL_STACK=10485760 ^
    -sINITIAL_MEMORY=20971520 ^
    -sALLOW_TABLE_GROWTH ^
    -sMALLOC=emmalloc ^
    -sEXPORT_ALL=1 ^
    -sEXPORTED_FUNCTIONS=[\"_malloc\",\"_free\",\"_main\"] ^
    -sEXPORTED_RUNTIME_METHODS=ccall,cwrap ^
    -sASYNCIFY ^
    --no-entry

cd bin

python3 -m http.server 8000