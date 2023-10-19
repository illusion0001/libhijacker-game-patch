#include "dbg/dbg.hpp"
#include "game_patch_memory.hpp"
#include "print.hpp"

uint32_t FlipRate_ConfigureOutput_Ptr;
uint32_t FlipRate_isVideoModeSupported_Ptr;

void hexdump1(void *data, size_t size)
{
#ifdef DEBUG
	if (!data || size <= 0)
	{
		return;
	}
	unsigned char *p = (unsigned char *)data;
	size_t i = 0;

	for (i = 0; i < size; i++)
	{
		printf("%02x", *p++);
		if (!(i % 16) && i != 0)
		{
			printf("\n");
		}
	}

	printf("\n");
#endif
}

// valid hex look up table.
const uint8_t hex_lut[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00};

__attribute__((noinline)) static uint8_t *hexstrtochar2(const char *hexstr,size_t *size)
{
	if (!hexstr || *hexstr == '\0' || !size || *size < 0)
	{
		return nullptr;
	}
	uint32_t str_len = strlen(hexstr);
	size_t data_len = ((str_len + 1) / 2) * sizeof(uint8_t);
	*size = (str_len) * sizeof(uint8_t);
	uint8_t *data = (uint8_t *)malloc(*size);
	if (!data)
	{
		return nullptr;
	}
	uint32_t j = 0; // hexstr position
	uint32_t i = 0; // data position

	if (str_len % 2 == 1)
	{
		data[i] = (uint8_t)(hex_lut[0] << 4) | hex_lut[(uint8_t)hexstr[j]];
		j = ++i;
	}

	for (; j < str_len; j += 2, i++)
	{
		data[i] = (uint8_t)(hex_lut[(uint8_t)hexstr[j]] << 4) |
				  hex_lut[(uint8_t)hexstr[j + 1]];
	}

	*size = data_len;
	return data;
}

void dump_bytes_vm(pid_t pid, uint64_t addr, size_t bytes_size)
{
#ifdef DEBUG
	constexpr size_t MAX_BYTES = 256;
	if (bytes_size >= MAX_BYTES || bytes_size <= 0)
		return;
	char buffer[MAX_BYTES] = {0};
	dbg::read(pid, addr, buffer, bytes_size);
	hexdump1(buffer, bytes_size);
#endif
}

void write_bytes(pid_t pid, uint64_t addr, const char *hexString, enum write_flag special_flag)
{
	uint8_t *byteArray = nullptr;
	size_t bytesize = 0;
	byteArray = hexstrtochar2(hexString, &bytesize);
	if (!byteArray)
	{
		return;
	}
	printf("addr: 0x%lx\n", addr);
	dump_bytes_vm(pid, addr, bytesize);
	dbg::write(pid, addr, byteArray, bytesize);
	dump_bytes_vm(pid, addr, bytesize);
	if (byteArray)
	{
		printf("freeing byteArray at 0x%p\n", byteArray);
		free(byteArray);
	}
	if (special_flag & isOffsetVideoModeSupported &&
		startsWith(hexString, "48050df0a70c") &&
		FlipRate_isVideoModeSupported_Ptr)
	{
		_puts("isOffsetVideoModeSupported");
		write_bytes32(pid, addr + 2, FlipRate_isVideoModeSupported_Ptr);
	}
	else if (special_flag & isOffsetConfigureOutput &&
			 startsWith(hexString, "48050df0ed5e") &&
			 FlipRate_ConfigureOutput_Ptr)
	{
		_puts("isOffsetConfigureOutput");
		write_bytes32(pid, addr + 2, FlipRate_ConfigureOutput_Ptr);
	}
}

void write_bytes32(pid_t pid, uint64_t addr, const uint32_t val)
{
	_printf("addr: 0x%lx\n", addr);
	_printf("val: 0x%08x\n", val);
	dump_bytes_vm(pid, addr, sizeof(uint32_t));
	dbg::write(pid, addr, (void*)&val, sizeof(uint32_t));
	dump_bytes_vm(pid, addr, sizeof(uint32_t));
}

void write_bytes64(pid_t pid, uint64_t addr, const size_t val)
{
	_printf("addr: 0x%lx\n", addr);
	_printf("val: 0x%016lx\n", val);
	dump_bytes_vm(pid, addr, sizeof(size_t));
	dbg::write(pid, addr, (void*)&val, sizeof(size_t));
	dump_bytes_vm(pid, addr, sizeof(size_t));
}

void write_string(pid_t pid, uint64_t addr, const char *string)
{
	_printf("addr: 0x%lx\n", addr);
	_printf("val: %s", string);
	size_t len = strlen(string) + 1;
	dump_bytes_vm(pid, addr, len);
	dbg::write(pid, addr, string, len);
	dump_bytes_vm(pid, addr, len);
}

void write_float32(pid_t pid, uint64_t addr, const float val)
{
	_printf("addr: 0x%lx\n", addr);
	_printf("val: %f\n", val);
	dump_bytes_vm(pid, addr, sizeof(float));
	dbg::write(pid, addr, (void*)&val, sizeof(float));
	dump_bytes_vm(pid, addr, sizeof(float));
}

