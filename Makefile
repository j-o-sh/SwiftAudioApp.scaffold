debuglib: $(wildcard libaudio/src/*.*)
	clang -g -DDEBUG -O0 -std=c11 -Wall -Wextra -fsanitize=address -Wno-pragma-once-outside-header -o debuglib libaudio/src/main.c

booom: $(wildcard libaudio/src/*.*)
	clang -std=c11 -Wall -Wextra -fsanitize=address -Wno-pragma-once-outside-header -o booom libaudio/src/main.c

libaudio/libaudio.a: $(wildcard libaudio/src/*.*)
	clang -c -O2 -arch arm64 -std=c11 -Wall -Wextra -Wno-pragma-once-outside-header libaudio/src/project.c -o project.o
	ar rcs libaudio/libaudio.a project.o
	rm project.o

run: libaudio/libaudio.a
	cd appaudio && \
	swift bundler run --platform iOSSimulator --simulator "iPhone 16"

clean:
	git clean -Xf

