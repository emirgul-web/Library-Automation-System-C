📚 Kütüphane Otomasyon Sistemi (Library Automation System)
Bu proje, C programlama dili kullanılarak geliştirilmiş, verilerin dinamik bellek yönetimi ve Bağlı Liste (Linked List) yapıları üzerinde tutulduğu kapsamlı bir kütüphane yönetim sistemidir.

Proje, standart dizi (array) yapısı yerine, bellek verimliliği sağlayan işaretçi (pointer) tabanlı veri yapılarını ve verilerin kalıcılığı için CSV dosya sistemini kullanır.

🚀 Öne Çıkan Teknik Yetkinlikler
Bu proje, temel programlamanın ötesinde şu ileri seviye C konseptlerini içerir:

Dinamik Veri Yapıları: Öğrenci, Yazar ve Kitap verileri sabit boyutlu dizilerde değil, Tek ve Çift Yönlü Bağlı Listelerde (Singly & Doubly Linked Lists) dinamik olarak yönetilir.

Fonksiyon İşaretçileri (Function Pointers): Kod tekrarını önlemek ve modülerliği artırmak için jenerik listeleme fonksiyonlarında (void (*ListFunc)(void*)) fonksiyon işaretçileri kullanılmıştır.

Dosya İşlemleri (File I/O): Veriler program kapandığında kaybolmaz; .csv formatındaki veritabanı dosyalarına yazılır ve açılışta tekrar belleğe yüklenir (Parsing).

Manuel Bellek Yönetimi: malloc ve free fonksiyonları ile bellek sızıntılarını (memory leaks) önleyecek şekilde hassas bellek yönetimi yapılmıştır.

🛠️ Sistem Özellikleri ve Modüller
1. Öğrenci Yönetimi
Sisteme yeni öğrenci eklendiğinde otomatik ID ataması yapar.

Öğrencilerin ödünç aldığı kitapları ve ceza durumlarını takip eder.

Çift Yönlü Bağlı Liste yapısı sayesinde veriler üzerinde ileri-geri gezinme imkanı sunar.

2. Kitap ve Yazar Entegrasyonu
Kitaplar, yazarlarıyla ilişkilendirilmiş (Linked) bir yapıda tutulur.

ISBN numarasına göre kitap sorgulama, güncelleme ve silme işlemleri yapılır.

Kitapların stok durumu (Rafta / Ödünçte) anlık olarak güncellenir.

3. Ödünç/İade Algoritması (Business Logic)
Tarih Hesaplama: Kitap ödünç verildiği andan itibaren 15 günlük süre başlar.

Ceza Sistemi: İade tarihi geçen her gün için öğrenciye otomatik ceza puanı yansıtılır.

Kitap iade edilmeden yeni kitap alımı engellenir.

📂 Proje Yapısı
Bash
Library-Automation-System-C/
├── src/
│   └── main.c          # Ana kaynak kod (Main logic, Structs, Linked List ops)
├── data/               # Veritabanı (Kitaplar.csv, Ogrenciler.csv, Yazarlar.csv)
├── docs/               # Proje raporu ve akış diyagramları
└── README.md           # Dokümantasyon
💻 Kurulum ve Çalıştırma
Projeyi derlemek için GCC derleyicisi önerilir. Terminal veya CMD üzerinden şu komutları kullanabilirsiniz:

Bash
# Projeyi derle
gcc src/main.c -o library_app

# Çalıştır (Windows)
library_app.exe

# Çalıştır (Linux/Mac)
./library_app
