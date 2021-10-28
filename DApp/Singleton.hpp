//
// Created by outlaw on 22.03.2021.
//
#ifndef DISP_SINGLETON_HPP
#define DISP_SINGLETON_HPP

#include <type_traits>

class RomObject{};

//Синглтон для RAM объектов
template<typename T, class Enable = void>
class Singleton {
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator = (const Singleton&) = delete;
    Singleton() =  delete;

    constexpr static T* GetInstance(){
        return &instance;
    }
private:
    static T instance;
};
template<typename T, class Enable>
T Singleton<T,Enable>::instance;


////Синглтон для ROM объектов
template<typename T>
class Singleton<T, typename std::enable_if_t<std::is_base_of<RomObject, T>::value>> {
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator = (const Singleton&) = delete;
    Singleton() = delete;

    static constexpr const T* GetInstance()   {
        return &instance;
    }
private:
    static constexpr T instance{T()};
};
template<typename T>
constexpr T Singleton<T, typename std::enable_if_t<std::is_base_of<RomObject, T>::value>>::instance;

#endif //DISP_SINGLETON_HPP