booom: $(wildcard src/*.*)
	clang -std=c11 -Wall -Wextra -fsanitize=address -Wno-pragma-once-outside-header -o booom src/main.c

