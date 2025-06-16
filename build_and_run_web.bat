@echo off
echo ========================
echo 🛠 Initializing EMSDK...
echo ========================

REM Step 1: Set up Emscripten environment
call "E:\StudyMaterial\C\Cproject\handCricket\emsdk\emsdk_env.bat"
if errorlevel 1 (
    echo ❌ Failed to load emsdk_env.bat
    pause
    exit /b 1
)

echo ==========================
echo 🚧 Starting Compilation...
echo ==========================

REM Step 2: Compile to WebAssembly
emcc main.c game.c toss_screen.c choose_bat_ball.c play_screen.c menu_screen.c ui_utils.c fade.c summary_screen.c \
resource/Montserrat_ExtraBold.c resource/Montserrat_light.c resource/Montserrat_Medium.c \
-o handcricket.html \
-I include \
-s USE_GLFW=3 \
-s ASYNCIFY \
-s FORCE_FILESYSTEM=1 \
-s ALLOW_MEMORY_GROWTH=1 \
--preload-file asset \
"E:/StudyMaterial/C/Cproject/handCricket/lib/build_web/libraylib.a"




echo ==========================
echo ✅ Compilation Successful!
echo ==========================

REM Step 3: Serve locally
echo 🌐 Launching local server: http://localhost:8080/
start http://localhost:8080/handcricket.html

emrun --no_browser --port 8080 handcricket.html
if errorlevel 1 (
    echo ❌ emrun failed! Is it installed and in PATH?
)

echo ==========================
echo ✅ All Done!
echo ==========================
pause
