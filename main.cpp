#include <iostream>
#include <fstream>
using namespace std;

/**
 * Class for working with the Google sheets API
 */
class Network {
public:
    Network(){

    }
    bool fetchRequests(){

    }
    void updateState(){

    }
    ~Network(){

    }

private:
    const char* sheetID = "1H062SSqLF8JN7vaRyhBx_h7om0sxhKRB9g6aEHo5Pyw";
};

/**
 * Class for working with the Omega (reading / controlling ports)
 */
class Omega{
public:
    Omega(){

    }

    void setPump(bool pumpOn){

    }

    bool getPump(){

    }

    void setAuto(bool autoOn){

    }

    bool getAuto(){

    }

    int getSoilHumidity(){

    }

    ~Omega(){

    }
private:
    const int relaySwitch = 1;
    const int sensorPort = 7;
};
int main() {
    Omega omega9E1A;
    Network network;

    ofstream logFile;
    logFile.open("WaterYouLog.csv");
    return 0;
}