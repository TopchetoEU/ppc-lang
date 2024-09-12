#pragma once

#ifdef WINDOWS
#define THREAD __stdcall
#else
#define THREAD
#endif

namespace ppc::threading {
    template <class ParserT>
    using thread_func_t = int (THREAD *)(ParserT *data);
    using empty_thread_func_t = int (THREAD *)();

    struct thread_t {
    private:
        void *handle;

        static thread_t start_impl(void *func, void *args);
    public:
        int join() const;

        thread_t(void *handle) { this->handle = handle; }
        template <class ParserT>
        inline static thread_t start(thread_func_t<ParserT> func, const ParserT &args) {
            ParserT _args = args;
            return start_impl((void*)func, &_args);
        }
        template <class ParserT>
        inline static thread_t start(thread_func_t<ParserT> func, ParserT &args) {
            return start_impl((void*)func, &args);
        }
        inline static thread_t start(empty_thread_func_t func) {
            return start<int>((thread_func_t<int>)func, 0);
        }

        ~thread_t();
    };
}

