cmake_minimum_required(VERSION 3.22)
project(B8miEax VERSION 0.1.0)

# JUCE is intentionally supplied by the builder; no hidden downloads or binaries.
# Configure with -DJUCE_DIR=C:/JUCE (or your JUCE source directory).
if(NOT DEFINED JUCE_DIR)
  message(FATAL_ERROR "Set JUCE_DIR to your local JUCE source directory, e.g. C:/JUCE")
endif()
add_subdirectory("${JUCE_DIR}" "${CMAKE_BINARY_DIR}/JUCE")

juce_add_plugin(B8miEax
    COMPANY_NAME "B8mi"
    BUNDLE_ID "com.b8mi.eax"
    PLUGIN_MANUFACTURER_CODE B8mi
    PLUGIN_CODE B8Ea
    PRODUCT_NAME "B8mi Eax"
    DESCRIPTION "Microphone EQ, echo and reverb effect"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE
    PLUGIN FORMATS VST3 Standalone
)
target_sources(B8miEax PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
)
target_compile_features(B8miEax PRIVATE cxx_std_17)
target_compile_definitions(B8miEax PUBLIC
    JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)
target_link_libraries(B8miEax PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    PUBLIC juce::juce_recommended_config_flags
           juce::juce_recommended_lto_flags
           juce::juce_recommended_warning_flags
)
