#include "memory.hpp"

namespace utils {
	namespace memory {
		int ReadInt(HANDLE processHandle, int address) {
			if (address == -1 || processHandle == nullptr) {
				return -1;
			}

			int buffer = 0;
			SIZE_T bytesRead = 0;
			bool success = ReadProcessMemory(processHandle, (LPCVOID)address, &buffer, sizeof(buffer), &bytesRead);

			if (!success || bytesRead != sizeof(buffer)) {
				return -1;
			}

			return buffer;
		}
	} // memory
} // utils