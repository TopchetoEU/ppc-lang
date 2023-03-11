export lsdep = $(bin)/lsdep$(exe)
export lsinc = $(bin)/lsinc$(exe)
export flags += "-I$(inc)" -D$(OS) -DPPC_VERSION_MAJOR=$(version-major) -DPPC_VERSION_MINOR=$(version-minor) -DPPC_VERSION_BUILD=$(version-build)

$(shell make -f scripts/ls.mak lsinc=$(lsinc) lsdep=$(lsdep) src=$(src) "flags=$(flags)" all)

rwildcard=$(foreach d, $(wildcard $(1:=/*)),\
	$(call rwildcard,$d,$2)\
	$(filter $(subst *,%,$2),$d)\
)

deps=$(shell $(lsdep) --dir=deps $1)
rdeps=$(shell $(lsdep) --dir=deps --rec $1)

frdeps=$(shell $(lsdep) --dir=deps --rec --transform=$(bin)/lib$(lib)*$(so) $1)
ldeps=$(shell $(lsdep) --dir=deps --transform=-l$(lib) $1)

modules = $(patsubst $(src)/%/,$(bin)/lib$(lib)%$(so),$(filter-out $(src)/$(mainmodule)/,$(wildcard $(src)/*/)))
sources = $(call rwildcard,$(src)/$1,*.cc)
headers = $(call rwildcard,$(inc),*.hh)
binaries = $(patsubst $(src)/%.cc,$(bin)/tmp/%.o,$(call sources,$1))

.PHONY: build
.PRECIOUS: $(bin)/tmp/%.o

build: $(binary)

.SECONDEXPANSION:
$(binary): $$(call frdeps,$(mainmodule)) $$(call binaries,$(mainmodule))
	$(call mkdir,$(dir $@))
	$(CXX) $(flags) $(call binaries,$(mainmodule)) -o $@ $(ldflags) $(call ldeps,$(mainmodule)) -L$(bin) "-I$(inc)"
	echo Compiling executable '$(notdir $(binary))'...

.SECONDEXPANSION:
$(bin)/lib$(lib)%$(so): $$(call frdeps,$$*) $$(call binaries,$$*)
	$(call mkdir,$(bin))
	$(CXX) -shared -fPIC $(flags) $(call binaries,$*) -o $@ $(ldflags) $(call ldeps,$*) -L$(bin) "-I$(inc)"
	echo Compiling library '$(notdir $@)'...

.SECONDEXPANSION:
$(bin)/tmp/%.o: $(src)/%.cc $$(shell $(lsinc) $(inc) $(src)/%.cc)
	$(call mkdir,$(dir $@))
	$(CXX) -fPIC -c $(flags) $< -o $@
	echo - Compiling '$*.cc'...
