export MAKEFLAGS += --silent -r -j
export flags=-std=c++17 -Wall -Wno-main -Wno-trigraphs -Wno-missing-braces -Wno-stringop-overflow -DPROFILE_$(profile) -fdiagnostics-color=always
export ldflags=-L$(bin)/$(profile)
export lib=ppc$(version-major)-
export profile=release

############# SETTINGS ############
export bin = bin
export src = src
export inc = include
export output = ppc
export mainmodule = main
export version-major=0
export version-minor=0
###################################

ifeq ($(OS),Windows_NT)
export os = Windows
export OS = WINDOWS
else
export os = $(shell uname)
export OS = $(shell echo '$(os)' | tr '[:lower:]' '[:upper:]')
endif

.PHONY: build debug leak lint clear install uninstall version

ifeq ($(profile),release)
flags += -O3
else ifeq ($(profile),debug)
flags += -g
ldflags+= -Wl,-rpath=bin/debug
endif
oldbin := bin
export bin := $(bin)/$(profile)

ifeq ($(os),Windows)

export mkdir=if not exist "$$1" mkdir "$$1"
export rmdir=if exist "$$1" rmdir /s /q "$$1"
export echo=echo "$$1"
export so=.dll
export exe=.exe
export CC=gcc
export CXX=g++

export version-build=$(if $(wildcard build.version),$(shell type build.version),0)
export binary = $(bin)/$(output)$(version-major)-windows.exe

build: version
	echo ======================== Compiling =========================
	make -f scripts/common.mak
	if exist "$(subst /,\,$(bin)\$(output).exe)" del "$(subst /,\,$(bin)\$(output).exe)"
	mklink /H "$(subst /,\,$(bin)\$(output).exe)" "$(subst /,\,$(binary))" > NUL
	echo Done!
clear:
	if exist $(subst /,\,$(oldbin)) rmdir /s /q $(subst /,\,$(oldbin))

.ONESHELL:
install: build
	powershell -Command "start-process cmd -verb runas -args '/K pushd %CD%&set bin=$(bin)&set output=$(output)&.\scripts\install.bat&exit'"	
#	.\scripts\install.bat
uninstall:
	.\scripts\uninstall.bat
version:
	cmd /c "set /a $(version-build) + 1 > build.version"

else

ldflags += -lpthread

export mkdir=mkdir -p $$1
export rmdir=rm -rf $$1
export echo=echo "$$1"
export so=.so
export version-build=$(if $(wildcard build.version),$(shell cat build.version),0)
export binary = $(bin)/$(output)$(version-major)-linux

build: version
	echo ======================== Compiling =========================
	make -f scripts/common.mak
	ln -f $(binary) $(bin)/$(output)

clear:
	rm -r $(oldbin)

install: build
	echo Installing ++C compiler to your system...
	sudo cp $(bin)/*.so /usr/lib
	sudo cp $(binary) /usr/bin/$(output)
	sudo chmod +777 /usr/bin/$(output)
	sudo chmod +777 $(patsubst $(bin)/%,/usr/lib/%,$(wildcard $(bin)/*.so))
uninstall:
	echo Uninstalling ++C compiler from your system...
	sudo rm $(patsubst $(bin)/%.so,/usr/lib/%*.so,$(bin)/*.so)
	sudo rm /usr/bin/$(output)
version:
	echo $$(($(version-build) + 1)) > build.version

leak: build
	echo ====================== Leak scanning =======================
	$(if $(filter $(os),Windows),cmd /C echo.,echo)
	valgrind --tool=memcheck --leak-check=full --track-origins=yes ./$(binary) $(patsubst %,./%,$(wildcard *.ppc))
lint:
	echo ========================= Linting ==========================
	cppcheck $(src) --track-origins=yes --suppress=unusedFunction --suppress=missingInclude --enable=all
endif
