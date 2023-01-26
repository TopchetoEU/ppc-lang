all: $(lsproj) $(lsinc)

$(lsproj): $(src)/lsproj.cc
	$(call mkdir,$(dir $@))
	$(CXX) $(flags) $^ -o $@
	
$(lsinc): $(src)/lsinc.cc
	$(call mkdir,$(dir $@))
	$(CXX) $(flags) $^ -o $@