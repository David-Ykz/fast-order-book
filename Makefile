benchmark-random:
	g++ -O3 -o bin/benchmark_random test/benchmark_random.cpp book/book.cpp && bin/benchmark_random
valgrind-benchmark-random:
	g++ -g -O0 -o bin/benchmark_random test/benchmark_random.cpp server/book.cpp && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/benchmark_random
compile-benchmark-random:
	g++ -O3 -o bin/benchmark_random test/benchmark_random.cpp server/book.cpp
run-benchmark-random:
	bin/benchmark_random



# .PHONY: debug benchmark profile

# debug:
# 	g++ -o main main.cpp && ./main

# benchmark:
# 	g++ -Ofast -o main benchmark.cpp && ./main

# profile:
# 	g++ -pg benchmark.cpp -o main && ./main
# 	gprof main gmon.out > report.txt