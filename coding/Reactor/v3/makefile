ALL_CC:=$(wildcard *.cc)
# SRC_CC += $(filter-out testTicket.cc, $(ALL_CC))  # 忽略掉指定文件
CPP:=g++
FLAGS:=-g -w # -w 禁止所有的报警 -Wall 警告转成错误终止编译
LIBS:=-lpthread
ELF:=server.exe

all:
	$(CPP) $(ALL_CC) -o $(ELF) $(LIBS) $(FLAGS)

clean:
	rm $(ELF)
