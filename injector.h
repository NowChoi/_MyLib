/**----------------------------------------------------------------------------
 * injector.h
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * All rights reserved by somma (fixbrain@gmail.com, unsorted@msn.com)
 *-----------------------------------------------------------------------------
 * 
**---------------------------------------------------------------------------*/
#ifndef _injector_h_
#define _injector_h_

typedef		void*	injector_context;

bool inject_dll(_In_ DWORD pid, _In_ const char* dll_path);



#endif//_injector_h_