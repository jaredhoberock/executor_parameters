// looper.cpp --------------------
#include <boost/asio.hpp>

boost::asio::io_service io_service;
int count = 0;

struct looper
{
  void operator()()
  {
    if (count++ < 1000000000)
    {
      io_service.post(*this);
    }
  }

#ifdef IS_CONTINUATION
  friend bool asio_handler_is_continuation(looper*)
  {
    return IS_CONTINUATION;
  }
#endif
};

int main()
{
  for (int i = 0; i < 100; ++i)
    looper()();
  io_service.run();
}

