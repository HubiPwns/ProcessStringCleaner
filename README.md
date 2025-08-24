# ProcessStringCleaner

**ProcessStringCleaner** is a Windows-based C++ tool that scans the memory of a running process, finds all occurrences of a user-specified string (both ASCII and UTF-16), and overwrites them with zero bytes.  
The process name is entered by the user (e.g., `javaw.exe`), making it flexible for different targets.  

This project is designed **for educational purposes only** — to demonstrate how process enumeration, memory inspection, and direct memory modification work using the Windows API.

---

## Features

- Search for a process by name and display its PID.
- Confirm successful attachment to the target process.
- Scan only committed, readable, and writable memory regions.
- Detect and remove both ASCII (1-byte) and UTF-16 (2-byte) representations of the provided string.
- Display real-time feedback with memory addresses where data was cleared.
- Report execution time for the entire operation.

---

## Example

<img width="553" height="548" alt="image" src="https://github.com/user-attachments/assets/290aca99-0376-41e2-a629-cfa70f21a579" />


---

## Technical details

- **Language:** C++17  
- **API used:** [Windows API](https://learn.microsoft.com/en-us/windows/win32/api/) via `windows.h`
- **Key functions:**
  - `CreateToolhelp32Snapshot`, `Process32FirstW`, `Process32NextW` – process enumeration.
  - `OpenProcess` – obtaining a handle to the target process.
  - `VirtualQueryEx` – querying memory regions.
  - `ReadProcessMemory` / `WriteProcessMemory` – reading and modifying process memory.
- **Encodings handled:**
  - ASCII (1 byte per character)
  - UTF-16 (wide characters, 2 bytes per character)
- **Memory protection checks:** scans only non-guarded, committed memory with read/write permissions.

---

## What to keep in mind

- You need **administrator privileges** to access memory of most processes.
- Writing into a process's memory may cause instability or crashes — test only on processes you control.
- This tool modifies **only in-memory data**, not data stored on disk.
- Different processes may have different memory layouts; results may vary depending on target.
- Use responsibly — this is meant for debugging, research, and controlled testing.

---

## Requirements

- **Windows OS**
- **C++ compiler** supporting Windows API (e.g., MSVC or MinGW)
- Administrator rights for accessing protected process memory

---

## Disclaimer

This tool is intended **solely for educational purposes**.  
Do not use it against software you do not own or without explicit permission.  
The author assumes **no responsibility** for any misuse or damage resulting from this program.

---

## License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

---

**Author:** HubiPwns — 2025
