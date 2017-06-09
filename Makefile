default: build

# Install git hooks once
.hooks:
	(cd .git && rm -riv hooks && ln -shiv ../hooks hooks || exit 0)
	@touch $@

# Install npm deps as needed
.deps: package.json
	npm install
	@touch $@

init: .hooks .deps

# Alias npm run ...
build: init; npm run build
watch: init; npm run watch
clean: init; npm run clean
serve: init; npm run serve
test:  init; npm run test

.PHONY: default init build hooks build watch clean serve test
