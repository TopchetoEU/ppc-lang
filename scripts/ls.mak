all: $(lsdep) $(lsinc)

$(lsdep): $(src)/lsdep.cc
	$(call mkdir,$(dir $@))
	echo - Compiling lsdep.cc... >&2
	$(CXX) $(flags) $^ -o $@
	
$(lsinc): $(src)/lsinc.cc
	echo - Compiling lsinc.cc... >&2
	$(call mkdir,$(dir $@))
	$(CXX) $(flags) $^ -o $@