/**----------------------------------------------------------------------------
 * Win32Util.h
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * All rights reserved by Noh,Yonghwan (fixbrain@gmail.com, unsorted@msn.com)
 *-----------------------------------------------------------------------------
 * 26:8:2011   15:31 created
**---------------------------------------------------------------------------*/
#ifndef _win32_utils_
#define _win32_utils_

#include "boost/shared_ptr.hpp"			// boost::shared_ptr
#include "boost/type_traits.hpp"		// boost::remove_pointer
#include "boost/algorithm/string.hpp"	// to_uppper, to_lower

#include <iosfwd>
#include <sstream>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

//> reported as vs 2010 bug, ms says that will be patch this bug next major vs release, vs2012.
//
//1>C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\include\intsafe.h(171): warning C4005: 'INT16_MAX' : macro redefinition
//1>          C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include\stdint.h(77) : see previous definition of 'INT16_MAX'
#pragma warning(disable:4005)
#include <intsafe.h>
#pragma warning(default:4005)



// | size | _______data_______ | ������ �޸� ����
// byte align ������ �Ϻη� buf[4] �� ������
// 
typedef struct _continuous_memory
{
    DWORD   size;   // buf size
    CHAR    buf[4];
} CTM, *PCTM;
#define CTM_SIZE( _data_size_ )   ((_data_size_) + sizeof(DWORD))

#define WSTRING_SIZE(_std_wstring_)	(DWORD)((_std_wstring_.size() + 1) * sizeof(WCHAR))
#define STRING_SIZE(_std_string_)   (DWORD)((_std_string_.size() + 1) * sizeof(CHAR))


#define _HIGH_PART(_int64_value_)    (UINT32)((_int64_value_) >> 32)
#define _LOW_PART(_int64_value_)     (UINT32)((_int64_value_) & 0x00000000FFFFFFFF)


/**	-----------------------------------------------------------------------
	����ÿ� TODO �޼����� ����ϱ� ���� ��ũ�� 

	#pragma TODO( "��� �����ϼ�	\n" )

	�䷸�� ����ϸ� �� 
	IT EXPERT, ������ ���α׷��Ӹ� ���� MFC ������ ����, ������ ��
-------------------------------------------------------------------------*/
#ifndef _TODO_DEFINED_
	#define LINE1(x)	#x
	#define LINE(x)		LINE1(x)
	#define TODO(msg)	message(  __FILE__"(" LINE(__LINE__) ") [TODO] " msg )
#endif

#define FreeAndNil(p)     do { if (NULL != p) { free(p); p = NULL;} } while(p != NULL);



/* 
	x64 ������ inline asm �� ����� �� �����Ƿ� ȣȯ���� ���� ����
	��� StopWatch.h ���Ͽ� ���ǵ� Ŭ������ ����ϸ� ��


// out-of-order execution �� ���� ���� cpuid ȣ��
//
static __inline LARGE_INTEGER getticks(void)
{
	LARGE_INTEGER retval={0};
	__asm 
	{
		cpuid
		rdtsc
		mov dword ptr retval.HighPart, edx
		mov dword ptr retval.LowPart, eax
	}
	return retval;
}

*/

//=============================================================================
// �ð� ����
//=============================================================================
#ifndef _FAT_TEIME_DEFINDED_
#define _FAT_TEIME_DEFINDED_
typedef struct _FATTIME
{
	USHORT		usDate;
	USHORT		usTime;
} FATTIME;

#endif//_FAT_TEIME_DEFINDED_

LPCWSTR FT2Str(IN FILETIME& ft);
LPCWSTR FAT2Str(IN FATTIME& fat);

/******************************************************************************
 * ���� ó��
******************************************************************************/
bool is_file_existsW(_In_ const wchar_t* file_path);
bool IsFileExistA(const CHAR* FilePath);

bool get_filepath_by_handle(_In_ HANDLE file_handle, _Out_ std::wstring& file_name);
bool get_mapped_file_name(_In_ HANDLE process_handle, _In_ const void* mapped_addr, _Out_ std::wstring& file_name);
bool nt_name_to_dos_name(_In_ const wchar_t* nt_name, _Out_ std::wstring& dos_name);
bool query_dos_device(_In_ const wchar_t* dos_device, _Out_ std::wstring& nt_device);

HANDLE open_file_to_write(_In_ const wchar_t* file_path);
HANDLE OpenFileToRead(LPCWCH file_path);
BOOL WriteToFileW(LPCWCH file_path, LPCWCH format,...);
BOOL WriteToFileW(HANDLE hFile, LPCWCH format, ...);
BOOL WriteToFileA(HANDLE hFile, LPCCH format, ...);

BOOL SaveToFileAsUTF8A(
                IN LPCWSTR FilePathDoesNotExists, 
                IN LPCSTR NullTerminatedAsciiString
                );
