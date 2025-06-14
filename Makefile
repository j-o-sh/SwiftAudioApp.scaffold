booom: $(wildcard src/*.*)
	clang -std=c11 -Wall -Wno-pragma-once-outside-header -o booom src/main.c

