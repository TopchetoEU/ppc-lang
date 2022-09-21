#pragma once

#ifdef WINDOWS
#define THREAD __stdcall
#else
#define THREAD
#endif

namespace ppc::threading {
    template <class T>
    using thread_func_t = int (THREAD *)(T *data);
    using empty_thread_func_t = int (THREAD *)();

    struct thread_t {
    private:
        void *handle;

        static thread_t start_impl(void *func, void *args);
    public:
        int join() const;

        thread_t(void *handle) { this->handle = handle; }
        template <class T>
        inline static thread_t start(thread_func_t<T> func, const T &args) {
            T _args = args;
            return start_impl((void*)func, &_args);
        }
        template <class T>
        inline static thread_t start(thread_func_t<T> func, T &args) {
            return start_impl((void*)func, &args);
        }
        inline static thread_t start(empty_thread_func_t func) {
            return start<int>((thread_func_t<int>)func, 0);
        }

        ~thread_t();
    };
}

