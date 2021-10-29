ALL_CC:=$(wildcard *.cc)
# SRC_CC += $(filter-out testTicket.cc, $(ALL_CC))  # 忽略掉指定文件
CPP:=g++
FLAGS:=-g -Wall
LIBS:=-lpthread

all:
	$(CPP) $(ALL_CC) $(LIBS) $(FLAGS)

clean:
	rm a.out