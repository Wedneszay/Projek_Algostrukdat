#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string>
using namespace std;

int main()
{
    int opsiRole;
    bool pilihRole = false;
    cout << "Selamat datang di Program Manajemen Toko Buku!" << endl;
    cout << "Silakan pilih role anda" << endl;
    cout << "1. Admin" << endl;
    cout << "2. Kasir" << endl;
    while (!pilihRole) {
        cout << ">> ";
        cin >> opsiRole;
        switch (opsiRole) {
        case 1:
            cout << "Anda adalah admin" << endl;
            pilihRole = true;
            break;
        case 2:
            cout << "Anda adalah kasir" << endl;
            pilihRole = true;
        default:
            cout << "Input tidak valid" << endl;
            break;
        }
    }
    return 0;
}
