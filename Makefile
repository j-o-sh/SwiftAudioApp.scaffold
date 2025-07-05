debug: $(wildcard src/*.*)
	clang -g -DDEBUG -O0 -std=c11 -Wall -Wextra -fsanitize=address -Wno-pragma-once-outside-header -o debug src/main.c

booom: $(wildcard src/*.*)
	clang -std=c11 -Wall -Wextra -fsanitize=address -Wno-pragma-once-outside-header -o booom src/main.c

librecorder.a: $(wildcard src/*.*)
	clang -c -O2 -std=c11 -Wall -Wextra -Iinclude src/project.c -o project.o
	ar rcs librecorder.a project.o
	rm project.o

clean:
	git clean -Xf

