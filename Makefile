
appname := rowCut

CC := gcc
CXX := g++

FLAGS := -O3 -lz 

all: $(appname)

$(appname): 
	$(CXX) $(appname).cpp $(FLAGS) -o $(appname)

