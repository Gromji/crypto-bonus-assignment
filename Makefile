run:
	gcc -O3 -g gcd.c -o gcd.out -lgmp && ./gcd.out

clean:
	rm ./gcd.out && rm ./output.txt