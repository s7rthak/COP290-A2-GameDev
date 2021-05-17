.PHONY: compile_game server

compile_game:
	g++ maze_gen.cpp -o maze_gen
	g++ main.cpp game.cpp texture_manager.cpp map.cpp game_object.cpp screen.cpp screen_component.cpp two_player_map.cpp util.cpp map_layout_read.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lboost_system -lpthread
	
run_game:	
	./a.out

server:
	g++ server.cpp -o server -lboost_system -lpthread

run_server:
	./server

map: 
	./maze_gen