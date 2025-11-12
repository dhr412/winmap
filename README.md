# WinMap

**WinMap** is a cross-process memory modification utility for Windows. It allows reading and writing memory in another process by specifying a process ID, memory address, data type, and value.

> ⚠️ **Warning**: This tool can modify the memory of running processes. Incorrect usage can corrupt data, crash applications, or cause system instability. Only use this on processes you own or have explicit permission to modify. Use with caution and always test on non-critical processes first.

## Installation

### From Releases

1. Download the latest binary for your architecture from the [Releases](https://github.com/dhr412/winmap/releases) page
2. Extract `winmap.exe` to your desired location
3. Optionally add the directory to your system PATH for easy access from any command line tool

### Compiling from Source

**Requirements:**

- Visual Studio 2015 or later, or MinGW with GCC
- Windows SDK (included with Visual Studio)

**Steps:**

1. Clone the repository:

   ```bash
   git clone https://github.com/dhr412/winmap.git
   cd winmap
   ```

2. Compile the source:

   **Using MSVC (Visual Studio):**

   ```bash
   cl /W3 src/winmap.c /link kernel32.lib
   ```

   **Using MinGW:**

   ```bash
   gcc -o winmap.exe src/winmap.c
   ```

3. The compiled binary will be in the current directory

## Usage

```
winmap.exe [--admin] <process_id> <hex_address> <type> <value>
```

### Parameters

- **process_id**: The decimal process ID (PID) of the target process
- **hex_address**: The memory address in hexadecimal format (e.g., `0x0d37ff6e0`)
- **type**: Data type to write - `int`, `float`, `bool`, or `str`
- **value**: The value to write (format depends on type)

### Options

- **--admin**: Enable administrator mode with elevated privileges.

> Requires running as administrator.

### Finding a Process ID

You can find the PID of a running process using:

```bash
tasklist /FI "IMAGENAME eq program.exe"
```

Or use Task Manager (Ctrl+Shift+Esc) and enable the PID column.

### Getting Help

You can display the help message by using any of the following flags:

```bash
winmap.exe -h
winmap.exe --help
winmap.exe -help
```

This will show detailed usage information, examples, and all available options.

### Examples

Write an integer value to a process:

```bash
winmap.exe 12345 0x0d37ff6e0 int 42
```

Write a float value:

```bash
winmap.exe 12345 0x0d37ff6e0 float 3.14
```

Write a boolean value:

```bash
winmap.exe 12345 0x0d37ff6e0 bool 1
```

Write a pointer value:

```bash
winmap.exe 12345 0x0d37ff6e0 str "Hello"
```

Write with admin mode enabled (run as administrator):

```bash
winmap.exe --admin 12345 0x0d37ff6e0 int 42
```

### Important Notes

- **Administrator Privileges**: You may need to run `winmap` as administrator to access memory of other processes. Use the `--admin` flag to enforce this check and enable additional privileges.
- **Memory Protection**: The tool automatically attempts to unprotect memory pages before writing allowing modification of read-only memory regions.
- **Process Must Be Running**: The target process must be active during the operation
- **Confirmation Prompt**: If the memory address contains a non-zero value, winmap will prompt you to confirm before overwriting
- **Error Handling**: The tool provides detailed error messages if operations fail, including Windows error codes
- **Data Type Matters**: Memory doesn't have inherent types. Always use the same data type when reading and writing to the same address, or you may misinterpret the data
- **Protected Processes**: Some processes (antivirus, security software, kernel-protected) cannot be modified even with admin privileges

## Disclaimer

This tool is provided for educational and debugging purposes only. Users are responsible for any damage caused by its use. Modifying memory in system processes or third-party software may violate terms of service or cause legal issues. Use responsibly.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
