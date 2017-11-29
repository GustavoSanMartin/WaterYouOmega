#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <climits>

using namespace std;

/**
 * Class for working with the Google sheets API
 */
class Network {
public:
    Network(){
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
    int fetch(bool& arg1, bool& arg2, int& arg3){
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
        int arg1Pos = startLocation+9;
        if (htmlLine.substr(arg1Pos, 1) == "1")
            arg1 = true;
        else if (htmlLine.substr(arg1Pos, 1) == "0")
            arg1 = false;
        else
            return 3;

        int arg2Pos = startLocation+13;
        if (htmlLine.substr(arg2Pos, 1) == "1")
            arg2 = true;
        else if (htmlLine.substr(arg2Pos, 1) == "0")
            arg2 = false;
        else
            return 3;

        int arg3Pos = startLocation+17;
        int arg3End = htmlLine.find("!END!"); //end is 1 character before !END!
        int arg3Length = arg3End-arg3Pos; //Length = end position - initial position
        arg3 = atoi(htmlLine.substr(arg3Pos, arg3Length).c_str());

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
    int push(const bool arg1, const bool arg2, const int arg3){
        //convert argument 1 to a string
        string sArg1 = "0";
        if (arg1)
            sArg1 = "1";

        //convert argument 2 to a string
        string sArg2 = "0";
        if (arg2)
            sArg2 = "1";

        //convert argument 3 to a string (int to string)
        string sArg3 = to_string(arg3);


        //GET parameters from Google scripts server
        // system command stores the returned html text in a text file.
        string command = "curl -k ";
        command += "\"https://script.google.com/macros/s/AKfycbwPE9mfnqfUhx8GCZrJ0J-AzaJAS2S08IFjy1R8NC93vvIXurk/exec?";
        command += "pWater=" + sArg1;
        command += "&pLamp=" + sArg2;
        command += "&pTimer=" + sArg3;
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
};

/**
 * Class for working with the Omega (reading / controlling ports)
 */
class Omega{
public:
    Omega(){
        //initialize the relay expansion
        system("relay-exp -i");

        //assign default values for the pins
        pump = false;
        lamp = false;
    }

    /**
     * control the physical state of the pump by opening and closing the relay switch
     * the relay switch is controlled through a system command which can only run on the Omega
     * @param pumpOn whether or not the pump is to be switched on (true: switch the pump on)
     */
    void setPump(bool pumpOn){
        //remember if the pump is on
        pump = pumpOn;

        if (pumpOn)
            system("relay-exp 0 1"); //turn relay switch 0 ('0') on ('1')
        else
            system("relay-exp 0 0"); //turn relay switch 0 ('0') off ('0')
    }

    /**
     * get the current state of the pump
     * @return whether or not the pump is currently turned on (if true, the pump is on)
     */
    bool getPump(){
        return pump;
    }

    /**
     * control the physical state of the lamp by opening and closing the relay switch
     * the relay switch is controlled through a system command which can only run on the Omega
     * @param lampOn whether or not the lamp is to be switched on (true: switch the lamp on)
     */
    void setLamp(bool lampOn){
        lamp = lampOn;
        if (lampOn)
            system("relay-exp 1 1");
        else
            system("relay-exp 1 0");
    }

    /**
     * get the current state of the lamp
     * @return whether or not the lamp is currently turned on (if true, the lamp is on)
     */
    bool getLamp(){
        return lamp;
    }

    ~Omega(){

    }
private:
    bool pump;
    bool lamp;
};

class Log{
public:
    Log(const string filename){
        Log::filename = filename;
    }
    void add(const string logText){
        ofstream LogFile;
        LogFile.open(filename);
        LogFile << logText << endl;
        LogFile.close();
    }
    void add(const int logNum){
        ofstream LogFile;
        LogFile.open(filename);
        string s = to_string(logNum);
        s += "\n";
        LogFile << s << endl;
        LogFile.close();
    }
    void add(const bool arg1, const bool arg2){
        ofstream LogFile;
        LogFile.open(filename);
        string sArg1 = "0";
        string sArg2 = "0";

        if (arg1)
            sArg1 = "1";
        if (arg2)
            sArg1 = "1";

        string logText = "Water: " + sArg1 + ", Auto: " + sArg2 + "\n";
        LogFile << logText << endl;
        LogFile.close();
    }

private:
    string filename;
};

int main() {
    bool Water = true, Lamp = false;
    bool TimerOn = false;
    int Timer = 0;
    int TimerCount = 1;

    Omega omega9E1A;
    Network network;

    unsigned long long currentTick = 0;
    unsigned long long tickAtPumpOn = ULLONG_MAX-2; //-2 to avoid overflow. high value meaning it has never been triggered
    unsigned long long tickAtTimerOn = 0;

    Log log("WaterYouLog.txt");
    log.add("booting");

    //loop until negative input
    while (Timer>=0){
        //check for updates (user input) from the server; store them in local booleans
        network.fetch(Water, Lamp, Timer);

        //if the timer is turned off run the manual controls
        if (Timer == 0) {
            TimerOn = false;

            //if the value obtained from the server for the pump is ON and the current state of the pump is OFF
            //(in other words, if the pump ON command was recently requested)
            if (Water && !omega9E1A.getPump())
                tickAtPumpOn = clock(); //check the time when the pump was turned on

            //if the pump has been on for 4(2sec/tick*ticks = 4 seconds) seconds
            if (currentTick > tickAtPumpOn+2) {
                Water = false;
                tickAtPumpOn = ULLONG_MAX-2;

                //notify server that pump is off
                network.push(Water, Lamp, Timer);
            }

            //set the state of the components
            omega9E1A.setPump(Water);
            omega9E1A.setLamp(Lamp);
        }

        //if the timer is on (has been on for some time)
        if (TimerOn){
            cout << "ticks until pump on: " << (tickAtTimerOn + (1800*Timer) * TimerCount) - currentTick << endl;
            //if the timer has been on for the set amount of time
            // (1800 ticks per hour) * (the number of times the timer has already been triggered)
            if (currentTick > tickAtTimerOn + (1800*Timer) * TimerCount) {
                TimerCount++;
                Water = true;
                tickAtPumpOn = currentTick;
                cout << "WATERING THE PLANT!!" << endl;
            }

            //if the pump has been on for 4 seconds, shut it off
            if (currentTick > tickAtPumpOn+2) {
                Water = false;
                tickAtPumpOn = ULLONG_MAX-2;
                cout << "DONE WATERING THE PLANT" << endl;
            }

            //set the value for the pump
            omega9E1A.setPump(Water);
        }
            //if the timer has just been turned on
        else if (Timer > 0){
            TimerOn = true;
            tickAtTimerOn = currentTick;
        }

        cout << endl << "Water: " << Water << " Auto: " << Lamp << " Timer: " << Timer << endl;
        log.add(Water, Lamp);

        //pause the program for 1 second
        sleep(1);
        currentTick++;
    }

    return 0;
}