$(lsproj): $(src)/lsproj.cc
	$(call mkdir,$(dir $@))
	$(CXX) $^ -o $@