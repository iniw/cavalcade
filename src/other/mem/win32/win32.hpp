#ifndef WIN32_HPP
#define WIN32_HPP

namespace mem::win32 {
	enum EXCEPTION_DISPOSITION : u32
	{
		ExceptionContinueExecution = 0,
		ExceptionContinueSearch,
		ExceptionNestedException,
		ExceptionCollidedUnwind
	};

	struct EXCEPTION_REGISTRATION_RECORD {
		EXCEPTION_REGISTRATION_RECORD* Next;
		EXCEPTION_DISPOSITION Handler;
	};

	struct NT_TIB {
		EXCEPTION_REGISTRATION_RECORD* ExceptionList;
		ptr StackBase;
		ptr StackLimit;
		ptr SubSystemTib;

		union {
			ptr FiberData;
			u32 Version;
		};

		ptr ArbitraryUserPointer;
		NT_TIB* Self;
	};

	struct CLIENT_ID {
		ptr UniqueProcess;
		ptr UniqueThread;
	};

	struct LIST_ENTRY {
		LIST_ENTRY* Flink;
		LIST_ENTRY* Blink;
	};

	struct PEB_LDR_DATA {
		u32 Length;
		u8 Initialized;
		ptr SsHandle;
		LIST_ENTRY InLoadOrderModuleList;
		LIST_ENTRY InMemoryOrderModuleList;
	};

	struct UNICODE_STRING {
		u16 Length;
		u16 MaximumLength;
		wstr Buffer;
	};

	struct LDR_DATA_TABLE_ENTRY {
		LIST_ENTRY InLoadOrderLinks;
		LIST_ENTRY InMemoryOrderLinks;
		LIST_ENTRY InInitializationOrderLinks;
		ptr DllBase;
		ptr EntryPoint;
		u32 SizeOfImage;
		UNICODE_STRING FullDllName;
		UNICODE_STRING BaseDllName;
	};

	struct PEB {
		u8 InheritedAddressSpace;
		u8 ReadImageFileExecOptions;
		u8 BeingDebugged;

		union {
			u8 BitField;

			struct {
				u8 ImageUsesLargePages          : 1;
				u8 IsProtectedProcess           : 1;
				u8 IsLegacyProcess              : 1;
				u8 IsImageDynamicallyRelocated  : 1;
				u8 SkipPatchingUser32Forwarders : 1;
				u8 SpareBits                    : 1 // 3;
			};
		};

		ptr Mutant;
		ptr ImageBaseAddress;
		PEB_LDR_DATA* Ldr;
	};
} // namespace mem::win32

#endif // WIN32_HPP
