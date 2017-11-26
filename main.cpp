#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <chrono>
#include <thread>

using namespace std;

/**
 * Class for working with the Google sheets API
 */
class Network {
public:
    Network(){
        cout << "constructor" << endl;
    }

    /**
     * Connect to a server and parse the parameters returned by the server
     * @param arg1 First argument to be passed in by the server
     * @param arg2 Second argument to be passed in by the server
     * @return  0 Success
     *          1 Error: Could not find file
     *          2 Error: Could not find "!START!" in server response
     *          3 Error: Server response is not a boolean
     */
    int fetch(bool& arg1, bool& arg2){
        //GET parameters from Google scripts server
        // system command stores the returned html text in a text file.
        system("curl -k \"https://script.google.com/macros/s/AKfycbwPE9mfnqfUhx8GCZrJ0J-AzaJAS2S08IFjy1R8NC93vvIXurk/exec\" | tee response.html");

        //open the text file with the stored
        fstream html;
        html.open("response.html");

        //parse the returned html text to extract the desired boolean values
        string htmlLine;
        int startLocation = -1; //the position of the keyword !START!

        while (startLocation == -1) {
            if (!getline(html, htmlLine)) //the next line of html text
                return 2;

            //check if "!START!" is in this line of html text. If not, go to the next line
            startLocation = htmlLine.find("!START!");
        }

        //convert string values to boolean values
        if (htmlLine.substr(startLocation+9, 1) == "1")
            arg1 = true;
        else if (htmlLine.substr(startLocation+9, 1) == "0")
            arg1 = false;
        else
            return 3;

        if (htmlLine.substr(startLocation+13, 1) == "1")
            arg2 = true;
        else if (htmlLine.substr(startLocation+13, 1) == "0")
            arg2 = false;
        else
            return 3;

        return 0;
    }

    /**
     * Change the values stored in the server
     * @param arg1 Replaces existing first parameter stored in the server
     * @param arg2 Replaces existing second parameter stored in the server
     * @return  0 Success
     *          1 Error: html file not found
     *          2 Error: server did not output (could not find "!START!")
     *          3 Error: server returned an error
     */
    int push(bool arg1, bool arg2){
        string sArg1 = "0";
        if (arg1)
            sArg1 = "1";

        string sArg2 = "0";
        if (arg2)
            sArg2 = "1";

        //GET parameters from Google scripts server
        // system command stores the returned html text in a text file.
        string command = "curl -k ";
        command += "\"https://script.google.com/macros/s/AKfycbwPE9mfnqfUhx8GCZrJ0J-AzaJAS2S08IFjy1R8NC93vvIXurk/exec?";
        command += "pWater=" + sArg1;
        command += "&pAuto=" + sArg2;
        command += "\" | tee response.html";
        cout << command << endl;
        const char* cCommand = command.c_str();
        system(cCommand);

        //open the text file with the stored
        fstream html;
        html.open("response.html");

        //parse the returned html text to extract the desired boolean values
        string htmlLine;
        int startLocation = -1; //the position of the keyword !START!

        while (startLocation == -1) {
            if (!getline(html, htmlLine)) //the next line of html text
                return 2;

            //check if "!START!" is in this line of html text. If not, go to the next line
            startLocation = htmlLine.find("!START!");
        }

        if (htmlLine.substr(startLocation+7, 7) == "SUCCESS")
            return 0;
        else
            return 3;
    }

    ~Network(){

    }

private:
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

    int getSoilHumidity(){
        return 0;
    }

    ~Omega(){

    }
private:

};

/*class Log{
public:
    Log(const string filename){
        LogFile.open(filename);
    }
    void add(const string logText){
        LogFile << logText << endl;
    }
    void add(const int logNum){
        string s = to_string(logNum);
        s += "\n";
        LogFile << logText << endl;
    }
    void add(const bool arg1, const bool arg2){
        string sArg1 = "0";
        string sArg2 = "0";

        if (arg1)
            sArg1 = "1";
        if (arg2)
            sArg1 = "1";

        string logText = "Water: " + sArg1 + ", Auto: " + sArg2 + "\n";
        LogFile << logText << endl;
    }
    ~Log(){
        LogFile.close;
    }

private:
    static ofstream LogFile;
};*/

int main() {
    bool Water = true, Auto = false;
    int soilHumidity = 0;
    int threshold = 75;
    Omega omega9E1A;
    Network network;
    unsigned long long currentTick = 0;
    unsigned long long tickAtPumpOn = 0;

    //Log log("WaterYouLog.txt");
    //log.add("booting");

    //loop infinitely. Step once every 2 seconds
    while (currentTick < 30){ //runs for 1 minute
        network.fetch(Water, Auto); //check for updates (user input) from the server

        cout << "Water: " << Water << endl << "Auto: " << Auto << endl;
        //log.add(Water, Auto);

        /*if (Auto){
            soilHumidity = omega9E1A.getSoilHumidity();
            //log.add(soilHumidity);

            //if the soil is less than some threshold (if it is too dry) AND the pump is not already on, turn on the pump
            if (soilHumidity < threshold && !omega9E1A.getPump()){
                omega9E1A.setPump(true);
                //log.add("pump on");
                Water = true;
            }

            //turn off the water pump after 2 ticks (6 seconds)
            if (Water && currentTick > tickAtPumpOn + 2){
                omega9E1A.setPump(false);
                Water = false;
            }

            //update the server with any changes made in the current iteration of the loop
            network.push(Water, Auto);
        }*/

        //pause the program for 2 seconds
        chrono::seconds duration(2);
        this_thread::sleep_for(duration);
        currentTick++;
    }

    return 0;
}