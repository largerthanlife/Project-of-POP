target = mysql
source = $(wildcard *.cpp)
objs = $(source:%.cpp=%.o)

$(target): $(objs)
	g++ $(objs) -std=c++11 -o $(target)
%.o:%.cpp
	g++ -c -o $@ $< -std=c++11

clean:
	rm -f *.o
