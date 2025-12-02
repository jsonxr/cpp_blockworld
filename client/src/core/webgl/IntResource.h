#ifndef APP_WEBGL_WEBGLRESOURCE_H
#define APP_WEBGL_WEBGLRESOURCE_H

#include <cstddef>
#include <cstdint>
#include <memory>

namespace app {

// This did not work for emscripten

// https://stackoverflow.com/questions/66470815/how-to-allow-a-stdunique-ptr-to-access-a-classs-private-destructor-or-impleme

// "pointers" in unique ptrs must be comparable to nullptr.
// So, let us make an integer qualify:
using GLuint=std::uint32_t;

template<class Int>
struct Nullable{
  Int val=0;
  Nullable()=default;
  [[maybe_unused]] Nullable(Int v):val(v){}
  [[maybe_unused]] Nullable(std::nullptr_t){}
  friend auto operator==(std::nullptr_t, Nullable const& self) -> bool{return !static_cast<bool>(self);}
  friend auto operator!=(std::nullptr_t, Nullable const& self) -> bool{return static_cast<bool>(self);}
  friend auto operator==(Nullable const& self, std::nullptr_t) -> bool{return !static_cast<bool>(self);}
  friend auto operator!=(Nullable const& self, std::nullptr_t) -> bool{return static_cast<bool>(self);}
  operator Int()const{return val;}
};

template<class Int, auto& deleter>
struct IntDeleter;

template<class Int, class...Args, void(*&deleter)(Int, Args...)>
struct IntDeleter<Int, deleter>:
    std::tuple<std::decay_t<Args>...>
{
  using base = std::tuple<std::decay_t<Args>...>;
  using base::base;
  using pointer=Nullable<Int>;
  void operator()(pointer p)const{
    std::apply([&p](std::decay_t<Args> const&...args)->void{
      deleter(p, args...);
    }, static_cast<base const&>(*this));
  }
};

template<class Int, void(*&deleter)(Int)>
using IntResource=std::unique_ptr<Int, IntDeleter<Int,deleter>>;

}

#endif  // APP_WEBGL_WEBGLRESOURCE_H
