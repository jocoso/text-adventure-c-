
EXEC = main

$(EXEC): $(EXEC).cpp
	g++ -o $(EXEC) $(EXEC).cpp

clean:
	rm -f main.exe
	rm -r .vs