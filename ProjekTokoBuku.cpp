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
DataBuku *keranjangHead = NULL;
DataBuku *keranjangTail = NULL;

// List fungsi
void loadFileBuku();
void pindahKeLinkedList(char isbn[], char judul[], char penulis[], int stok, double harga);
void admin();
void simpanKeFile();
void urutkanKatalogBuku();
void lihatKatalogBuku();
void tambahBuku();
void hapusBuku();
void updateStokBuku();
void kasir();
void searchBuku();
void tambahKeranjang(DataBuku *katalogBuku);
void lihatKeranjang();

int main()
{
    int opsiRole;
    bool pilihRole = true;

    loadFileBuku();

    while (pilihRole) { // Perulangan jika input tidak valid
        cout << "PROGRAM MANAJEMEN TOKO BUKU" << endl;
        cout << "MENU ROLE" << endl; // Memilih role
        cout << "[1] Admin" << endl;
        cout << "[2] Kasir" << endl;
        cout << "[0] Keluar" << endl;
        cout << "Silahkan pilih role anda: "; cin >> opsiRole;
        switch (opsiRole) {
            case 1:
                cout << "Anda adalah admin" << endl;
                admin();
                break;
            case 2:
                cout << "Anda adalah kasir" << endl;
                kasir();
                break;
            case 0: 
                // system("cls");
                pilihRole = false;
                cout << "[MESSAGE] Anda telah keluar. Terima kasih!" << endl;
                break;
            default:
                //system("cls");
                cout << "[ERROR] Input tidak valid" << endl;
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

    while (fscanf(fptr, "%[^;];%[^;];%[^;];%d;%lf;\n", isbn, judul, penulis, &stok, &harga) == 5) {
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
    int opsiAdmin;
    bool pilihAdmin = true;
    while (pilihAdmin) {
        cout << "\n| ==== MENU ADMIN ==== |" << endl;
        cout << "[1] Lihat Katalog" << endl;
        cout << "[2] Tambah Buku" << endl;
        cout << "[3] Hapus Buku" << endl;
        cout << "[4] Update Stok Buku" << endl;
        cout << "[5] Kembali ke Menu Utama" << endl;
        cout << "Pilih menu: ";
        cin >> opsiAdmin;

        switch (opsiAdmin) {
        case 1:
            lihatKatalogBuku();
            break;
        case 2:
            tambahBuku();
            break;
        case 3:
            hapusBuku();
            break;
        case 4:
            updateStokBuku();
            break;
        case 5:
            pilihAdmin = false; 
            break;
        default:
            cout << "Input tidak valid\n";
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

                swap = true;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swap);
}

void lihatKatalogBuku() {
    //system("cls");
    urutkanKatalogBuku();
    bool kembali =  true;
    char backToMenu;

    DataBuku *temp;
    temp = head;

    if (temp == NULL) {
        cout << "Data buku masih kosong, silakan isi melalui Menu Admin [2] Tambah Buku" << endl;
        admin(); // Kembali ke menu admin
    }

    cout << setfill(' ') << setw(37) << " ";
    cout << "K A T A L O G  B U K U";
    cout << setfill(' ') << setw(37) << " " << endl;
    cout << setfill('=') << setw(96) << "" << setfill(' ') << endl;
    printf("| %-10s | %-26s | %-20s | %-5s | %-19s |\n", "ISBN", "JUDUL", "PENULIS", "STOK", "HARGA");
    cout << setfill('=') << setw(96) << "" << setfill(' ') << endl;

    while (temp != NULL) {
        printf("| %-10s | %-26s | %-20s | %-5d | Rp%-17.2lf |\n", temp->isbn, temp->judul, temp->penulis, temp->stok, temp->harga);
        temp = temp->next;
    }
    cout << setfill('=') << setw(96) << "" << setfill(' ') << endl;
    
    while (kembali) {
        cout << "Kembali ke Menu Sebelumnya (Y/N): "; cin >> backToMenu;
        if (toupper(backToMenu) == 'Y') {
            return;
        }
        else if (toupper(backToMenu) == 'N') {
            // system("cls");
            cout << "Terima kasih!" << endl;
            // system("pause");
            exit(1);
        }
        else {
            cout << "Input tidak valid, silakan ulang" << endl;
        }
    }
    
}

void tambahBuku() {
    //system("cls");
    DataBuku *newNode = (DataBuku *) malloc(sizeof(DataBuku));
    bool kembali = true;
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
    cout << "Berhasil menambahkan buku!" << endl;

    while (kembali) {
        cout << "Kembali ke Menu Admin (Y/N): "; cin >> backToMenu;
        if (toupper(backToMenu) == 'Y') {
            admin();
        }
        else if (toupper(backToMenu) == 'N') {
            system("cls");
            cout << "Terima kasih!" << endl;
            system("pause");
            exit(1);
        }
        else {
            cout << "Input tidak valid, silakan ulang" << endl;
        }
    }
}

void hapusBuku() {
    DataBuku *temp = head;
    char isbn[20];
    
    // Kalau data kosong
    if (head == NULL) {
        cout << "Data buku masih kosong, silakan isi melalui Menu Admin [2] Tambah Buku" << endl;
        admin(); // Kembali ke menu admin
    }

    cout << "Masukkan ISBN buku yang ingin dihapus: ";
    cin >> isbn;

    while (temp != NULL) {
        if (strcmp(temp->isbn, isbn) == 0) {

            // 1. Kalau cuma 1 data
            if (temp == head && temp == tail) {
                head = tail = NULL;
            }
            // 2. Kalau di awal
            else if (temp == head) {
                head = head->next;
                head->prev = NULL;
            }
            // 3. Kalau di akhir
            else if (temp == tail) {
                tail = tail->prev;
                tail->next = NULL;
            }
            // 4. Kalau di tengah
            else {
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
            }

            free(temp);
            simpanKeFile();

            cout << "Buku berhasil dihapus!\n";
            admin();
            return;
        }
        temp = temp->next;
    }

    cout << "Buku tidak ditemukan!\n";
    admin();
}

void updateStokBuku() {
    DataBuku *temp = head;
    char isbn[20];

    if (head == NULL) {
        cout << "Data buku masih kosong, silakan isi melalui Menu Admin [2] Tambah Buku" << endl;
        admin(); // Kembali ke menu admin
    }

    cout << "Masukkan ISBN buku: ";
    cin >> isbn;

    while (temp != NULL) {
        if (strcmp(temp->isbn, isbn) == 0) {
            cout << "Masukkan stok baru: ";
            cin >> temp->stok;

            simpanKeFile();

            cout << "Stok berhasil diupdate!\n";
            admin();
            return;
        }
        temp = temp->next;
    }

    cout << "Buku tidak ditemukan!\n";
    admin();
}

void kasir() {
    int opsiKasir;
    bool pilihKasir = true;
    while (pilihKasir) {
        cout << "\n| ==== MENU KASIR ==== |" << endl;
        cout << "[1] Lihat Katalog" << endl;
        cout << "[2] Search Buku" << endl;
        cout << "[3] Lihat Keranjang" << endl;
        cout << "[4] Hapus Item di Keranjang" << endl;
        cout << "[5] Pembayaran" << endl;
        cout << "[6] Kembali ke Menu Utama" << endl;
        cout << "Pilih menu: ";
        cin >> opsiKasir;

        switch (opsiKasir) {
            case 1: 
                lihatKatalogBuku();
                break;
            case 2:
                searchBuku();
                break;
            case 3:
                lihatKeranjang();
                break;
            case 6:
                pilihKasir = false; 
                break;
            default:
                cout << "Input tidak valid\n";
        }
    }
}

void searchBuku() {
    DataBuku *temp;
    char cariISBN[20];
    temp = head;
    bool found = false;
    bool kembali = true;
    char backToMenu, tambah;
    
    if (head == NULL) {
        cout << "Data buku masih kosong, silakan isi melalui Menu Admin" << endl;
        admin(); // Kembali ke menu admin
    }

    printf("Masukkan ISBN yang ingin dicari: "); scanf("%s", cariISBN);

    while(temp != NULL && !found) {
        if (strcmp(temp->isbn, cariISBN) == 0) {
            found = true;
            break;
        }
        temp = temp->next;
    }

    if (found) {
        cout << "K A T A L O G  B U K U";
        cout << setfill(' ') << setw(37) << " " << endl;
        cout << setfill('=') << setw(96) << "" << setfill(' ') << endl;
        printf("| %-10s | %-26s | %-20s | %-5s | %-19s |\n", "ISBN", "JUDUL", "PENULIS", "STOK", "HARGA");
        cout << setfill('=') << setw(96) << "" << setfill(' ') << endl;
        printf("| %-10s | %-26s | %-20s | %-5d | Rp%-17.2lf |\n", temp->isbn, temp->judul, temp->penulis, temp->stok, temp->harga);
        cout << setfill('=') << setw(96) << "" << setfill(' ') << endl;

        cout << "Tambahkan buku ke keranjang (Y/N): "; cin >> tambah;
        if (toupper(tambah) == 'Y') {
            if (temp->stok > 0) {
                tambahKeranjang(temp);
            }
            else {
                cout << "Stok habis!" << endl;
            }
        }
        else if (toupper(tambah) == 'N') {
            while (kembali) {
                cout << "Kembali ke menu Kasir (Y/N): "; cin >> backToMenu;
                if (toupper(backToMenu) == 'Y') {
                    return;
                }
                else if (toupper(backToMenu) == 'N') {
                    //system("cls");
                    cout << "Terima kasih!" << endl;
                    //system("pause");
                    exit(1);
                }
            }
        }
    }
    else {
        cout << "Buku tidak ditemukan!" << endl;
    }

    while (kembali) {
        cout << "Kembali ke menu Kasir (Y/N): "; cin >> backToMenu;
        if (toupper(backToMenu) == 'Y') {
            return;
        }
        else if (toupper(backToMenu) == 'N') {
            //system("cls");
            cout << "Terima kasih!" << endl;
            //system("pause");
            exit(1);
        }
    }

    
}

void tambahKeranjang(DataBuku *katalogBuku) {
    int jumlahBeli;
    cout << "Masukkan jumlah beli: "; cin >> jumlahBeli;

    if (jumlahBeli <= 0) { // Jika stok buku yang dimasukkan <= 0
        cout << "Jumlah beli tidak valid" << endl;
        return;
    }

    if (jumlahBeli <= katalogBuku->stok) { // Jika jumlah beli tidak melebihi stok buku yang ada
        DataBuku *newNode = (DataBuku *) malloc(sizeof(DataBuku));

        // Salin node hasil search buku ke node di keranjang
        strcpy(newNode->isbn, katalogBuku->isbn);
        strcpy(newNode->judul, katalogBuku->judul);
        strcpy(newNode->penulis, katalogBuku->penulis);
        newNode->stok = jumlahBeli; // Menyimpan kuantitas buku yang dibeli
        newNode->harga = katalogBuku->harga;

        newNode->prev = NULL;
        newNode->next = NULL;

        if (keranjangHead == NULL) {
            keranjangHead = keranjangTail = newNode;
        }
        else {
            keranjangTail->next = newNode;
            newNode->prev = keranjangTail;
            keranjangTail = newNode;
        }
        
        cout << "Buku berhasil dimasukkan ke keranjang!" << endl;
    }
    else {
        cout << "Stok tidak mencukupi! (Sisa stok adalah " << katalogBuku->stok << ")" << endl;
    }
}

void lihatKeranjang() {
    DataBuku *temp;
    temp = keranjangHead;
    bool kembali = true;
    char backToMenu;

    if (temp == NULL) {
        cout << "Keranjang masih kosong, silakan isi melalui Menu Kasir [2] Cari Buku." << endl;
        kasir();
    }

    cout << setfill(' ') << setw(35) << " " << endl;
    cout << "K E R A N J A N G  B U K U";
    cout << setfill(' ') << setw(35) << " " << endl;
    cout << setfill('=') << setw(96) << "" << setfill(' ') << endl;
    printf("| %-10s | %-26s | %-20s | %-5s | %-19s |\n", "ISBN", "JUDUL", "PENULIS", "STOK", "HARGA");
    cout << setfill('=') << setw(96) << "" << setfill(' ') << endl;
    while (temp != NULL) {
        printf("| %-10s | %-26s | %-20s | %-5d | Rp%-17.2lf |\n", temp->isbn, temp->judul, temp->penulis, temp->stok, temp->harga);
        temp = temp->next;
    }
    cout << setfill('=') << setw(96) << "" << setfill(' ') << endl;

    while (kembali) {
        cout << "Kembali ke Menu Kasir (Y/N): "; cin >> backToMenu;
        if (toupper(backToMenu) == 'Y') {
            return;
            break;
        }
        else if (toupper(backToMenu) == 'N') {
            // system("cls");
            cout << "Terima kasih!" << endl;
            // system("pause");
            exit(1);
        }
    }
}