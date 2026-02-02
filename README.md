# 📚 Library Automation System (C Based)

![Language](https://img.shields.io/badge/Language-C-blue)
![Structure](https://img.shields.io/badge/Data%20Structure-Linked%20Lists-orange)
![Storage](https://img.shields.io/badge/Storage-CSV%20File%20IO-yellow)

Bu proje, **C programlama dili** kullanılarak geliştirilmiş, verilerin **Bağlı Liste (Linked List)** yapıları üzerinde dinamik olarak yönetildiği kapsamlı bir kütüphane otomasyonudur.

Sistem, sabit diziler yerine işaretçi (pointer) tabanlı bellek yönetimi kullanarak yüksek performans ve esneklik sağlar.

## 🚀 Proje Özellikleri

Proje, ileri seviye C konseptlerini pratik bir uygulamada birleştirir:

* **Dinamik Veri Yapıları:** Öğrenci ve Kitap verileri için **Çift Yönlü Bağlı Liste (Doubly Linked List)** kullanılmıştır.
* **Fonksiyon İşaretçileri (Function Pointers):** Kod tekrarını önlemek için jenerik listeleme fonksiyonları (`void (*ListFunc)`) kullanılmıştır.
* **Kalıcı Veri (Persistence):** Program kapansa bile veriler özel parser (ayrıştırıcı) ile `.csv` dosyalarında saklanır ve geri yüklenir.
* **Ceza Algoritması:** Kitapların iade tarihlerini `time.h` kütüphanesi ile hesaplayarak otomatik ceza puanı işler.

## 🧠 Algoritma Mantığı

Sistemindeki **"Kitap-Yazar İlişkisi"** şu şekilde yönetilir:

1.  **Arama:** Girilen ISBN numarası, Kitap Bağlı Listesinde aranır ($O(n)$).
2.  **Eşleştirme:** Kitap bulunduğunda, Yazar ID'si üzerinden Yazar Bağlı Listesine işaretçi (pointer) ile erişilir.
3.  **Stok Kontrolü:** Kitap ödünç verilirse `stok` azalır, `ödünç_verilen` artar ve log dosyasına tarih damgası vurulur.

## 📂 Proje Yapısı

```bash
Library-Automation-System-C/
├── src/
│   └── main.c          # Ana kod (Structs, Pointers, Logic)
├── data/               # Veritabanı (Kitaplar.csv, Ogrenciler.csv)
├── docs/               # Proje raporu
└── README.md
