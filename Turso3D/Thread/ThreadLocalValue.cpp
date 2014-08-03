// For conditions of distribution and use, see copyright notice in License.txt

#include "ThreadLocalValue.h"

#ifdef WIN32
#include <windows.h>
#endif

#include "../Debug/DebugNew.h"

namespace Turso3D
{

ThreadLocalValue::ThreadLocalValue()
{
    #ifdef WIN32
    key = TlsAlloc();
    valid = key != TLS_OUT_OF_INDEXES;
    #else
    valid = pthread_key_create(&key, 0) == 0;
    #endif
}

ThreadLocalValue::~ThreadLocalValue()
{
    if (valid)
    {
        #ifdef WIN32
        TlsFree(key);
        #else
        pthread_key_delete(key);
        #endif
    }
}

void ThreadLocalValue::SetValue(void* value)
{
    if (valid)
    {
        #ifdef WIN32
        TlsSetValue(key, value);
        #else
        pthread_setspecific(key, value);
        #endif
    }
}

void* ThreadLocalValue::Value() const
{
    if (valid)
    {
        #ifdef WIN32
        return TlsGetValue(key);
        #else
        return pthread_getspecific(key);
        #endif
    }
    else
        return 0;
}

}