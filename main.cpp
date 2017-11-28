#include <iostream>
#include <fstream>
using namespace std;

/**
 * Class for working with the Google sheets API
 */
class Network {
public:
    Network(){
        cout << "constructor" << endl;
    }
    void fetchRequests(bool& arg1, bool& arg2){
        system("curl -k \"https://script.google.com/macros/s/AKfycbwPE9mfnqfUhx8GCZrJ0J-AzaJAS2S08IFjy1R8NC93vvIXurk/exec?pWater=1&pAuto=1\" | tee test.txt");
        fstream test;
        test.open("test.txt");

    }
    void updateState(bool arg1, bool arg2){

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
    bool Water = false, Auto = false;
    Omega omega9E1A;
    Network network;

    network.fetchRequests(Water, Auto);

    ofstream logFile;
    logFile.open("WaterYouLog.csv");
    return 0;
}