.PHONY: debug benchmark profile

debug:
	g++ -o main main.cpp && ./main

benchmark:
	g++ -Ofast -o main benchmark.cpp && ./main

profile:
	g++ -pg benchmark.cpp -o main && ./main
	gprof main gmon.out > report.txt