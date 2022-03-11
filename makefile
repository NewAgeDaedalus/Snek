flags = -Wall
libs = -lsfml-system -lsfml-window -lGL -lGLEW

snake: main.o 
	g++ main.o  render.cpp snek.cpp -o snake $(libs) 

main.o: main.cpp render.cpp snek.cpp 


clean:
	@rm -f *.o snake