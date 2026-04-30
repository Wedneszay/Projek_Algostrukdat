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
DataBuku *head = NULL;
DataBuku *tail = NULL;

// List fungsi
void loadFileBuku();
void pindahKeLinkedList(char isbn[], char judul[], char penulis[], int stok, double harga);
void admin();
void simpanKeFile();
void urutkanKatalogBuku();
void lihatKatalogBuku();
void tambahBuku();

int main()
{
    int opsiRole;
    bool pilihRole = false;

    loadFileBuku();

    cout << "PROGRAM MANAJEMEN TOKO BUKU" << endl;
    cout << "MENU ROLE" << endl; // Memilih role
    cout << "[1] Admin" << endl;
    cout << "[2] Kasir" << endl;
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
    if (fptr == NULL) {
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
    
    if (head == NULL) { // Jika data masih kosong
        head = tail = newNode;
    }
    else { // Jika data sudah terisi 
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}

void admin() {
    //system("cls");
    int opsiAdmin;
    bool pilihAdmin = false;
    cout << "| ==== MENU ADMIN ==== |" << endl;
    cout << "[1] Lihat Katalog" << endl;
    cout << "[2] Tambah Buku" << endl;
    cout << "[3] Hapus Buku" << endl;
    cout << "[4] Update Stok Buku" << endl;
    cout << "[5] Kembali ke Menu Utama" << endl;
    cout << "Pilih menu: "; cin >> opsiAdmin;

    while (!pilihAdmin) {
        switch (opsiAdmin) {
        case 1:
            pilihAdmin = true;
            lihatKatalogBuku();
            break;
        case 2:
            pilihAdmin = true;
            tambahBuku();
            break;
        case 5:
            pilihAdmin = true;
            main();
            break;
        default:
            cout << "Input tidak valid" << endl;
            break;
        }
    }
    
}

void simpanKeFile() {
    FILE *fptr;
    fptr = fopen("databuku.txt", "w");

    if (fptr == NULL) {
        cout << "[Error] Gagal menyimpan file";
        exit(1);
    }
    
    DataBuku *temp = head;

    while (temp != NULL) {
        fprintf(fptr, "%s;%s;%s;%d;%.2lf;\n", temp->isbn, temp->judul, temp->penulis, temp->stok, temp->harga);
        temp = temp->next;
    }
    fclose(fptr);
}

void urutkanKatalogBuku() {
    if (head == NULL || head->next == NULL) { // Mengecek apakah isi data 0 atau 1, jika iya tidak perlu sorting
        return;
    }

    bool swap;
    DataBuku *ptr1; // Pointer untuk mengecek setiap node
    DataBuku *lptr = NULL; // Pointer pembatas untuk ptr1 atau pembatas untuk data yg urut

    do {
        swap = false;
        ptr1 = head;
        
        while (ptr1->next != lptr) {
            if (stricmp(ptr1->judul, ptr1->next->judul) > 0) {
                char tempISBN[20], tempJudul[100], tempPenulis[50];
                int tempStok;
                double tempHarga;
                
                // Proses menukar judul melalui variabel sementara
                strcpy(tempISBN, ptr1->isbn);
                strcpy(tempJudul, ptr1->judul);
                strcpy(tempPenulis, ptr1->penulis);
                tempStok = ptr1->stok;
                tempHarga = ptr1->harga;
                
                strcpy(ptr1->isbn, ptr1->next->isbn);
                strcpy(ptr1->judul, ptr1->next->judul);
                strcpy(ptr1->penulis, ptr1->next->penulis);
                ptr1->stok = ptr1->next->stok;
                ptr1->harga = ptr1->next->harga;

                strcpy(ptr1->next->isbn, tempISBN);
                strcpy(ptr1->next->judul, tempJudul);
                strcpy(ptr1->next->penulis, tempPenulis);
                ptr1->next->stok = tempStok;
                ptr1->next->harga = tempHarga;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swap);
}

void lihatKatalogBuku() {
    //system("cls");
    urutkanKatalogBuku();
    bool kembali =  false;
    char backToMenu;

    DataBuku *temp = head;
    if (temp == NULL) {
        cout << "Data buku masih kosong, silakan isi melalui menu [2] Tambah Buku" << endl;
        admin(); // Kembali ke menu admin
    }

    cout << setfill('=') << setw(95) << "" << setfill(' ') << endl;
    printf("| %-10s | %-25s | %-20s | %-5s | %-19s |\n", "ISBN", "JUDUL", "PENULIS", "STOK", "HARGA");
    cout << setfill('=') << setw(95) << "" << setfill(' ') << endl;

    while (temp != NULL) {
        printf("| %-10s | %-25s | %-20s | %-5d | Rp%-17.2lf |\n", temp->isbn, temp->judul, temp->penulis, temp->stok, temp->harga);
        temp = temp->next;
    }
    cout << setfill('=') << setw(95) << "" << setfill(' ') << endl;
    
    while (!kembali) {
        cout << "Kembali ke Menu Admin (Y/N): "; cin >> backToMenu;
        if (toupper(backToMenu) == 'Y') {
            kembali = true;
            admin();
        }
        else if (toupper(backToMenu) == 'N') {
            kembali = true;
            return;
        }
        else {
            cout << "Input tidak valid, silakan ulang" << endl;
        }
    }
    
}

void tambahBuku() {
    //system("cls");
    DataBuku *newNode = (DataBuku *) malloc(sizeof(DataBuku));
    bool kembali = false;
    char backToMenu;

    char isbn[20], judul[100], penulis[50];
    int stok;
    double harga;

    printf("Masukkan ISBN    : ");
    scanf(" %[^\n]", newNode->isbn);
    printf("Masukkan Judul   : ");
    scanf(" %[^\n]", newNode->judul);
    printf("Masukkan Penulis : ");
    scanf(" %[^\n]", newNode->penulis);
    printf("Masukkan Stok    : ");
    scanf("%d", &newNode->stok);
    printf("Masukkan Harga   : ");
    scanf("%lf", &newNode->harga);

    newNode->next = NULL;
    newNode->prev = NULL;
    
    if (head == NULL) { // Jika data masih kosong
        head = tail = newNode;
    }
    else { // Jika data sudah terisi
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    simpanKeFile();

    while (!kembali) {
        cout << "Kembali ke Menu Admin (Y/N): "; cin >> backToMenu;
        if (toupper(backToMenu) == 'Y') {
            kembali = true;
            admin();
        }
        else if (toupper(backToMenu) == 'N') {
            kembali = true;
            return;
        }
        else {
            cout << "Input tidak valid, silakan ulang" << endl;
        }
    }
}
