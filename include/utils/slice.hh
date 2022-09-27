#include <vector>

namespace ppc {
    template <class T>
    class slice_t {
    private:
        T *iterable;
        std::size_t start;
        std::size_t n;
    public:
        auto begin() const { return iterable->begin() + start; }
        auto end() const { return iterable->end() + start + n; }

        auto size() const { return n; }
        auto &operator[](std::size_t i) { return iterable[start + i]; }
        const auto &operator[](std::size_t i) const { return iterable[start + i]; }

        slice_t(T &iterable, std::size_t start, std::size_t n) {
            this->iterable = &iterable;
            this->start = start;
            this->n = n;
            if (n == -1u) this->n = iterable.size() - start;
        }
    };


    template <class T>
    inline slice_t<T> slice(slice_t<T> &sl) {
        return slice_t<T>(sl.iterable, sl.start, sl.n);
    }
    template <class T>
    inline slice_t<T> slice(slice_t<T> &sl, std::size_t start) {
        return slice_t<T>(sl.iterable, sl.start + start, sl.n);
    }
    template <class T>
    inline slice_t<T> slice(slice_t<T> &sl, std::size_t start, std::size_t n) {
        return slice_t<T>(sl.iterable, sl.start + start, n);
    }

    template <class T>
    inline slice_t<T> slice(T &vec) {
        return slice_t<T>(vec, 0, vec.size());
    }
    template <class T>
    inline slice_t<T> slice(T &vec, std::size_t start) {
        return slice_t<T>(vec, start, vec.size());
    }
    template <class T>
    inline slice_t<T> slice(T &vec, std::size_t start, std::size_t n) {
        return slice_t<T>(vec, start, n);
    }
}