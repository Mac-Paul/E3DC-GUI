CXX=g++
ROOT_VALUE=watchdog
ROOT_VALUE2=RscpMain

all: $(ROOT_VALUE) $(ROOT_VALUE2)

$(ROOT_VALUE): clean
	$(CXX) -O1 Watchdog.cpp -o $@
$(ROOT_VALUE2): clean
	$(CXX) -O3 RscpMain.cpp RscpProtocol.cpp AES.cpp SocketConnection.cpp -o $@


clean:
	-rm $(ROOT_VALUE) $(ROOT_VALUE2) $(VECTOR) 
