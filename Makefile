all: looper_continuations looper defer_demo post_demo

looper_continuations: looper.cpp
	g++ -std=c++11 -O3 -DBOOST_ERROR_CODE_HEADER_ONLY -DIS_CONTINUATION=true -pthread looper.cpp -o looper_continuations

looper: looper.cpp
	g++ -std=c++11 -O3 -DBOOST_ERROR_CODE_HEADER_ONLY -pthread looper.cpp -o looper

defer_demo: defer_demo.cpp
	g++ -std=c++11 -O3 -DBOOST_ERROR_CODE_HEADER_ONLY -pthread defer_demo.cpp -o defer_demo

post_demo: post_demo.cpp
	g++ -std=c++11 -O3 -DBOOST_ERROR_CODE_HEADER_ONLY -pthread post_demo.cpp -o post_demo

clean:
	rm -f looper_continuations looper defer_demo post_demo
