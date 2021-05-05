#ifndef _MY_TEST_HELPER_
#define _MY_TEST_HELPER_

namespace MyHelper
{
    template <typename T1, typename T2>
    bool isSame(std::vector<T1> &a, std::vector<T2> &b)
    {
        if (a.size() != b.size())
        {
            return false;
        }
        for (size_t i = 0; i < a.size(); i++)
        {
            if (a[i] != b[i])
            {
                return false;
            }
        }
        return true;
    }
}

#endif