#include <iostream>

using namespace std;

int main() {
    int day, hour, minute;
    cin >> day >> hour >> minute;
    int nooow = (day - 1) * 24 * 60 + hour * 60 + minute;

    int N;
    cin >> N;

    int min_time = 1000000000;

    for (int i = 0; i < N; i++) {
        int d, h, m;
        cin >> d >> h >> m;
        int new_bud;

        if (d == 0) {
            for (int j = 0; j < 7; j++) {
                new_bud = j * 24 * 60 + h * 60 + m;
                if (new_bud < nooow) {
                    new_bud += 7 * 24 * 60;
                }
                if (new_bud < min_time) {
                    min_time = new_bud;
                }
            }
        } else {
            new_bud = (d - 1) * 24 * 60 + h * 60 + m;
            if (new_bud < nooow) {
                new_bud += 7 * 24 * 60;
            }
            if (new_bud < min_time) {
                min_time = new_bud;
            }
        }
    }

    int total = min_time;
    int result_d = (total / (24 * 60)) % 7 + 1;
    int result_h = (total % (24 * 60)) / 60;
    int result_m = total % 60;

    cout << result_d << " " << result_h << " " << result_m << endl;

    return 0;
}