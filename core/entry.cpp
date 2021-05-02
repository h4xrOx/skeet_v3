#include "../includes.h"

int __stdcall DllMain( HMODULE self, ulong_t reason, void *reserved ) {
    if( reason == DLL_PROCESS_ATTACH ) {

#ifndef KOLO
        HANDLE thread = CreateThread( nullptr, 0, Client::init, reserved, 0, nullptr );
        if( !thread )
            return 0;

        CloseHandle( thread );
#else
		Client::init( reserved );
#endif
        return 1;
    }

    return 0;
}