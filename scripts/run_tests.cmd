@echo off
setlocal EnableDelayedExpansion

REM === 💡 Set working directory to repo root ===
cd /d %~dp0\..
echo 📁 Preparing logs directory...
mkdir build\logs 2>nul

REM === 📦 Move to build directory ===
cd build

REM === 🧪 Running Unit Tests ===
echo Running Unit Tests...
ctest -R Unit --output-on-failure > logs\unit_test.log 2>&1
if errorlevel 1 (
    echo ⚠️ Unit tests failed
) else (
    echo ✅ Unit tests passed
)

REM === 🔌 Running Integration Tests ===
echo Running Integration Tests...
ctest -R Integration --output-on-failure > logs\integration_test.log 2>&1
if errorlevel 1 (
    echo ⚠️ Integration tests failed
) else (
    echo ✅ Integration tests passed
)

REM === 📦 Combine test logs ===
echo Combining test logs...
type logs\unit_test.log > logs\test.log
type logs\integration_test.log >> logs\test.log

echo ✅ All tests executed and logs saved.