void write_float64(pid_t pid, uint64_t addr, const double val)
{
	_printf("addr: 0x%lx\n", addr);
	_printf("val: %lf\n", val);
	dump_bytes_vm(pid, addr, sizeof(double));
	dbg::write(pid, addr, (void*)&val, sizeof(double));
	dump_bytes_vm(pid, addr, sizeof(double));
}

// Must use `-fshort-wchar`
// Otherwise, in freebsd target, wchar_t is 32 bit characters 
extern "C" size_t wcslen( const wchar_t *str );

void write_wstring(pid_t pid, uint64_t addr, const wchar_t* string)
{
	size_t len = (wcslen(string) * sizeof(wchar_t)) + (1 * sizeof(wchar_t));
	dump_bytes_vm(pid, addr, len);
	dbg::write(pid, addr, string, len);
	dump_bytes_vm(pid, addr, len);
}

static uint32_t pattern_to_byte(const char *pattern, uint8_t *bytes)
{
	uint32_t count = 0;
	const char *start = pattern;
	const char *end = pattern + strlen(pattern);

	for (const char *current = start; current < end; ++current)
	{
		if (*current == '?')
		{
			++current;
			if (*current == '?')
			{
				++current;
			}
			bytes[count++] = -1;
		}
		else
		{
			bytes[count++] = strtoul(current, (char **)&current, 16);
		}
	}
	return count;
}

/*
 * @brief Scan for a given byte pattern on a module
 *
 * @param module_base Base of the module to search
 * @param module_size Size of the module to search
 * @param signature   IDA-style byte array pattern
 * @credit            https://github.com/OneshotGH/CSGOSimple-master/blob/59c1f2ec655b2fcd20a45881f66bbbc9cd0e562e/CSGOSimple/helpers/utils.cpp#L182
 * @returns           Address of the first occurrence
 */
uint8_t *PatternScan(const uint64_t module_base, const uint64_t module_size, const char *signature)
{
	_printf("module_base: 0x%lx module_size: 0x%lx\n", module_base, module_size);
	if (!module_base || !module_size)
	{
		return nullptr;
	}
	constexpr uint32_t MAX_PATTERN_LENGTH = 256;
	uint8_t patternBytes[MAX_PATTERN_LENGTH];
	(void)memset(patternBytes, 0, MAX_PATTERN_LENGTH);
	int32_t patternLength = pattern_to_byte(signature, patternBytes);
	if (patternLength <= 0 || patternLength >= MAX_PATTERN_LENGTH)
	{
		_printf("Pattern length too large or invalid! %i (0x%08x)\n", patternLength, patternLength);
		_printf("Input Pattern %s\n", signature);
		return nullptr;
	}
	uint8_t *scanBytes = (uint8_t *)module_base;
	for (uint64_t i = 0; i < module_size; ++i)
	{
		bool found = true;
		for (int32_t j = 0; j < patternLength; ++j)
		{
			if (scanBytes[i + j] != patternBytes[j] && patternBytes[j] != 0xff)
			{
				found = false;
				break;
			}
		}
		if (found)
		{
			_printf("found pattern at 0x%p\n", &scanBytes[i]);
			return &scanBytes[i];
		}
	}
	return nullptr;
}

char backupShellCoreBytes[5] = {0};
uint64_t shellcore_offset_patch = 0;

bool patchShellCore(const pid_t app_pid, const uint64_t shellcore_base, const uint64_t shellcore_size)
{
	bool status = false;
	(void)memset(backupShellCoreBytes, 0, sizeof(backupShellCoreBytes));
	shellcore_offset_patch = 0;
	if (!shellcore_base || !shellcore_size)
	{
		return false;
	}
	_printf("allocating 0x%lx bytes\n", shellcore_size);
	char *shellcore_copy = (char *)malloc(shellcore_size);
	_printf("shellcore_copy: 0x%p\n", shellcore_copy);
	if (!shellcore_copy)
	{
		_puts("shellcore_copy is nullptr");
		return false;
	}
	if (dbg::read(app_pid, shellcore_base, shellcore_copy, shellcore_size))
	{
		uint8_t *shellcore_offset = PatternScan((uint64_t)shellcore_copy, shellcore_size, "e8 ?? ?? ?? ?? 48 8d 5c 24 40 48 8d 15 ?? ?? ?? ?? 48 8d 0d ?? ?? ?? ?? 4c 89 fe 41 89 85 74 02 00 00");
		if (shellcore_offset)
		{
			uint64_t offset_to_patch = ((uint64_t)shellcore_offset - (uint64_t)shellcore_copy);
			shellcore_offset_patch = shellcore_base + offset_to_patch;
			_printf("shellcore_offset_patch: 0x%lx\n", shellcore_offset_patch);
			_printf("offset_to_patch: 0x%lx\n", offset_to_patch);
			dbg::read(app_pid, shellcore_offset_patch, backupShellCoreBytes, sizeof(backupShellCoreBytes));
			write_bytes(app_pid, shellcore_offset_patch, "b840100000");
			status = true;
		}
		else
		{
			status = false;
		}
	}
	if (shellcore_copy)
	{
		_printf("freeing shellcore_copy from 0x%p\n", shellcore_copy);
		free(shellcore_copy);
	}
	return status;
}

bool UnPatchShellCore(const pid_t app_pid)
{
	if (dbg::write(app_pid, shellcore_offset_patch, backupShellCoreBytes, sizeof(backupShellCoreBytes)))
	{
		return true;
	}
	return false;
}
