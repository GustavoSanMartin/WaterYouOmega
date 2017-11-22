#include <iostream>
#include <fstream>
#include "string"
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
        //accesses the online server and stores the html text in a text file.
        system("curl -k \"https://script.google.com/macros/s/AKfycbwPE9mfnqfUhx8GCZrJ0J-AzaJAS2S08IFjy1R8NC93vvIXurk/exec\" | tee test.txt");
        fstream html;
        html.open("test.txt");
        string Html;
        getline(html, Html);
        int startLocation = -1;
        while (startLocation == -1) {
            if (!getline(html, Html)){
                return;
            }
            startLocation = Html.find("!START!");
        }

        if (!Html.substr(startLocation+9, 1).compare("1")) {
            arg1 = true;
            cout << "Water is true" << endl;
        }
        else if (!Html.substr(startLocation+9, 1).compare("0"))
            arg1 = false;

        if (!Html.substr(startLocation+13, 1).compare("1"))
            arg2 = true;
        else if (!Html.substr(startLocation+13, 1).compare("0"))
            arg2 = false;

        return;
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
    bool Water = true, Auto = false;
    Omega omega9E1A;
    Network network;

    network.fetchRequests(Water, Auto);
    cout << "Water: " << Water << endl;
    cout << "Auto: " << Auto << endl;

    ofstream logFile;
    logFile.open("WaterYouLog.csv");
    return 0;
}