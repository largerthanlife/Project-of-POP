target = mycalculator
source = $(wildcard *.cpp)
objs = $(source:%.cpp=%.o)

$(target): $(objs)
	g++ $(objs) -std=c++11 -lreadline -o  $(target)	
%.o:%.cpp
	g++ -c -o $@ $< -std=c++11 -Werror 
clean:
	rm -f *.o .history
