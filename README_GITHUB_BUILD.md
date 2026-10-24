# Build B8mi Eax using GitHub Actions

## How to run
1. Create a GitHub repository and upload the contents of this ZIP (including the hidden `.github` folder).
2. Open the repository's **Actions** tab and enable workflows if asked.
3. Choose **Build B8mi Eax (Windows x64)** and click **Run workflow**.
4. Open the completed run and download **B8mi-Eax-Windows-x64** from Artifacts.

## Important compatibility note
This workflow builds the project's modern **VST3 + standalone** targets using JUCE 8.0.10. It does not build VST2 `.dll`.
Equalizer APO versions/setups may require VST2 for their VST filter. Verify that your installed Equalizer APO supports VST3 before relying on this artifact. If your version only supports VST2, a legally obtained VST2 SDK compatible with JUCE is needed; it is not bundled here.

## Security
The workflow checks out JUCE from its official repository at a pinned tag and compiles source. It does not run any generated plugin during CI. Review the source and workflow before running it. A successful compile does not prove DSP equivalence or malware-free behavior.
