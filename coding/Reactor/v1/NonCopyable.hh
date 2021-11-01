#pragma once

namespace wd
{

class NonCopyable
{
protected:
    NonCopyable() {}
    ~NonCopyable() {}
    NonCopyable(const NonCopyable &) = delete;
    NonCopyable &operator=(const NonCopyable &) = delete;
};

} // end of namespace wd