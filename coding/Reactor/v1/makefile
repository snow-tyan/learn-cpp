ALL_CC:=$(wildcard *.cc)
# SRC_CC += $(filter-out testTicket.cc, $(ALL_CC))  # 忽略掉指定文件
CPP:=g++
FLAGS:=-g -Wall
LIBS:=-lpthread
ELF:=server

all:
	$(CPP) $(ALL_CC) -o $(ELF) $(LIBS) $(FLAGS)

clean:
	rm $(ELF)
