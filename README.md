# VST3-Plugin-Steinberg-SDK

This is my very first attempt at creating a VST3 audio plugin using the [Steinberg VST3 SDK](https://github.com/steinbergmedia/vst3sdk).  
The plugin is built for use with digital audio workstations (DAWs) that support the VST3 format.

> 🚫 Note: This repository does **not** include the Steinberg VST3 SDK due to licensing restrictions.

---

## 📦 Requirements

- [Steinberg VST3 SDK](https://github.com/steinbergmedia/vst3sdk)
- VST3-compatible DAW (e.g., Reaper, Cubase, FL Studio, etc.)
- CMake 3.15+ (for building from source, if needed)
- C++17-compatible compiler (if building)

---

## 🛠 Installation

1. **Clone this repository**:

   ```bash
   git clone https://github.com/yourusername/VST3-Plugin-Steinberg-SDK.git
   ```

2. **Download the VST3 SDK from Steinberg:**

   [https://github.com/steinbergmedia/vst3sdk](https://github.com/steinbergmedia/vst3sdk)

   Place the downloaded SDK in the root of the repository (e.g., `VST3_SDK/`).

3. **Build with CMake:**

   ```bash
   mkdir build
   cd build
   cmake .. -DVST3_SDK_PATH=../VST3_SDK
   cmake --build .
   ```

4. **Copy the resulting `.vst3` file to your system's VST3 folder:**

   - **Windows:** `C:\Program Files\Common Files\VST3`
   - **macOS:** `~/Library/Audio/Plug-Ins/VST3`

5. **Open your DAW and rescan the plugin list in preferences.**

---

## 🔒 Licensing
This project is licensed under the MIT License.  
The Steinberg VST3 SDK is subject to its own license agreement and must be obtained separately.