BOOL SaveToFileAsUTF8W(
                IN LPCWSTR FilePathDoesNotExists, 
                IN LPCWSTR NullTerminatedWideString
                );
BOOL LoadFileToMemory(
                IN LPCWSTR FilePath, 
                OUT DWORD& MemorySize, 
                OUT PBYTE& Memory
                );
BOOL SaveBinaryFile(
                IN LPCWSTR  Directory,
                IN LPCWSTR  FileName, 
                IN DWORD    Size,
                IN PBYTE    Data
                );

typedef BOOL (__stdcall *fnFindFilesCallback)(IN DWORD_PTR Tag, IN LPCWSTR path);

bool 
find_file_recursive(
	_In_ const wchar_t* root, 
	_In_ fnFindFilesCallback cb, 
	_In_ DWORD_PTR tag
	);

BOOL 
FindSubDirectory(
	IN LPCWSTR RootPath,
	IN OUT std::vector<std::wstring>& DirectoryList, 
	IN BOOL Recursive
	);

/******************************************************************************
 * directory management 
******************************************************************************/
BOOL WUGetCurrentDirectoryW(IN OUT std::wstring& CurrentDir);
BOOL WUGetCurrentDirectoryA(IN OUT std::string& CurrentDir);

bool get_current_module_path(_Out_ std::wstring& module_path);
bool get_current_module_dir(_Out_ std::wstring& module_dir);
bool get_current_module_file(_Out_ std::wstring& module_file);
bool get_system_directory(_Out_ std::wstring& system_dir);

BOOL WUCreateDirectory(const LPCWSTR DirectoryPath);
BOOL WUDeleteDirectoryW(IN LPCWSTR  DirctoryPathToDelete);
BOOL GetSystemRootDirectory(DWORD Len, LPTSTR Buffer);
BOOL DeleteDirectory(IN LPCWSTR  DirctoryPathToDelete);
BOOL GetImageFullPathFromPredefinedPathW(
                IN  LPCWSTR ImageName, 
                IN  DWORD   BufferLen,
                OUT LPWSTR  FullPathBuffer
                );
BOOL GetImageFullPathFromPredefinedPathA(
                IN  LPCSTR ImageName, 
                IN  DWORD  FullPathBufferLen,
                OUT LPSTR  FullPathBuffer
                );
#ifdef UNICODE
#define GetImageFullPathFromPredefinedPath  GetImageFullPathFromPredefinedPathW
#else
#define GetImageFullPathFromPredefinedPath  GetImageFullPathFromPredefinedPathA	
#endif//UNICODE

/******************************************************************************
 * ���ڿ� ó��
******************************************************************************/
wchar_t* MbsToWcs(_In_ const char* mbs);
char* WcsToMbs(_In_ const wchar_t* wcs);
char* WcsToMbsUTF8(IN const wchar_t* wcs);
std::wstring MbsToWcsEx(_In_ const char *mbs);
std::string WcsToMbsEx(_In_ const wchar_t *wcs);
std::string WcsToMbsUTF8Ex(_In_ const wchar_t *wcs);

//> T = std::string || std::wstring
template <typename T> void to_upper_string(_Inout_ T& input){ boost::algorithm::to_upper(input);}
template <typename T> void to_lower_string(_Inout_ T& input){ boost::algorithm::to_lower(input);}

bool 
extract_first_tokenW(
	_In_ std::wstring& org_string,
	_In_ const std::wstring& token,
	_Out_ std::wstring& out_string, 
	_In_ bool forward,
	_In_ bool delete_token
	);

bool
extract_first_tokenA(
		_In_ std::string& org_string,
		_In_ const std::string& token,
		_Out_ std::string& out_string, 
		_In_ bool forward,
		_In_ bool delete_token
		);

bool 
extract_last_tokenW(
	_In_ std::wstring& org_string,
	_In_ const std::wstring& token,
	_Out_ std::wstring& out_string, 
	_In_ bool forward,
	_In_ bool delete_token
	);

bool 
extract_last_tokenA(
	_In_ std::string& org_string,
	_In_ const std::string& token,
	_Out_ std::string& out_string, 
	_In_ bool forward,
	_In_ bool delete_token
    );

std::string  trima(std::string& s, const std::string& drop = " ");
std::string rtrima(std::string& s, const std::string& drop = " ");
std::string ltrima(std::string& s, const std::string& drop = " ");

std::wstring  trimw(std::wstring& s, const std::wstring& drop = L" ");
std::wstring rtrimw(std::wstring& s, const std::wstring& drop = L" ");
std::wstring ltrimw(std::wstring& s, const std::wstring& drop = L" ");

#ifdef UNICODE
	#define ExtractFirstToken	ExtractFirstTokenW
	#define ExtractLastToken	ExtractLastTokenW
