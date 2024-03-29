export lsproj = $(bin)/lsproj$(exe)
export flags += "-I$(inc)" -D$(OS) -DPPC_VERSION_MAJOR=$(version-major) -DPPC_VERSION_MINOR=$(version-minor) -DPPC_VERSION_BUILD=$(version-build)

$(shell make -f scripts/lsproj.mak lsproj=$(lsproj) src=$(src) $(lsproj))

rwildcard=$(foreach d, $(wildcard $(1:=/*)),\
	$(call rwildcard,$d,$2)\
	$(filter $(subst *,%,$2),$d)\
)

uniq=$(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1)))
modoutput=$(shell ./$(lsproj) $(src) $1 output)
deps=$(strip \
	$(foreach dep, $(shell ./$(lsproj) $(src) $1 deps),\
		$(if $(wildcard src/$(dep)), $(dep),\
			$(error The module '$(dep)' (dependency of '$1') doesn't exist)\
		)\
	)\
)
rdeps=$(call uniq,$(strip \
	$(foreach dep, $(call deps,$1),\
		$(call rdeps,$(dep))\
		$(dep)\
	)\
))

fdeps=$(foreach dep,$(call deps,$1),$(bin)/lib$(lib)$(call modoutput,$(dep))$(so))
frdeps=$(foreach dep,$(call rdeps,$1),$(bin)/lib$(lib)$(call modoutput,$(dep))$(so))

ldeps=$(foreach dep,$(call deps,$1),-l$(lib)$(call modoutput,$(dep)))
lrdeps=$(foreach dep,$(call rdeps,$1),-l$(lib)$(call modoutput,$(dep)))

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

$(bin)/tmp/%.o: $(src)/%.cc $(headers)
	$(call mkdir,$(dir $@))
	$(CXX) -fPIC -c $(flags) $< -o $@
	echo - Compiling '$*.cc'...
