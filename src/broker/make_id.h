#ifndef __MAKE_ID_H__
#define __MAKE_ID_H__

#include <cstdint>
#include <mutex>

class MakeId
{
public:
    uint64_t request()
    {
        std::lock_guard<std::mutex> glck(m_mutex);
        return m_count++;
    }

private:
    uint64_t m_count = 0;
    std::mutex m_mutex;
};

#endif // __MAKE_ID_H__