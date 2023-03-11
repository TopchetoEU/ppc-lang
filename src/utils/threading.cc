#include "utils/threading.hh"
#include <string>

using namespace ppc;
using namespace std::string_literals;

#ifdef WINDOWS

#include <windows.h>

threading::thread_t threading::thread_t::start_impl(void *func, void *args) {
    DWORD id;
    HANDLE hnd = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) func, args, 0, &id);

    if (!hnd) throw "Couldn't create thread."s;
    return { hnd };
}
int threading::thread_t::join() const {
    if (WaitForSingleObject(handle, INFINITE) == WAIT_FAILED) {
        return GetLastError();
    }
    return 0;
}
threading::thread_t::~thread_t() {
    CloseHandle(handle);
}

#elif LINUX

#include <pthread.h>

threading::thread_t threading::thread_t::start_impl(void *func, void *args) {
    pthread_t *handle = new pthread_t;
    pthread_create(handle, nullptr, (void *(*) (void *) ) func, args);
    return { handle };
}
int threading::thread_t::join() const {
    return pthread_join(*(pthread_t *) handle, nullptr);
}
threading::thread_t::~thread_t() {
    delete (pthread_t *) handle;
}

#endif