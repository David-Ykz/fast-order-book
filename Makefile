benchmark-random:
	g++ -O3 -o bin/benchmark_random test/benchmark_random.cpp book/book.cpp && bin/benchmark_random
valgrind-benchmark-random:
	g++ -g -O0 -o bin/benchmark_random test/benchmark_random.cpp book/book.cpp && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./bin/benchmark_random

compile-benchmark-random:
	g++ -O3 -o bin/benchmark_random test/benchmark_random.cpp book/book.cpp
run-benchmark-random:
	bin/benchmark_random

compile-benchmark-latency:
	g++ -O3 -flto -march=native -o bin/benchmark_latency test/benchmark_latency.cpp book/book.cpp
valgrind-benchmark-latency: compile-benchmark-latency
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes bin/benchmark_latency
run-benchmark-latency:
	bin/benchmark_latency

compile-server:
	g++ -O3 -flto -march=native -o bin/server server/server.cpp server/worker.cpp server/broadcaster.cpp test/benchmark_latency.cpp book/book.cpp
run-server:
	bin/server
compile-client:
	g++ -O3 -flto -march=native -o bin/client client/listener.cpp
run-client:
	bin/client 8002


test-benchmark-random: compile-benchmark-random run-benchmark-random
test-benchmark-latency: compile-benchmark-latency run-benchmark-latency
server: compile-server run-server
client: compile-client run-client
