#ifndef NONCOPYABLE_H_INCLUDED_
#define NONCOPYABLE_H_INCLUDED_

class NonCopyable
{
protected:
        NonCopyable() noexcept = default;
        ~NonCopyable() = default;

    private:
        NonCopyable(NonCopyable const&) = delete;
        NonCopyable& operator=(NonCopyable const&) = delete;
};

#endif
