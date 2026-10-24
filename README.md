# B8mi Eax — Equalizer APO / VST2 build

This variant targets **64-bit VST2 (`.dll`)** for an Equalizer APO setup that exposes a VST plugin filter. A source-only package is not a ready-to-load plugin.

## Build requirements
- Windows x64
- Visual Studio 2022 with Desktop development with C++
- CMake 3.22+
- JUCE source tree
- A legally obtained VST2 SDK compatible with your JUCE version. JUCE does not bundle the VST2 SDK; without it, the VST2 target may fail to configure/build.

## Build
Run `BUILD_WINDOWS.ps1` from PowerShell in this folder. It asks for the local JUCE path and runs CMake/build. It does not install Visual Studio's GUI for you, and it cannot guarantee VST2 target support: that depends on the JUCE version and a legally obtained VST2 SDK.
The exact generated target/artifact folder can vary by JUCE/CMake version. Find the resulting 64-bit `B8mi Eax.dll`.

## Load in Equalizer APO
1. Use a 64-bit Equalizer APO installation and select the microphone/capture device in Device Selector.
2. Copy the plugin DLL to a stable path accessible to the audio service, preferably under `C:\Program Files\EqualizerAPO\VSTPlugins\`.
3. In Configuration Editor, add a VST plugin filter and select `B8mi Eax.dll`. If your Equalizer APO build does not expose a VST plugin filter, that build does not support this plugin format.
4. Apply changes and test at low volume first. Check for feedback, clipping, latency, and device compatibility.

## Scope and limitations
- This is a format-target adjustment of the supplied prototype, not a completed or tested binary. No `.dll` is included.
- The current DSP remains the prototype EQ/echo/reverb implementation; it does not magically become the full original OctopusEAX engine by changing plugin format.
- The plugin was not compiled or validated inside Equalizer APO in this environment. Do not assume it will run without issues until built and tested.
- This source archive contains no bundled executable payload. JUCE is expected to be supplied locally; no build-time network download is configured by this project.


## Source fix
Removed an invalid unused `DelayLine` type declaration that prevented compilation. No parameter defaults or audio controls were intentionally changed.
