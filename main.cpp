
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <limits>
#include <iomanip>

using namespace std;

// Struktur data pasien
struct Pasien {
    string idPasien;
    string nama;
    int tingkatDarurat;
    string waktuDatang;
    int urutanDatang;
};

// Antrean pasien dan riwayat
vector<Pasien> priorityQueue;
vector<Pasien> patientLog;

int nomorUrut = 0;

// Mengubah tingkat darurat menjadi keterangan
string keteranganDarurat(int tingkat) {
    switch (tingkat) {
        case 1: return "MERAH (KRITIS)";
        case 2: return "KUNING (SEDANG)";
        case 3: return "HIJAU (RINGAN)";
        default: return "TIDAK DIKETAHUI";
    }
}

// Mengurutkan pasien berdasarkan prioritas
void urutkanAntrean() {
    stable_sort(
        priorityQueue.begin(),
        priorityQueue.end(),
        [](const Pasien &a, const Pasien &b) {
            if (a.tingkatDarurat != b.tingkatDarurat) {
                return a.tingkatDarurat < b.tingkatDarurat;
            }

            return a.urutanDatang < b.urutanDatang;
        }
    );
}

// Menambahkan pasien
void tambahPasien() {
    Pasien p;

    cout << "\n===== PENDAFTARAN PASIEN =====\n";

    cout << "ID Pasien       : ";
    getline(cin, p.idPasien);

    cout << "Nama Pasien     : ";
    getline(cin, p.nama);

    cout << "Tingkat Darurat : ";
    if (!(cin >> p.tingkatDarurat)) {
        cout << "Input harus berupa angka!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (p.idPasien.empty() || p.nama.empty()) {
        cout << "ID dan nama pasien tidak boleh kosong!\n";
        return;
    }

    if (p.tingkatDarurat < 1 || p.tingkatDarurat > 3) {
        cout << "Tingkat darurat hanya boleh 1, 2, atau 3!\n";
        return;
    }

    // Memeriksa ID agar tidak duplikat
    for (const Pasien &pasien : priorityQueue) {
        if (pasien.idPasien == p.idPasien) {
            cout << "ID pasien sudah terdaftar!\n";
            return;
        }
    }

    for (const Pasien &pasien : patientLog) {
        if (pasien.idPasien == p.idPasien) {
            cout << "ID pasien sudah pernah diproses!\n";
            return;
        }
    }

    cout << "Waktu Datang    : ";
    getline(cin, p.waktuDatang);

    if (p.waktuDatang.empty()) {
        cout << "Waktu datang tidak boleh kosong!\n";
        return;
    }

    nomorUrut++;
    p.urutanDatang = nomorUrut;

    priorityQueue.push_back(p);

    urutkanAntrean();

    cout << "\nPasien berhasil ditambahkan!\n";
    cout << "Prioritas: " << keteranganDarurat(p.tingkatDarurat) << endl;
}

// Menampilkan antrean pasien
void tampilkanAntrean() {
    cout << "\n========== ANTREAN PASIEN IGD ==========\n";

    if (priorityQueue.empty()) {
        cout << "Belum ada pasien dalam antrean.\n";
        return;
    }

    cout << left
         << setw(5) << "No"
         << setw(12) << "ID"
         << setw(20) << "Nama"
         << setw(20) << "Prioritas"
         << "Waktu Datang\n";

    cout << string(85, '-') << endl;

    for (int i = 0; i < (int)priorityQueue.size(); i++) {
        const Pasien &p = priorityQueue[i];

        cout << left
             << setw(5) << i + 1
             << setw(12) << p.idPasien
             << setw(20) << p.nama
             << setw(20) << keteranganDarurat(p.tingkatDarurat)
             << p.waktuDatang << endl;
    }

    cout << "\nTotal pasien menunggu: "
         << priorityQueue.size() << endl;
}

