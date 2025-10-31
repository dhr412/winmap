#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void segfault_handler(int sig) {
  fprintf(stderr, "Error: Invalid memory address (segfault)\n");
  exit(1);
}

int main(int argc, char *argv[]) {
  if (argc != 5) {
    fprintf(stdout, "Usage: %s <process_id> <hex_address> <type> <value>\n",
            argv[0]);
    fprintf(stdout, "Types: int, float, bool, str\n");
    return 1;
  }

  signal(SIGSEGV, segfault_handler);

  char *endptr;
  errno = 0;
  unsigned long pid = strtoul(argv[1], &endptr, 10);
  if (endptr == argv[1] || *endptr != '\0' || errno == ERANGE) {
    fprintf(stderr, "Error: Invalid process ID '%s'\n", argv[1]);
    return 1;
  }

  HANDLE hProcess =
      OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION,
                  FALSE, (DWORD)pid);
  if (hProcess == NULL) {
    fprintf(stderr, "Error: Could not open process %lu (error: %lu)\n", pid,
            GetLastError());
    return 1;
  }

  errno = 0;
  unsigned long long ull = strtoull(argv[2], &endptr, 16);
  if (endptr == argv[2] || *endptr != '\0' ||
      (ull == ULLONG_MAX && errno == ERANGE)) {
    fprintf(stderr, "Error: Invalid hex address '%s'\n", argv[2]);
    CloseHandle(hProcess);
    return 1;
  }

  void *addr = (void *)ull;
  if (addr == NULL) {
    fprintf(stderr, "Error: Address is NULL\n");
    CloseHandle(hProcess);
    return 1;
  }

  char *type = argv[3];
  char *val_str = argv[4];
  SIZE_T bytesRead, bytesWritten;

  if (strcmp(type, "int") == 0) {
    char *end;
    errno = 0;
    long val = strtol(val_str, &end, 10);
    if (end == val_str || *end != '\0' || errno == ERANGE || val < INT_MIN ||
        val > INT_MAX) {
      fprintf(stderr, "Error: Invalid int value '%s'\n", val_str);
      CloseHandle(hProcess);
      return 1;
    }

    int current = 0;
    if (!ReadProcessMemory(hProcess, addr, &current, sizeof(int), &bytesRead)) {
      fprintf(stderr, "Error: Could not read memory at %p (error: %lu)\n", addr,
              GetLastError());
      CloseHandle(hProcess);
      return 1;
    }

    if (current != 0) {
      printf("Current value at %p: %d\n", addr, current);
      printf("Press any key to confirm overwrite...");
      getchar();
    }

    int writeVal = (int)val;
    if (!WriteProcessMemory(hProcess, addr, &writeVal, sizeof(int),
                            &bytesWritten)) {
      fprintf(stderr, "Error: Could not write memory at %p (error: %lu)\n",
              addr, GetLastError());
      CloseHandle(hProcess);
      return 1;
    }

    printf("Successfully wrote %d to address %p\n", writeVal, addr);

  } else if (strcmp(type, "float") == 0) {
    char *end;
    errno = 0;
    double dval = strtod(val_str, &end);
    if (end == val_str || *end != '\0' || errno == ERANGE) {
      fprintf(stderr, "Error: Invalid float value '%s'\n", val_str);
      CloseHandle(hProcess);
      return 1;
    }

    float current = 0.0f;
    if (!ReadProcessMemory(hProcess, addr, &current, sizeof(float),
                           &bytesRead)) {
      fprintf(stderr, "Error: Could not read memory at %p (error: %lu)\n", addr,
              GetLastError());
      CloseHandle(hProcess);
      return 1;
    }

    if (current != 0.0f) {
      printf("Current value at %p: %f\n", addr, current);
      printf("Press any key to confirm overwrite...");
      getchar();
    }

    float writeVal = (float)dval;
    if (!WriteProcessMemory(hProcess, addr, &writeVal, sizeof(float),
                            &bytesWritten)) {
      fprintf(stderr, "Error: Could not write memory at %p (error: %lu)\n",
              addr, GetLastError());
      CloseHandle(hProcess);
      return 1;
    }

    printf("Successfully wrote %f to address %p\n", writeVal, addr);

  } else if (strcmp(type, "bool") == 0) {
    char *end;
    errno = 0;
    long val = strtol(val_str, &end, 10);
    if (end == val_str || *end != '\0' || errno == ERANGE) {
      fprintf(stderr, "Error: Invalid bool value '%s'\n", val_str);
      CloseHandle(hProcess);
      return 1;
    }

    bool current = false;
    if (!ReadProcessMemory(hProcess, addr, &current, sizeof(bool),
                           &bytesRead)) {
      fprintf(stderr, "Error: Could not read memory at %p (error: %lu)\n", addr,
              GetLastError());
      CloseHandle(hProcess);
      return 1;
    }

    if (current) {
      printf("Current value at %p: %s\n", addr, current ? "true" : "false");
      printf("Press any key to confirm overwrite...");
      getchar();
    }

    bool writeVal = val != 0;
    if (!WriteProcessMemory(hProcess, addr, &writeVal, sizeof(bool),
                            &bytesWritten)) {
      fprintf(stderr, "Error: Could not write memory at %p (error: %lu)\n",
              addr, GetLastError());
      CloseHandle(hProcess);
      return 1;
    }

    printf("Successfully wrote %s to address %p\n", writeVal ? "true" : "false",
           addr);

  } else if (strcmp(type, "str") == 0) {
    if (val_str == NULL || strlen(val_str) == 0) {
      fprintf(stderr, "Error: Invalid string value (null or empty)\n");
      CloseHandle(hProcess);
      return 1;
    }

    char *current = NULL;
    if (!ReadProcessMemory(hProcess, addr, &current, sizeof(char *),
                           &bytesRead)) {
      fprintf(stderr, "Error: Could not read memory at %p (error: %lu)\n", addr,
              GetLastError());
      CloseHandle(hProcess);
      return 1;
    }

    if (current != NULL) {
      printf("Current pointer at %p: %p\n", addr, current);
      printf("Press any key to confirm overwrite...");
      getchar();
    }

    char *writeVal = val_str;
    if (!WriteProcessMemory(hProcess, addr, &writeVal, sizeof(char *),
                            &bytesWritten)) {
      fprintf(stderr, "Error: Could not write memory at %p (error: %lu)\n",
              addr, GetLastError());
      CloseHandle(hProcess);
      return 1;
    }

    printf("Successfully wrote pointer %p to address %p\n", writeVal, addr);

  } else {
    fprintf(stderr, "Invalid type: %s\n", type);
    CloseHandle(hProcess);
    return 1;
  }

  CloseHandle(hProcess);
  return 0;
}
