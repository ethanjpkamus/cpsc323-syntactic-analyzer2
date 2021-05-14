test:
	# clear
	clear && printf '\e[3J'
	clang++ -std=c++17 main.cpp -o main.o
# 	./main.o test1.txt
	./main.o test2.txt
# 	./main.o test3.txt

clean:
	rm *.o