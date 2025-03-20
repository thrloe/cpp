#include <string>
#include <vector>
#include <iostream>

using namespace std;

int main() {
    string arm_INPUT;
    getline(cin, arm_INPUT);

    int num;

        for(int i = 0; i < arm_INPUT.size(); i++) {
            if(
                arm_INPUT[i] != '1' 
                && arm_INPUT[i] != '2' 
                && arm_INPUT[i] != '3' 
                && arm_INPUT[i] != '4' 
                && arm_INPUT[i] != '5' 
                && arm_INPUT[i] != '6' 
                && arm_INPUT[i] != '7' 
                && arm_INPUT[i] != '8' 
                && arm_INPUT[i] != '9' 
                && arm_INPUT[i] != '0'
            ) {
                cout << "nah" << endl << arm_INPUT[i] << endl;
                return 0;
            }
            switch(arm_INPUT[i]) {
                case '1': {
                    num = num + 1;
                    break;
                }
                case '2': {
                    num = num + 2;
                    break;
                }
                case '3': {
                    num = num + 3;
                    break;
                }
                case '4': {
                    num = num + 4;
                    break;
                }
                case '5': {
                    num = num + 5;
                    break;
                }
                case '6': {
                    num = num + 6;
                    break;
                }
                case '7': {
                    num = num + 7;
                    break;
                }
                case '8': {
                    num = num + 8;
                    break;
                }
                case '9': {
                    num = num + 9;
                    break;
                }
                case '0': {
                    num = num + 0;
                    break;
                }
            }
        }
        if (num % 3 == 0){
            cout << "yay" << endl;
            return 0;
        }
        else {
            cout << "nah" << endl;
            return 0;
        }
    return 0;
}
