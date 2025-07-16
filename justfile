set positional-arguments

default:
  swift bundler run


simulate: 
	swift bundler run --platform iOSSimulator --simulator "iPhone 16"

device device:
  swift bundler run --device "{{device}}"

clean:
	git clean -Xf

