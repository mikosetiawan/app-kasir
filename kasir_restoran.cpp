#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <windows.h>
#include <ctime>        // untuk tanggal & jam
using namespace std;

struct Pesanan {
    string nama;
    int jumlah;
    double hargaSatuan;
    double subtotal;
};

int main() {
    // Menu
    string menuMakanan[5] = {"Nasi Goreng", "Mie Goreng", "Ayam Bakar", "Soto Ayam", "Rendang"};
    double hargaMakanan[5] = {15000, 12000, 25000, 18000, 30000};

    string menuMinuman[5] = {"Es Teh", "Jus Jeruk", "Kopi Hitam", "Teh Hangat", "Air Mineral"};
    double hargaMinuman[5] = {5000, 10000, 8000, 4000, 3000};

    vector<Pesanan> daftarPesanan;
    int pilihJenis, pilihMenu, jumlah;
    double totalHarga = 0;
    char lanjut;

    system("cls"); // bersihkan layar
    cout << "==========================================\n";
    cout << "     KASIR RESTORAN SEDERHANA v2.0        \n";
    cout << "==========================================\n\n";

    // Proses pesan (sama seperti sebelumnya)
    do {
        cout << "Pilih jenis: 1. Makanan  2. Minuman : ";
        cin >> pilihJenis;

        if (pilihJenis == 1) {
            cout << "\n--- MAKANAN ---\n";
            for (int i = 0; i < 5; i++)
                cout << i+1 << ". " << menuMakanan[i] << " - Rp " << hargaMakanan[i] << endl;
            cout << "Pilih (1-5): ";
            cin >> pilihMenu;
            if (pilihMenu >= 1 && pilihMenu <= 5) {
                cout << "Jumlah: ";
                cin >> jumlah;
                double sub = hargaMakanan[pilihMenu-1] * jumlah;
                totalHarga += sub;
                daftarPesanan.push_back({menuMakanan[pilihMenu-1], jumlah, hargaMakanan[pilihMenu-1], sub});
                cout << "Berhasil ditambah!\n\n";
            }
        }
        else if (pilihJenis == 2) {
            cout << "\n--- MINUMAN ---\n";
            for (int i = 0; i < 5; i++)
                cout << i+1 << ". " << menuMinuman[i] << " - Rp " << hargaMinuman[i] << endl;
            cout << "Pilih (1-5): ";
            cin >> pilihMenu;
            if (pilihMenu >= 1 && pilihMenu <= 5) {
                cout << "Jumlah: ";
                cin >> jumlah;
                double sub = hargaMinuman[pilihMenu-1] * jumlah;
                totalHarga += sub;
                daftarPesanan.push_back({menuMinuman[pilihMenu-1], jumlah, hargaMinuman[pilihMenu-1], sub});
                cout << "Berhasil ditambah!\n\n";
            }
        }

        cout << "Tambah pesanan lagi? (y/t): ";
        cin >> lanjut;
        cout << endl;
    } while (lanjut == 'y' || lanjut == 'Y');

    // Pembayaran
    cout << "\nTotal Belanja : Rp " << fixed << setprecision(0) << totalHarga << endl;
    double bayar;
    cout << "Uang Bayar    : Rp ";
    cin >> bayar;

    if (bayar < totalHarga) {
        cout << "Uang kurang Rp " << (totalHarga - bayar) << " ! Transaksi dibatalkan.\n";
        system("pause");
        return 0;
    }

    double kembalian = bayar - totalHarga;
    cout << "Kembalian     : Rp " << kembalian << endl;
    cout << "\nTerima kasih!\n\n";

    // ================== BUAT FILE HTML → PDF CANTIK ==================
    ofstream file("struk.html");
    time_t now = time(0);
    char* dt = ctime(&now);

    file << "<!DOCTYPE html>\n<html>\n<head>\n";
    file << "<meta charset=\"UTF-8\">\n";
    file << "<title>Struk Pembayaran</title>\n";
    file << "<style>\n";
    file << "  body {font-family: Arial, sans-serif; margin: 20px;}\n";
    file << "  .header {text-align: center; margin-bottom: 20px;}\n";
    file << "  table {width: 100%; border-collapse: collapse;}\n";
    file << "  th, td {padding: 8px; text-align: left; border-bottom: 1px solid #ddd;}\n";
    file << "  .total {font-weight: bold; font-size: 1.2em;}\n";
    file << "  .footer {text-align: center; margin-top: 30px; font-size: 0.9em;}\n";
    file << "</style>\n</head>\n<body>\n";

    file << "<div class=\"header\">\n";
    file << "  <h2>RESTORAN MAKAN ENAK</h2>\n";
    file << "  <p>Jl. Contoh No.123 - Telp: 0812-3456-7890</p>\n";
    file << "  <p>" << dt << "</p>\n";
    file << "  <h3>=== S T R U K ===</h3>\n";
    file << "</div>\n";

    file << "<table>\n";
    file << "<tr><th>No</th><th>Item</th><th>Jml</th><th>Harga</th><th>Subtotal</th></tr>\n";

    for (int i = 0; i < daftarPesanan.size(); i++) {
        file << "<tr>";
        file << "<td>" << (i+1) << "</td>";
        file << "<td>" << daftarPesanan[i].nama << "</td>";
        file << "<td>" << daftarPesanan[i].jumlah << "</td>";
        file << "<td>Rp " << fixed << setprecision(0) << daftarPesanan[i].hargaSatuan << "</td>";
        file << "<td>Rp " << daftarPesanan[i].subtotal << "</td>";
        file << "</tr>\n";
    }

    file << "</table>\n<br>\n";

    file << "<p class=\"total\">Total         : Rp " << totalHarga << "</p>\n";
    file << "<p class=\"total\">Bayar         : Rp " << bayar << "</p>\n";
    file << "<p class=\"total\">Kembalian     : Rp " << kembalian << "</p>\n";

    file << "<div class=\"footer\">\n";
    file << "  <p>~~~ Terima kasih atas kunjungan Anda ~~~</p>\n";
    file << "  <p>Barang yang sudah dibeli tidak dapat ditukar/dikembalikan</p>\n";
    file << "</div>\n";

    file << "</body></html>";
    file.close();

    cout << "Struk PDF berhasil dibuat!\n";
    cout << "Membuka struk untuk dicetak...\n";

    // Buka otomatis di browser → tinggal Ctrl+P → Save as PDF atau langsung Print
    ShellExecuteA(NULL, "open", "struk.html", NULL, NULL, SW_SHOW);

    system("pause");
    return 0;
}