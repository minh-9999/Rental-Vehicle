@echo off
setlocal EnableDelayedExpansion

REM === 💡 Set working directory to repo root ===
cd /d %~dp0\..
echo 📁 Preparing logs directory...
@REM mkdir build\logs 2>nul

REM === 📦 Ensure coverage flags are enabled for compilation ===
set CXXFLAGS=--coverage
set LDFLAGS=--coverage

REM === 📦 Move to build directory ===
cd build
mkdir logs 2>nul

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

REM === 📝 Collect code coverage data ===
echo 📊 Collecting coverage data...
:: Using gcovr to generate coverage report in XML format
gcovr --root . --xml --output logs\coverage.xml

REM === 📦 Check if coverage data was generated ===
if exist logs\coverage.xml (
    echo ✅ Coverage XML file created successfully.
) else (
    echo ❌ Failed to create coverage XML file.
    exit /b 1
)

REM === 📊 Verifying content of coverage.xml ===
for /f "delims=" %%i in (logs\coverage.xml) do set "content=%%i"
if defined content (
    echo ✅ coverage.xml has content.
) else (
    echo ❌ coverage.xml is empty.
    exit /b 1
)

echo ✅ All tests executed, logs saved, and coverage collected.

endlocal
