$ErrorActionPreference = "Stop"
Write-Host "B8mi Eax - Equalizer APO VST2 build helper"
Write-Host "This script does not download dependencies or run unknown binaries."
$juce = Read-Host "Enter path to JUCE source folder (e.g. C:\JUCE)"
if (!(Test-Path (Join-Path $juce "CMakeLists.txt"))) { throw "JUCE path is invalid." }
$cmake = Get-Command cmake -ErrorAction SilentlyContinue
if (!$cmake) { throw "CMake is not installed or not on PATH." }
Write-Host "Configuring x64 project..."
cmake -S . -B build -DJUCE_DIR="$juce" -A x64
if ($LASTEXITCODE -ne 0) { throw "CMake configuration failed. VST2 support/SDK may be missing in this JUCE setup." }
Write-Host "Building Release..."
cmake --build build --config Release
if ($LASTEXITCODE -ne 0) { throw "Build failed. Read the compiler output; this source package does not include JUCE or the proprietary VST2 SDK." }
Write-Host "Build command completed. Locate the generated DLL under build and validate it in a VST host before using it in Equalizer APO."
