#ifndef ZAD_UTILS_H
#define ZAD_UTILS_H

#include <memory>

// C++11 ...
template<typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&& ...args )
{
    return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
}


#endif //ZAD_UTILS_H
