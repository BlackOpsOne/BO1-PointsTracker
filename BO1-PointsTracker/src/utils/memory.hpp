#include <windows.h>
#include <iostream>

namespace utils {
    namespace memory {
        int ReadInt(HANDLE processHandle, int address);
    } // memory
} // utils