#else
	#define ExtractFirstToken	ExtractFirstTokenA
	#define ExtractLastToken	ExtractLastTokenA
#endif//UNICODE

/**
* @brief	source ���� find ���ڿ��� ��� ã�� replace ���ڿ��� �����Ѵ�.
*/
template <typename T> int		
FindAndReplace(IN T& source, IN T& find, IN T replace)
{
	int count = 0;
	size_t pos = source.find(find, 0);
	while (T::npos != pos)
	{
		source.replace(pos, find.length(), replace);
		++count;
		pos = source.find(find, pos);
	}
	return count;
}

/******************************************************************************
 * RAII (Resource Acquisition Is Initialization )
******************************************************************************/
/*	ex)
	raii_handle map_handle(
					CreateFileMapping(file_handle, NULL, PAGE_READONLY, 0, 1, NULL), 
					raii_CloseHandle
					);
	if (NULL == map_handle.get())
	{
		err...
	}
*/
typedef boost::shared_ptr< boost::remove_pointer<HANDLE>::type > raii_handle;
void raii_CloseHandle(_In_ HANDLE handle);

/* 
	raii_void_ptr ods_buf( malloc(ods_size), raii_free );
	if (NULL == ods_buf.get())
	{
		err...
	}
	...
	ods_buf.get();
	...


	raii_void_ptr map_ptr(
					MapViewOfFile(map_handle.get(), FILE_MAP_READ, 0, 0, 1), 
					raii_UnmapViewOfFile
					);
	if (NULL == map_ptr.get())
	{
		err...
	}

*/
typedef boost::shared_ptr< boost::remove_pointer<wchar_t*>::type > raii_wchar_ptr;
typedef boost::shared_ptr< boost::remove_pointer<char*>::type > raii_char_ptr;
typedef boost::shared_ptr< boost::remove_pointer<void*>::type > raii_void_ptr;
void raii_free(_In_ void* void_ptr);
void raii_UnmapViewOfFile(_In_ void* void_ptr);







/******************************************************************************
 * API wrapper
******************************************************************************/
std::string Win32ErrorToStringA(IN DWORD ErrorCode);
std::wstring Win32ErrorToStringW(IN DWORD ErrorCode);

BOOL DumpMemory(DWORD Length, BYTE* Buf);
BOOL DumpMemory(FILE* stream,DWORD Length,BYTE* Buf);
BOOL GetTimeStringA(OUT std::string& TimeString);
BOOL GetTimeStringW(IN std::wstring& TimeString);
bool get_local_ip_list(_In_ std::wstring& host_name, _In_ std::vector<std::wstring>& ip_list);

bool set_privilege(_In_z_ const wchar_t* privilege, _In_ bool enable);
HANDLE privileged_open_process(_In_ DWORD pid, _In_ DWORD rights, _In_ bool raise_privilege);


/******************************************************************************
 * console stuff
******************************************************************************/
/*
	int main()
    {
        COORD curXY;
        int i;
        printf("This number will change: ");

        curXY = GetCurCoords();
        for(i = 0; i < 10; i++)
        {
            printf("%d", i);
            Sleep(500);
            SetConsoleCoords(curXY);
        }
        return 0;
    }
*/
void SetConsoleCoords(COORD xy);
COORD GetCurCoords(void);


#define LL_DEBG		0
#define LL_INFO		1
#define LL_ERRR		2
#define LL_NONE		3

#define con_err		write_to_console( LL_ERRR, __FUNCTIONW__, 
#define con_info	write_to_console( LL_INFO, __FUNCTIONW__, 
#define con_dbg		write_to_console( LL_DEBG, __FUNCTIONW__, 
#define con_msg		write_to_console( LL_NONE, __FUNCTIONW__, 
#define con_end		);

#ifndef _slogger_included
	//> slogger �� ������� �ʴ� ���
	#define log_err		con_err
	#define log_dbg		con_dbg
	#define log_info	con_info
	#define	log_msg		con_msg
	#define log_end		con_end
	
#endif //_slogger_included

void write_to_console(_In_ DWORD log_level, _In_ wchar_t* function, _In_ wchar_t* format, ...);



