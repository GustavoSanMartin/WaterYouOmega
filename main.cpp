#include <iostream>
#include <fstream>
#include <cstring>
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

    cout << "return: " << network.push(Water, Auto) << endl;
    cout << "Water: " << Water << endl;
    cout << "Auto: " << Auto << endl;

    ofstream logFile;
    logFile.open("WaterYouLog.csv");
    return 0;
}