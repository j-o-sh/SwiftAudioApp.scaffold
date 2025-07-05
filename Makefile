debug: $(wildcard src/*.*)
	clang -g -DDEBUG -O0 -std=c11 -Wall -Wextra -fsanitize=address -Wno-pragma-once-outside-header -o debug src/main.c

booom: $(wildcard src/*.*)
	clang -std=c11 -Wall -Wextra -fsanitize=address -Wno-pragma-once-outside-header -o booom src/main.c

