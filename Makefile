

CC = i686-w64-mingw32-g++-win32

INCLUDEFLAGS = -I./dxsdk/Include/ -I./minihook/include/ -I./d3dhook/ -I./Include/   

LIBFLAGS =  -g -Wall -Wextra  -lpsapi -static -std=c++17 -lstdc++fs  -mtune=generic -march=x86-64  -m32 

CFLAGS = $(INCLUDEFLAGS) $(LIBFLAGS)

$(info CFLAGS=$(CFLAGS))

# Source files
EXE_SOURCE = injector.cpp
#DLL_SOURCE = $(wildcard ./d3dhook/*.cc ./minihook/src/*.cc maindll.cpp)
DLL_SOURCE = maindll.cpp


# Output files
EXE_NAME = BetterComboDisplay.exe
DLL_NAME = BetterComboDisplay.dll

# Targets
all: $(EXE_NAME) $(DLL_NAME)

$(EXE_NAME): $(EXE_SOURCE)
	$(CC) -o $@ $^ $(CFLAGS)
	echo done 

#$(DLL_NAME): $(DLL_SOURCE)
$(DLL_NAME): $(DLL_SOURCE)
	#which g++
	$(CC) -o $@ $^ $(CFLAGS)  -shared 

	@bash -c 'if [[ -n "$$MELTYPATH" ]]; then \
		cp -v BetterComboDisplay.dll "$$MELTYPATH"; \
	fi'
	

	
clean:
		rm $(DLL_NAME) $(EXE_NAME)
