#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <cstring>
using namespace std;

// Struct buku
typedef struct Buku {
    char isbn[20];
    char judul[100];
    char penulis[50];
    int stok;
    double harga;

    // Pointer untuk double linked list
    struct Buku *next; 
    struct Buku *prev;
} DataBuku;
DataBuku *head = nullptr;
DataBuku *tail = nullptr;

// List fungsi
void admin();
void lihatKatalogBuku();
void loadFileBuku();
void pindahKeLinkedList(char isbn[], char judul[], char penulis[], int stok, double harga);

int main()
{
    int opsiRole;
    bool pilihRole = false;

    loadFileBuku();

    cout << "PROGRAM MANAJEMEN TOKO BUKU" << endl;
    cout << "MENU ROLE" << endl; // Memilih role
    cout << "1. Admin" << endl;
    cout << "2. Kasir" << endl;
    while (!pilihRole) { // Perulangan jika input tidak valid
        cout << "Silahkan pilih role anda: "; cin >> opsiRole;
        switch (opsiRole) {
        case 1:
            cout << "Anda adalah admin" << endl;
            pilihRole = true;
            admin();
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

void loadFileBuku() {
    FILE *fptr;
    fptr = fopen ("databuku.txt", "r");
    if (fptr == nullptr) {
        cout << "Database tidak ditemukan, memulai dengan katalog kosong" << endl;
        return;
    }

    char isbn[20], judul[100], penulis[50];
    int stok;
    double harga;

    while (fscanf(fptr, "%[^;];%[^;];%[^;];%d;%lf\n", isbn, judul, penulis, &stok, &harga)) {
        pindahKeLinkedList(isbn, judul, penulis, stok, harga);
    }
    
    fclose(fptr);

}

void pindahKeLinkedList(char isbn[], char judul[], char penulis[], int stok, double harga) {
    DataBuku *newNode =  (DataBuku *) malloc(sizeof(DataBuku));

    strcpy(newNode->isbn, isbn);
    strcpy(newNode->judul, judul);
    strcpy(newNode->penulis, penulis);
    newNode->stok = stok;
    newNode->harga = harga;

    newNode->next = NULL;
    newNode->prev = NULL;
    
    if (head == NULL) {
        head = tail = newNode;
    }
    else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}

void admin() {
    //system("cls");
    int opsiAdmin;
    bool pilihAdmin = false;
    cout << "MENU ADMIN" << endl;
    cout << "1. Lihat Katalog" << endl;
    cout << "2. Tambah Buku" << endl;
    cout << "3. Hapus Buku" << endl;
    cout << "4. Update Stok Buku" << endl;
    cout << "Pilih menu: "; cin >> opsiAdmin;

    while (!pilihAdmin) {
        switch (opsiAdmin) {
        case 1:
            pilihAdmin = true;
            
            break;
        default:
            break;
        }
    }
    
}

void lihatKatalogBuku() {
    DataBuku *temp = head;

}