// Menangani pasien terdepan
void tanganiPasien() {
    cout << "\n========== PENANGANAN PASIEN ==========\n";

    if (priorityQueue.empty()) {
        cout << "Tidak ada pasien yang perlu ditangani.\n";
        return;
    }

    Pasien pasien = priorityQueue.front();

    cout << "Pasien yang akan ditangani:\n";
    cout << "ID Pasien   : " << pasien.idPasien << endl;
    cout << "Nama        : " << pasien.nama << endl;
    cout << "Prioritas   : "
         << keteranganDarurat(pasien.tingkatDarurat) << endl;
    cout << "Waktu Datang: " << pasien.waktuDatang << endl;

    char konfirmasi;

    cout << "\nApakah pasien sudah selesai ditangani? (y/n): ";
    cin >> konfirmasi;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (konfirmasi == 'y' || konfirmasi == 'Y') {
        patientLog.push_back(pasien);
        priorityQueue.erase(priorityQueue.begin());

        cout << "\nPasien berhasil dipindahkan ke riwayat penanganan!\n";
    } else {
        cout << "\nPenanganan pasien belum diselesaikan.\n";
    }
}

// Menampilkan riwayat pasien
void tampilkanRiwayat() {
    cout << "\n========== RIWAYAT PENANGANAN ==========\n";

    if (patientLog.empty()) {
        cout << "Belum ada riwayat penanganan.\n";
        return;
    }

    for (int i = 0; i < (int)patientLog.size(); i++) {
        const Pasien &p = patientLog[i];

        cout << "\nPasien ke-" << i + 1 << endl;
        cout << "ID Pasien   : " << p.idPasien << endl;
        cout << "Nama        : " << p.nama << endl;
        cout << "Prioritas   : "
             << keteranganDarurat(p.tingkatDarurat) << endl;
        cout << "Waktu Datang: " << p.waktuDatang << endl;
    }
}

// Menyimpan riwayat ke file
void simpanRiwayat() {
    ofstream file("log_triase.txt");

    if (!file.is_open()) {
        cout << "Gagal membuka file log_triase.txt!\n";
        return;
    }

    file << "========== LOG TRIASE PASIEN IGD ==========\n\n";

    file << left
         << setw(12) << "ID Pasien"
         << setw(20) << "Nama"
         << setw(20) << "Prioritas"
         << "Waktu Datang\n";

    file << string(75, '-') << endl;

    for (const Pasien &p : patientLog) {
        file << left
             << setw(12) << p.idPasien
             << setw(20) << p.nama
             << setw(20) << keteranganDarurat(p.tingkatDarurat)
             << p.waktuDatang << endl;
    }

    file << "\nTotal pasien selesai ditangani: "
         << patientLog.size() << endl;

    file.close();

    cout << "\nRiwayat berhasil disimpan ke log_triase.txt!\n";
}

// Menu utama
int main() {
    int pilihan;

    do {
        cout << "\n";
        cout << "============================================\n";
        cout << "       CLINIC TRIAGE SYSTEM (IGD)\n";
        cout << "============================================\n";
        cout << "1. Tambah Pasien\n";
        cout << "2. Lihat Antrean Pasien\n";
        cout << "3. Tangani Pasien\n";
        cout << "4. Lihat Riwayat Penanganan\n";
        cout << "5. Simpan Riwayat ke File\n";
        cout << "0. Keluar\n";
        cout << "============================================\n";
        cout << "Pilih Menu: ";

        if (!(cin >> pilihan)) {
            cout << "Input tidak valid!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (pilihan) {
            case 1:
                tambahPasien();
                break;

            case 2:
                tampilkanAntrean();
                break;

            case 3:
                tanganiPasien();
                break;

            case 4:
                tampilkanRiwayat();
                break;

            case 5:
                simpanRiwayat();
                break;

            case 0:
                simpanRiwayat();
                cout << "\nTerima kasih telah menggunakan "
                     << "Clinic Triage System!\n";
                break;

            default:
                cout << "Pilihan menu tidak tersedia!\n";
        }

    } while (pilihan != 0);

    return 0;
}

