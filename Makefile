test-book:
	g++ -o bin/test_book test/test_book.cpp server/book.cpp && bin/test_book



# .PHONY: debug benchmark profile

# debug:
# 	g++ -o main main.cpp && ./main

# benchmark:
# 	g++ -Ofast -o main benchmark.cpp && ./main

# profile:
# 	g++ -pg benchmark.cpp -o main && ./main
# 	gprof main gmon.out > report.txt