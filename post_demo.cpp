#include <boost/asio.hpp>

boost::asio::io_service io_service;
int count = 0;

namespace std
{
namespace experimental
{


enum class bikeshed
{
  execute_as_continuation,
  execute_not_as_continuation
};


template<class NonblockingExecutor, class Function>
void defer(const NonblockingExecutor& exec, Function&& f)
{
  // create a copy of exec with the deferring parameter
  // NonblockingExecutor would include requirements that this kind of construction is possible
  NonblockingExecutor deferring_exec(exec, bikeshed::execute_as_continuation);

  // execute with the deferring executor
  deferring_exec.execute();

  // XXX for completeness, another overload of defer() would simply call execute() if the executor doesn't understand the parameter
}

template<class NonblockingExecutor, class Function>
void post(const NonblockingExecutor& exec, Function&& f)
{
  // create a copy of exec with the posting parameter
  // NonblockingExecutor would include requirements that this kind of construction is possible
  NonblockingExecutor posting_exec(exec, bikeshed::execute_not_as_continuation);

  // execute with the posting executor
  posting_exec.execute();

  // XXX for completeness, another overload of post() would simply call execute() if the executor doesn't understand the parameter
}


} // end experimental
} // end std


struct my_executor
{
  // see e.g. std::uniform_int_distribution for param_type inspiration 
  using param_type = std::experimental::bikeshed;

  my_executor(const param_type& p = param_type())
    : param_(p)
  {}

  // copy construct from other with parameter
  my_executor(const my_executor&, const param_type& p)
    : param_(p)
  {}

  param_type param() const
  {
    return param_;
  }

  void execute()
  {
    if(count++ < 1000000000)
    {
      io_service.post(*this);
    }
  }

  void operator()()
  {
    execute();
  }

  friend bool asio_handler_is_continuation(my_executor* self)
  {
    return self->param_ == std::experimental::bikeshed::execute_as_continuation;
  }

  param_type param_;
};


int main()
{
  my_executor exec;

  for(int i = 0; i < 100; ++i)
  {
    std::experimental::post(exec, []{});
  }

  io_service.run();
}

