	#MAKEFILE
	GG=g++ -Wall -pedantic -g
	OBJ=main.o Game.o IOManager.o FileManager.o Level.o Movable.o Map.o Player.o Ghost.o Blinky.o Inky.o Clyde.o Speedy.o MapPart.o Item.o Teleport.o solcmich
	DOC=doc

all: compile run doc

run: 
	./solcmich

compile: main.o Game.o IOManager.o FileManager.o Level.o Movable.o Map.o Player.o Ghost.o Blinky.o Inky.o Clyde.o Speedy.o MapPart.o Item.o Teleport.o
	g++ -g -o solcmich main.o Game.o IOManager.o FileManager.o Level.o Movable.o Map.o Player.o Ghost.o Blinky.o Inky.o Clyde.o Speedy.o MapPart.o Item.o Teleport.o -lncurses

clean: 
	rm -r $(OBJ)
	rm -rf $(DOC)

doc: src/main.cpp src/Gameplay/Game.h src/Gameplay/GameBehaviour.h src/Gameplay/Level.h src/Ghost/Blinky.h src/Ghost/Clyde.h src/Ghost/Ghost.h src/Ghost/Inky.h src/Ghost/Speedy.h src/Helpers/FileManager.h src/Helpers/IOManager.h src/Map/Item.h \
src/Map/Map.h src/Map/MapPart.h src/Map/Movable.h src/Map/Teleport.h src/Player/Player.h
	doxygen Doxyfile

main.o: src/main.cpp src/Gameplay/Game.h
	$(GG) -c src/main.cpp

Game.o: src/Gameplay/Game.cpp src/Gameplay/Game.h src/Gameplay/GameBehaviour.h src/Gameplay/Settings.h src/Gameplay/Level.h src/Helpers/IOManager.h src/Ghost/Ghost.h src/Helpers/FileManager.h
	$(GG) -c src/Gameplay/Game.cpp 

IOManager.o: src/Helpers/IOManager.cpp src/Helpers/IOManager.h src/Map/Movable.h src/Gameplay/Level.h src/Player/Player.h
	$(GG) -c src/Helpers/IOManager.cpp 
    
FileManager.o: src/Helpers/FileManager.cpp src/Helpers/FileManager.h 
	$(GG) -c src/Helpers/FileManager.cpp
    
Constants.o: src/Gameplay/Constants.h 
	$(GG) -c src/Gameplay/Constants.h 
	
GameBehaviour.o: src/Gameplay/GameBehaviour.h 
	$(GG) -c src/Gameplay/GameBehaviour.h 
    
Level.o: src/Gameplay/Level.cpp src/Gameplay/Level.cpp src/Map/Map.h
	$(GG) -c src/Gameplay/Level.cpp
    
Settings.o: src/Gameplay/Level.h
	$(GG) -c src/Gameplay/Level.h
    
Movable.o: src/Map/Movable.cpp src/Map/Movable.h src/Map/Map.h src/Gameplay/GameBehaviour.h
	$(GG) -c src/Map/Movable.cpp
    
Map.o: src/Map/Map.cpp src/Map/Map.h src/Map/Item.h src/Gameplay/GameBehaviour.h src/Gameplay/Game.h src/Map/Teleport.h src/Gameplay/Settings.h src/Gameplay/Constants.h src/Player/Player.h src/Ghost/Inky.h src/Ghost/Speedy.h src/Ghost/Blinky.h 	src/Ghost/Clyde.h
	$(GG) -c src/Map/Map.cpp
    
Player.o: src/Player/Player.cpp src/Player/Player.h src/Helpers/IOManager.h src/Gameplay/GameBehaviour.h src/Map/MapPart.h src/Map/Movable.h src/Gameplay/Game.h src/Ghost/Ghost.h src/Gameplay/Level.h
	$(GG) -c src/Player/Player.cpp
    
Ghost.o: src/Ghost/Ghost.cpp src/Ghost/Ghost.h src/Map/MapPart.h src/Gameplay/GameBehaviour.h src/Map/Movable.h src/Gameplay/Game.h src/Gameplay/Settings.h src/Gameplay/Constants.h src/Player/Player.h 
	$(GG) -c src/Ghost/Ghost.cpp
    
Blinky.o: src/Ghost/Blinky.cpp src/Ghost/Blinky.h src/Ghost/Ghost.h 
	$(GG) -c src/Ghost/Blinky.cpp
    
Inky.o: src/Ghost/Inky.cpp src/Ghost/Inky.h src/Ghost/Ghost.h 
	$(GG) -c src/Ghost/Inky.cpp
    
Clyde.o: src/Ghost/Clyde.cpp src/Ghost/Clyde.h src/Ghost/Ghost.h 
	$(GG) -c src/Ghost/Clyde.cpp
    
Speedy.o: src/Ghost/Speedy.cpp src/Ghost/Speedy.h src/Ghost/Ghost.h 
	$(GG) -c src/Ghost/Speedy.cpp
    
MapPart.o: src/Map/MapPart.cpp src/Map/MapPart.h src/Gameplay/Constants.h
	$(GG) -c src/Map/MapPart.cpp

Item.o: src/Map/Item.cpp src/Map/Item.h src/Map/MapPart.h src/Gameplay/Game.h src/Gameplay/Level.h src/Gameplay/Constants.h
	$(GG) -c src/Map/Item.cpp
    
Teleport.o: src/Map/Teleport.cpp src/Map/Teleport.h src/Map/MapPart.h src/Gameplay/Constants.h src/Player/Player.h
	$(GG) -c src/Map/Teleport.cpp
