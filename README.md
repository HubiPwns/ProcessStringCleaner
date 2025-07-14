# ProcessStringCleaner

**ProcessStringCleaner** is a simple Windows-based C++ tool that scans the memory of a running process (`javaw.exe` by default) and wipes all instances of a user-provided string (in both ASCII and UTF-16 formats).

🧠 This project was built for **educational purposes** — to explore how memory manipulation and scanning works on Windows systems using the WinAPI.

## 🔧 Features

- Scans the entire accessible memory of a running process.
- Detects and wipes both ASCII and UTF-16 representations of a given string.
- Uses low-level WinAPI functions like `ReadProcessMemory`, `WriteProcessMemory`, `VirtualQueryEx`.
- Provides real-time feedback on where the string was found and cleared.

## 📸 Example

[?] Enter the string to clear (ASCII UTF-16): secret_token
[+] javaw.exe PID: 3848
[ASCII] Cleared at: 0x3f24000
[UTF-16] Cleared at: 0x3f24810
[*] Done in 112 ms.
[+] Cleared!

## 🛠️ How It Works

1. Finds the target process by name (`javaw.exe`).
2. Iterates over all readable and writable memory regions.
3. Searches for the user-provided string in:
   - ASCII (1-byte per char)
   - UTF-16 (2 bytes per wchar)
4. Overwrites found matches with zero bytes (0x00).

## 🖥️ Requirements

- Windows OS
- C++ compiler supporting Windows API (`g++` via MinGW or MSVC)
- Administrator privileges to access other process memory

## 🚨 Disclaimer

This project is **strictly for learning** how memory scanning and manipulation works. Do **not** use it against software you don't own or without permission. The author is **not responsible** for any misuse.

## 📄 License

This project is licensed under the MIT License — see the [LICENSE](LICENSE) file for details.

---

> Created by **Hubi9warezz** — 2025  
