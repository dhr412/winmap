# WinMap

**WinMap** is a cross-process memory modification utility for Windows. It allows you to read and write memory in another process by specifying a process ID, memory address, data type, and value.

## Installation

### From Releases

1. Download the latest binary from the [Releases](https://github.com/dhr412/winmap/releases) page
2. Extract `winmap.exe` to your desired location
3. Optionally add the directory to your system PATH for easy access from any command line tool

### Compiling from Source

**Requirements:**

- Visual Studio 2015 or later, or MinGW with GCC
- Windows SDK (included with Visual Studio)

**Steps:**

1. Clone the repository:

   ```bash
   git clone https://github.com/yourusername/winmap.git
   cd winmap
   ```

2. Compile the source:

   **Using MSVC (Visual Studio):**

   ```bash
   cl /W3 src/winmap.c /link kernel32.lib
   ```

   **Using MinGW:**

   ```bash
   gcc -o winmap.exe src/winmap.c -lkernel32
   ```

3. The compiled binary will be in the current directory

## Usage

```
winmap.exe <process_id> <hex_address> <type> <value>
```

### Parameters

- **process_id**: The decimal process ID (PID) of the target process
- **hex_address**: The memory address in hexadecimal format (e.g., `0x0d37ff6e0`)
- **type**: Data type to write - `int`, `float`, `bool`, or `char*`
- **value**: The value to write (format depends on type)

### Finding a Process ID

You can find the PID of a running process using:

```bash
tasklist /FI "IMAGENAME eq program.exe"
```

Or use Task Manager (Ctrl+Shift+Esc) and enable the PID column.

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
winmap.exe 12345 0x0d37ff6e0 char* "Hello"
```

### Important Notes

- **Administrator Privileges**: You _may_ need to run `winmap` as administrator to access memory of other processes
- **Process Must Be Running**: The target process must be active during the operation
- **Confirmation Prompt**: If the memory address contains a non-zero value, winmap will prompt you to confirm before overwriting
- **Error Handling**: The tool provides detailed error messages if operations fail, including Windows error codes

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

