
SRC = $(wildcard Text/*.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))


Server : $(OBJ)
	g++ $(OBJ) -o Release/server.exe -L ../SDL/lib/x64 -lSDL2main -lSDL2 -lSDL2_ttf -lwsock32 -mwindows
	del Text\*.o
	cls

%.o : %.cpp
	g++ -c $< -I ../SDL/include -o $@

clean :
	del Release\*.exe
	del Text\*.o
	del Release\*.txt
	cls