#include <iostream>
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

int main() {
  string IN;
  cout << "Enter input string: ";
  getline(cin, IN);
  vector<ZWNODE> OU = LZW_compressing(IN);

  cout << "Compressed output:" << endl;
  for (int i = 0; i < OU.size(); i++) {
    cout << OU[i].chr << " - " << OU[i].code << endl;
  }

  return 0;
}
