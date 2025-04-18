#include <atomic>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct ZWNODE {
  int code;
  string chr;
};

vector<ZWNODE> LZW_compressing(string INPUT) {
  vector<ZWNODE> dict;

  for (int j = 0; j < 256; j++) {
    ZWNODE node;
    node.chr = string(1, (char)j);
    node.code = j;
    dict.push_back(node);
  }
  int dictSize = 256;

  string current = "";
  vector<ZWNODE> OUT;

  for (int i = 0; i < INPUT.size(); i++) {
    char c = INPUT[i];
    string next = current + c;
    bool found = false;
    for (int j = 0; j < dict.size(); j++) {
      if (dict[j].chr == next) {
        current = next;
        found = true;
        break;
      }
    }

    if (!found) {

      for (int j = 0; j < dict.size(); j++) {
        if (dict[j].chr == current) {
          OUT.push_back(dict[j]);
          break;
        }
      }

      if (dictSize < 4096) {
        ZWNODE newNode;
        newNode.chr = next;
        newNode.code = dictSize;
        dict.push_back(newNode);
        dictSize++;
      }

      current = string(1, c);
    }
  }

  if (current != "") {
    for (int j = 0; j < dict.size(); j++) {
      if (dict[j].chr == current) {
        OUT.push_back(dict[j]);
        break;
      }
    }
  }

  return OUT;
}

string convering (int a){
string arm = "";
for (int l = 0; a != 0;l++){
int g = (a%2);
if (g=0){ arm = '0' + arm;}
if (g=1){ arm = '1' + arm;}
a = a-g;

}
}

int main() {
  string IN;
  cout << "Enter input string: ";
  getline(cin, IN);
  vector<ZWNODE> OU = LZW_compressing(IN);
  string now;
  for(int k = 0; k < OU.size(); k++){
    now += convering(OU[k].code);
  }
  for (int i; i < (now.length()/8)+1; i++){
    
  }

  return 0;
}