/******************************************************************************
 * PE stuff
******************************************************************************/
/* 
    cfix_user.dll= IMAGE_FILE_DLL IMAGE_SUBSYSTEM_WINDOWS_GUI     
    C:\WINDOWS\$NtUninstallKB973869$\dhtmled.ocx= IMAGE_FILE_DLL IMAGE_SUBSYSTEM_WINDOWS_GUI 
    c:\windows\system32\kernel32.dll= IMAGE_FILE_DLL IMAGE_SUBSYSTEM_NATIVE IMAGE_SUBSYSTEM_WINDOWS_GUI IMAGE_SUBSYSTEM_WINDOWS_CUI 

    c:\windows\system32\notepad.exe= IMAGE_SUBSYSTEM_WINDOWS_GUI 

    c:\windows\system32\ntoskrnl.exe= IMAGE_SUBSYSTEM_NATIVE 
    c:\windows\system32\win32k.sys= IMAGE_SUBSYSTEM_NATIVE 
    c:\windows\system32\csrss.exe= IMAGE_SUBSYSTEM_NATIVE 
    c:\windows\system32\smss.exe= IMAGE_SUBSYSTEM_NATIVE 

    c:\windows\system32\winlogon.exe= IMAGE_SUBSYSTEM_WINDOWS_GUI 
    c:\windows\system32\services.exe= IMAGE_SUBSYSTEM_WINDOWS_GUI 
    
*/
typedef enum _IMAGE_TYPE 
{
    IT_DLL,                     // dll, ocx file    
    IT_EXE_GUI,                 // gui app
    IT_EXE_CUI,                 // cui app
    IT_NATIVE_APP,              // ntoskrnl.exe, win32k.sys, csrss.exe
    IT_NORMAL                   // unknown or not executable or invalid image
} IMAGE_TYPE;

BOOL     IsExecutableFile(LPCTSTR path, IMAGE_TYPE& type);
LPCWSTR  FileTypeToString(IMAGE_TYPE type);


/******************************************************************************
 * type cast
******************************************************************************/
bool 
bin_to_hexa(
	_In_ UINT32 code_size, 
	_In_ const UINT8* code, 
	_In_ bool upper_case, 
	_Out_ std::string& hex_string
	);

bool 
bin_to_hexw(
	_In_ UINT32 code_size, 
	_In_ const UINT8* code, 
	_In_ bool upper_case, 
	_Out_ std::wstring& hex_string
	);

bool str_to_int32(_In_ const char* int32_string, _Out_ INT32& int32_val);
bool str_to_uint32(_In_ const char* uint32_string, _Out_ UINT32& uint32_val);
bool str_to_int64(_In_ const char* int64_string, _Out_ INT64& int64_val);
bool str_to_uint64(_In_ const char* uint64_string, _Out_ UINT64& uint64_val);
    

bool wstr_to_int32(_In_ const wchar_t* int32_string, _Out_ INT32& int32_val);
bool wstr_to_uint32(_In_ const wchar_t* uint32_string, _Out_ UINT32& uint32_val);
bool wstr_to_int64(_In_ const wchar_t* int64_string, _Out_ INT64& int64_val);
bool wstr_to_uint64(_In_ const wchar_t* uint64_string, _Out_ UINT64& uint64_val);

UINT16 swap_endian_16(_In_ UINT16 value); 
UINT32 swap_endian_32(_In_ UINT32 value); 
UINT64 swap_endian_64(_In_ UINT64 value);



// Window �÷����ڵ�
//
#define OSTYPE_WIN_95		        0x0001
#define OSTYPE_WIN_98		        0x0002
#define OSTYPE_WIN_SE		        0x0003
#define OSTYPE_WIN_ME		        0x0004

#define	OSTYPE_WIN_NT		        0x0005      // 4.0

#define OSTYPE_WIN_2000		        0x0006      // 5.0
#define OSTYPE_WIN_XP		        0x0007      // 5.1
#define OSTYPE_WIN_XP_64            0x0008      // 5.2
#define OSTYPE_WIN_2003		        0x0009      // 5.2
#define OSTYPE_WIN_HOME_SERVER      0x000A      // 5.2
#define OSTYPE_WIN_2003_R2		    0x000B      // 5.2
#define OSTYPE_WIN_VISTA	        0x000C      // 6.0  
#define OSTYPE_WIN_SERVER_2008      0x000D      // 6.0  
#define OSTYPE_WIN_7				0x000E      // 6.1
#define OSTYPE_WIN_SERVER_2008_R2   0x000F      // 6.1
#define OSTYPE_WIN_8                0x0010      // 6.2
#define OSTYPE_WIN_SERVER_2012		0x0011      // 6.2


#define	ISNTFAMILY(_OSTYPE_WIN_XXX_)	((_OSTYPE_WIN_XXX_) >= OSTYPE_WIN_NT ? TRUE : FALSE)

WORD OsVersion();


typedef struct WU_PROCESSOR_INFO
{
    DWORD   NumaNodeCount;
    DWORD   PhysicalCpuPackageCount;
    DWORD   PhysicalCpuCoreCount;
    DWORD   LogicalCpuCount;

    DWORD   L1Cache;
    DWORD   L2Cache;
    DWORD   L3Cache;
} *PWU_PROCESSOR_INFO;

BOOL WUGetProcessorInfo(IN OUT WU_PROCESSOR_INFO& CpuInfo);

#endif//_win32_utils_