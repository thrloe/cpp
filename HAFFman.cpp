#include <cstddef>
#include <iostream>
#include <vector>
#include <string>
using namespace std;


struct HAFFNODE{
    HAFFNODE* left;
    HAFFNODE* right;
    double freq;
    string chr;
};

int main(){

    vector<HAFFNODE*> list1;
    int n;
    cin >> n;
    cout << " готово: " << n << endl;
    for(int i = 0; i < n; i++){
        double freq;
        string chr;
        cout << "частота: " << endl;
        cin >> freq;
        cout << "ага: " << freq << endl; 
        cout << "буква: " << endl;
        cin >> chr;
        cout << "ага: " << chr << endl; 

        HAFFNODE* l = new HAFFNODE;
        l->chr = chr;
        l->freq = freq;
        list1.push_back(l); 
    }


    while (list1.size() > 1){
        double freq_lowest1 = 1;
        double freq_lowest2 = 1;
        HAFFNODE* low1;
        int low1_i = -1;
        bool low1_full = false;
        HAFFNODE* low2;
        int low2_i = -1;

        for(int i = 0; i < list1.size(); i++){
            if(list1[i]->freq < freq_lowest1){
                freq_lowest1 = list1[i]->freq;
                low1 = list1[i];
                low1_full = true;
                low1_i = i;
            }
            else{
                if(low1_full && list1[i]->freq < freq_lowest2){
                    freq_lowest2 = list1[i]->freq;
                    low2 = list1[i];
                    low2_i = i;
                }
            }
        }
        HAFFNODE* N = new HAFFNODE;
        N->left = low1;
        N->right = low2;
        N->chr = low1->chr + low2->chr;
        N->freq = low1->freq + low2->freq;
        list1.erase(list1.begin()+low1_i);
        list1.erase(list1.begin()+low2_i);
        list1.push_back(N);
    }

    string INPUT;

    cin >> INPUT;
    HAFFNODE H = *list1[0];
    for (int i = 0; i < INPUT.length(); i++){
        for (int j = 0; j < n; j++)
        int code1 = 0;
        if (H.left->chr.find(INPUT[i], 0) != string::npos){
            H = *H.left;
            code1 = code1 * 10 + 1;
        }
        if (H.right->chr.find(INPUT[i], 0) != string::npos){
            H = *H.right;
            code1 = code1 * 10 + 0;
        }
        cout << INPUT[i] << " - " << code1;
    }
    return 0;
}
