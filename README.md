# executor_parameters
A demo of generalized executor parameters for `post()` & `defer()` use cases

The jist of the idea is for executors to optionally expose a `param_type`
constructor argument. The meaning of this parameter is executor-defined. It has
no semantic meaning to executor customization points such as
`std::experimental::execution::execute()`.

For example, a simple one-way executor with a construction parameter could look like this:

    struct my_executor
    {
      // expose param_type as a member type
      // see e.g. std::uniform_int_distribution::param_type
      using param_type = ...

      // construct from a param_type
      my_executor(const param_type& parameter = param_type());

      // copy construct from another executor and an optional parameter
      my_executor(const my_executor& other, const param_type& parameter = other.param());

      // get the parameter this was constructed with
      const param_type& param() const;

      // the execute() interface doesn't expose the parameter 
      template<class Function>
      void execute(Function&& f) const;
    };

`defer_demo.cpp` and `post_demo.cpp` demonstrate the construction of a
non-blocking executor with a parameter suggesting how calls to `.execute()`
should be interpreted. `defer()` uses the parameter to suggest that
`.execute()` should treat functions as continuations.

The build the programs, run

    $ make

The performance of `defer_demo` should match `looper_continuations` while the
performance of `post_demo` should match `looper`.

For example, on my workstation:

    $ make -j8
    g++ -std=c++11 -O3 -DBOOST_ERROR_CODE_HEADER_ONLY -DIS_CONTINUATION=true -pthread looper.cpp -o looper_continuations
    g++ -std=c++11 -O3 -DBOOST_ERROR_CODE_HEADER_ONLY -pthread looper.cpp -o looper
    g++ -std=c++11 -O3 -DBOOST_ERROR_CODE_HEADER_ONLY -pthread defer_demo.cpp -o defer_demo
    g++ -std=c++11 -O3 -DBOOST_ERROR_CODE_HEADER_ONLY -pthread post_demo.cpp -o post_demo
    $ time ./looper
    
    real	0m50.478s
    user	0m50.472s
    sys	0m0.000s
    $ time ./looper_continuations 
    
    real	0m23.358s
    user	0m23.312s
    sys	0m0.020s
    $ time ./post_demo 
    
    real	0m49.467s
    user	0m49.452s
    sys	0m0.000s
    $ time ./defer_demo 
    
    real	0m24.383s
    user	0m24.376s
    sys	0m0.000s


