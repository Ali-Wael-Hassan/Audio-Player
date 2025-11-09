# Faculty of Computing and Artificial Intelligence, Cairo University

## CS213: Object-Oriented Programming

# Chicken Player: JUCE Audio Player Application

## Team Members

* Ali Wael Abdalla 20240354
* Kareem Ahmed Abdelmoneim 20240417
* Omar Osama 20250921

## Instructor

* Dr. Mohammad El-Ramly

## Project Overview

**Chicken Player** is a desktop audio application developed for the **CS213: Object-Oriented Programming** course at the **Faculty of Computing and Artificial Intelligence, Cairo University**.

The program allows users to load, play, and manage audio files and playlists. It is built using **C++** and the **JUCE Framework**, showcasing an event-driven architecture with a clear separation between the audio engine and the graphical user interface.

| Detail | Value |
| :--- | :--- |
| **Application Name** | **Chicken Player** |
| **Team Name** | **El 3esha Company** |
| **Instructor** | Dr. Mohammad El-Ramly |
| **Build System** | **Projucer + Visual Studio (MinGW)** |

---

## Project Documentation

* **View Project Design Document:** [**Click here for the Google Doc**](https://docs.google.com/document/d/1XCiHWhwyYIqgTqKv2bRetHcsCzcCk4Eg/edit?usp=sharing&ouid=107699201691207194397&rtpof=true&sd=true)

* **View Generated Code Documentation (Static):** [**Click here for a snapshot on Google Drive**](https://drive.google.com/file/d/1b3WJW8wr5WWNAZMCeeX6HqlwbraDC8uW/view?usp=sharing)
    *(This is a pre-generated version of the Doxygen documentation.)*

* **Generate Local Documentation:** You can also generate the latest documentation from the source code by following the **"Documentation Generation"** steps at the bottom of this file.

---

## Key Features & Object-Oriented Design

The application implements core OOP principles by encapsulating logic into distinct classes that communicate via listeners and callbacks.

### Core Functionality

* **Persistent Data:** Handles loading and saving of all application data, including the main song library (`List.txt`), playlist definitions (`PLayList.txt`), user playlist contents (`UserPlaylist.txt`), favorites (`FavoritSong.txt`, `FavoritePlaylist.txt`), and recently played tracks (`recent.txt`).
* **Settings Management:** A dedicated settings page allows users to change the application's **Theme** (dark/light) and **Language**.
* **Sleep Timer:** Includes a functional sleep timer that can be set by the user to stop playback after a set duration.
* **State Management:** The application's main components (`MainComponent`, `Home`, `PlayerGUI`) are decoupled, managing audio state and UI state separately.
* **Metadata Reading:** Uses the **TagLib** library to read audio file metadata (artist, title, duration) on the fly.

### Audio Player Features

| Category | Features Implemented |
| :--- | :--- |
| **Playback & Control** | Full playback controls (play, stop, loop), circular volume slider, playback speed control, and a draggable position slider. |
| **Library** | A `LibraryPage` that loads and displays `SongData` (title, artist, duration) and `PlaylistData` from files. |
| **Playlist Management** | A `PlaylistManager` class handles the creation, saving, and loading of custom user playlists. A pop-up component (`NewPlaylistPopup`) allows for new playlist creation. |
| **UI Components** | A multi-page interface including a `Home` screen, `LibraryPage`, `SettingsPage`, and one or more `PlayerGUI` instances. |

---

## Project Setup & Configuration

**This is a critical step.** For the application to run correctly, you must configure its assets (images) and data files (text files).

### 1. Image/Asset Setup (Projucer)

The application's images (like icons, backgrounds) are not loaded directly from disk. They must be bundled into the application binary using the Projucer.

1.  Place all your raw image files (e.g., `.png`, `.jpg`) into a source folder (e.g., `C:\MyImages`).
2.  Open the **Projucer** and go to the "File Explorer" tab on the left.
3.  Right-click on the "BinaryData" folder and select **"Add new files..."**.
4.  Navigate to `C:\MyImages` and select all the images you need.
5.  Make sure the "Use "JuceHeader.h"..." box is **ticked**.
6.  Click "Save and Open in IDE...". The Projucer will generate a new `BinaryData.h` and `BinaryData.cpp` file. Your images are now part of the program and can be accessed from memory.

### 2. Data File Setup (.txt files)

The application depends on several `.txt` files to load and save the song library and user playlists. These files must be in the same directory as the final executable.

1.  Locate your project's build directory (e.g., `Builds\VisualStudio2022\x64\Release\App\`).
2.  **Copy all of the following `.txt` files** into this `Release` folder:
    * `List.txt`
    * `PLayList.txt`
    * `UserPlaylist.txt`
    * `FavoritSong.txt`
    * `FavoritePlaylist.txt`
    * `recent.txt`
3.  Now, when you run the `Chicken Player.exe` from that folder, it will be able to find, read, and write to these data files.

## Building from the Projucer

1.  Ensure you have the JUCE library and the **Projucer** application.
2.  Open the `.jucer` file for this project in the Projucer.
3.  Set up your export target (e.g., Visual Studio 2022).
4.  Perform the **"Project Setup"** steps listed above.
5.  Click "Save and Open in IDE..." to generate the project files.
6.  Build and run the project from your IDE (e.g., Visual Studio) in **Release Mode**.

## Dependency Setup: Building TagLib (for Audio Metadata)

This project uses `taglib` to read metadata. It must be built from source as a static library.

### 1. Prerequisites
* [Visual Studio 2022](https://visualstudio.microsoft.com/) with the "Desktop development with C++" workload.
* [CMake](https://cmake.org/download/) (make sure to add it to your system PATH).
* Download **zlib Source Code** from [zlib.net](https://zlib.net/).
* Download **TagLib Source Code** from [taglib.org](https://taglib.org/releases/).

### 2. Directory Setup
Create the following folder structure in `C:\`. Unzip the source code into the `_source` folders.
├── taglib_install ├── taglib_source │ ├── (source files for taglib...) ├── taglib_build │ ├── zlib_source │ ├── (source files for zlib...) └── zlib_build

### 3. Build and Install Commands
Run all of these commands sequentially from your terminal (e.g., `cmd` or PowerShell). This single block will configure, build, and install both `zlib` and `taglib`.

```bat
:: --- 1. BUILD ZLIB (DEPENDENCY) ---

:: Navigate to the zlib build folder
cd C:\Dev\Libs\zlib_build

:: Configure the VS project
cmake -G "Visual Studio 17 2022" -A x64 ..\zlib_source

:: Build Release
cmake --build . --config Release

:: Build Debug
cmake --build . --config Debug


:: --- 2. BUILD AND INSTALL TAGLIB ---

:: Navigate to the taglib build folder
cd C:\Dev\Libs\taglib_build

:: Configure the VS project using the built zlib
:: Note: This command points to the Release zlib.lib, which is common.
cmake -G "Visual Studio 17 2022" -A x64 ..\taglib_source -DCMAKE_INSTALL_PREFIX="C:\Dev\Libs\taglib_install" -DZLIB_INCLUDE_DIR="C:\Dev\Libs\zlib_source" -DZLIB_LIBRARY="C:\Dev\Libs\zlib_build\Release\zlib.lib" -DBUILD_SHARED_LIBS=OFF

:: Build and Install Release
cmake --build . --config Release
cmake --install . --config Release

:: Build and Install Debug
cmake --build . --config Debug
cmake --install . --config Debug
