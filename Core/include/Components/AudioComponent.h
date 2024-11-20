// #pragma once
//
// #include "Entity.h"
// #include "Component.h"
// #include "FMOD/inc/fmod.hpp"
// #include <cstring>  // For handling char* operations
//
// class AudioComponent : public Component
// {
// public:
//     AudioComponent(Entity* entity, const char* audioFilePath);  // Constructor with Entity* and char* for the file path
//
//     virtual ~AudioComponent();  // Destructor
//
//     void LoadSound(const char* filePath);  // Load a sound file
//     void PlaySound();  // Play the loaded sound
//     void StopSound();  // Stop the sound
//     void Update();  // Update the audio component
//     Component* clone() const override;  // Clone the component
//
//     bool IsPlaying() const;  // Check if the sound is currently playing
//
//     private:
//     char* audioFilePath;  // File path to the audio file as char*
//     FMOD::Sound* sound;   // FMOD sound object
//     FMOD::Channel* channel; // FMOD channel to control playback
//     bool isPlaying;        // Whether the sound is currently playing
// };
