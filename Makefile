simulate: 
	swift bundler run --platform iOSSimulator --simulator "iPhone 16"

tardis:
  swift bundler run --device "tardis"

mac:
  swift bundler run

clean:
	git clean -Xf

