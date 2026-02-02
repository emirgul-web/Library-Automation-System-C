#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct YAZAR {
    char isim[50];
    char soyisim[50];
    int yazarID;
    struct YAZAR *next;
} YAZAR;

typedef struct BORROW_LOG {
    char etiket[30];
    int studentID;
    int islemTuru;
    char tarih[12];
    struct BORROW_LOG *next;
} BORROW_LOG;

typedef struct STUDENT {
    char isim[50];
    char soyisim[50];
    int studentID;
    int puan;
    struct STUDENT *prev;
    struct STUDENT *next;
} STUDENT;

typedef struct BOOK_COPY {
    char etiket[30];
    int odunc_durumu; 
    struct BOOK_COPY *next;
} BOOK_COPY;

typedef struct BOOK {
    char kitapad[50];
    long long isbn;
    int adet;
    struct BOOK *next;
    BOOK_COPY *copiesHead;
} BOOK;

typedef struct {
    long long isbn;
    int yazarID;
} KitapYazar;



typedef void (*ListFunc)(void*);
void yazar_dosyaya_yaz(YAZAR *head);
void ogrenci_dosyaya_yaz(STUDENT *head);



int validate_string(char *str) {
    return str[0] != '\0' && str[0] != '\n';
}

int validate_id(int id) {
    return id > 0;
}

int validate_isbn(long long isbn) {
    return isbn >= 1000000000000LL && isbn <= 9999999999999LL;
}

int compare_dates(const char *date1, const char *date2) {
    int d1, m1, y1, d2, m2, y2;
    sscanf(date1, "%d.%d.%d", &d1, &m1, &y1);
    sscanf(date2, "%d.%d.%d", &d2, &m2, &y2);
    
    struct tm t1 = {0}, t2 = {0};
    t1.tm_mday = d1; t1.tm_mon = m1 - 1; t1.tm_year = y1 - 1900;
    t2.tm_mday = d2; t2.tm_mon = m2 - 1; t2.tm_year = y2 - 1900;
    
    time_t time1 = mktime(&t1);
    time_t time2 = mktime(&t2);
    
    double diff = difftime(time2, time1) / (60 * 60 * 24);
    return (int)diff;
}


KitapYazar* csv_oku(const char* dosya_adi, int* eleman_sayisi);

void yazar_ekle(YAZAR **head, int *count) {
    YAZAR *s1 = malloc(sizeof(YAZAR));
    if (!s1) {
        printf("Bellek ayrýlamadý!\n");
        return;
    }

    printf("Yazarýn ismini ve soyismini giriniz: ");
    scanf("%s %s", s1->isim, s1->soyisim);
    if (!validate_string(s1->isim) || !validate_string(s1->soyisim)) {
        printf("Geçersiz isim veya soyisim!\n");
        free(s1);
        return;
    }

    s1->yazarID = (*count)++;
    s1->next = NULL;

    if (*head == NULL) {
        *head = s1;
    } else {
        YAZAR *tmp = *head;
        YAZAR *prev = NULL;
        while (tmp != NULL && tmp->yazarID < s1->yazarID) {
            prev = tmp;
            tmp = tmp->next;
        }
        if (prev == NULL) {
            s1->next = *head;
            *head = s1;
        } else {
            prev->next = s1;
            s1->next = tmp;
        }
    }

    yazar_dosyaya_yaz(*head);
    printf("Yazar eklendi: %s %s, ID: %d\n", s1->isim, s1->soyisim, s1->yazarID);
}

void yazar_sil(YAZAR **head) {
    int silinecekID;
    printf("Silinecek yazarýn ID'sini giriniz: ");
    scanf("%d", &silinecekID);
    if (!validate_id(silinecekID)) {
        printf("Geçersiz yazar ID!\n");
        return;
    }

    YAZAR *tmp = *head;
    YAZAR *prev = NULL;
    int found = 0;

    while (tmp != NULL) {
        if (tmp->yazarID == silinecekID) {
            found = 1;
            break;
        }
        prev = tmp;
        tmp = tmp->next;
    }

    if (!found) {
        printf("Yazar bulunamadý!\n");
        return;
    }

    if (prev == NULL) {
        *head = tmp->next;
    } else {
        prev->next = tmp->next;
    }
   
    int i;
    int eleman_sayisi;
    KitapYazar *liste = csv_oku("KitapYazar.csv", &eleman_sayisi);
    if (liste) {
        for (i = 0; i < eleman_sayisi; i++) {
            if (liste[i].yazarID == silinecekID) {
                liste[i].yazarID = -1;
            }
        }
        csv_yaz("KitapYazar.csv", liste, eleman_sayisi);
        free(liste);
    }

    free(tmp);
    yazar_dosyaya_yaz(*head);
    printf("Yazar silindi.\n");
}


void yazar_guncelle(YAZAR *head) {
    int guncellenecekID;
    printf("Güncellenecek yazarýn ID'sini girin: ");
    scanf("%d", &guncellenecekID);
    if (!validate_id(guncellenecekID)) {
        printf("Geçersiz yazar ID!\n");
        return;
    }

    YAZAR *tmp = head;
    int found = 0;

    while (tmp != NULL) {
        if (tmp->yazarID == guncellenecekID) {
            printf("Yeni ismi ve soyismi girin: ");
            scanf("%s %s", tmp->isim, tmp->soyisim);
            if (!validate_string(tmp->isim) || !validate_string(tmp->soyisim)) {
                printf("Geçersiz yeni isim veya soyisim!\n");
                return;
            }
            found = 1;
            break;
        }
        tmp = tmp->next;
    }

    if (!found) {
        printf("Yazar bulunamadý!\n");
        return;
    }

    yazar_dosyaya_yaz(head);
    printf("Yazar bilgisi güncellendi.\n");
}




void yazar_listele_fp(void *head) {
    YAZAR *tmp = (YAZAR*)head;
    printf("\n--- Yazar Listesi ---\n");
    if (tmp == NULL) {
        printf("Yazar listesi bos!\n");
    } else {
        while (tmp != NULL) {
            printf("%d,%s,%s\n", tmp->yazarID, tmp->isim, tmp->soyisim);
            tmp = tmp->next;
        }
    }
    printf("\n");
}

void yazar_dosyaya_yaz(YAZAR *head) {
    FILE *dosya = fopen("Yazarlar.csv", "w");
    if (!dosya) {
        printf("Dosya açýlamadý!\n");
        return;
    }

    YAZAR *tmp = head;
    while (tmp != NULL) {
        fprintf(dosya, "%d,%s,%s\n", tmp->yazarID, tmp->isim, tmp->soyisim);
        tmp = tmp->next;
    }
    fclose(dosya);
}

void yazar_dosyadan_oku(YAZAR **head, int *count) {
    FILE *dosya = fopen("Yazarlar.csv", "r");
    if (!dosya) {
        printf("Yazarlar.csv dosyasi acilamadi!\n");
        return;
    }

    char satir[256];
    int hata_var = 0;

    while (fgets(satir, sizeof(satir), dosya) != NULL && !hata_var) {
        YAZAR *yeni = malloc(sizeof(YAZAR));
        if (!yeni) {
            printf("Bellek ayrilamadi!\n");
            hata_var = 1;
        } else {
       
            int scanned = sscanf(satir, "%d,%[^,],%s", &yeni->yazarID, yeni->isim, yeni->soyisim);
            if (scanned != 3) {
                printf("Satir okunamadi: %s", satir);
                free(yeni);
            } else {
                yeni->next = NULL;
                
                if (*head == NULL) {
                    *head = yeni;
                } else {
                    YAZAR *tmp = *head;
                    YAZAR *prev = NULL;
                    while (tmp != NULL && tmp->yazarID < yeni->yazarID) {
                        prev = tmp;
                        tmp = tmp->next;
                    }
                    if (prev == NULL) {
                        yeni->next = *head;
                        *head = yeni;
                    } else {
                        prev->next = yeni;
                        yeni->next = tmp;
                    }
                }
                if (yeni->yazarID >= *count) {
                    *count = yeni->yazarID + 1;
                }
            }
        }
    }
    fclose(dosya);
}


void ogrenci_ekle(STUDENT **head) {
    STUDENT *s2 = malloc(sizeof(STUDENT));
    if (!s2) {
        printf("Bellek ayrilamadi!\n");
        return;
    }

    printf("Ogrencinin adini ve soyadini giriniz: ");
    scanf("%s %s", s2->isim, s2->soyisim);
    if (s2->isim[0] == '\0' || s2->soyisim[0] == '\0') {
        printf("Gecersiz isim veya soyisim!\n");
        free(s2);
        return;
    }

    s2->puan = 100;
    s2->next = NULL;
    s2->prev = NULL;

   
    int max_id = 18011000; 
    STUDENT *tmp = *head;
    while (tmp != NULL) {
        if (tmp->studentID > max_id) {
            max_id = tmp->studentID;
        }
        tmp = tmp->next;
    }
    s2->studentID = max_id + 1;


    if (*head == NULL) {
        *head = s2;
    } else {
        tmp = *head;
        STUDENT *prev = NULL;
        while (tmp != NULL && tmp->studentID < s2->studentID) {
            prev = tmp;
            tmp = tmp->next;
        }
        if (prev == NULL) {
            s2->next = *head;
            (*head)->prev = s2;
            *head = s2;
        } else {
            s2->next = tmp;
            s2->prev = prev;
            prev->next = s2;
            if (tmp != NULL) {
                tmp->prev = s2;
            }
        }
    }

    ogrenci_dosyaya_yaz(*head);
    printf("Ogrenci eklendi: %s %s, ID: %d\n", s2->isim, s2->soyisim, s2->studentID);
}
void ogrenci_sil(STUDENT **head) {
    int silinecekID;
    printf("Silinecek öðrencinin ID'sini giriniz: ");
    scanf("%d", &silinecekID);
    if (!validate_id(silinecekID)) {
        printf("Geçersiz öðrenci ID!\n");
        return;
    }

    STUDENT *tmp = *head;
    STUDENT *prev = NULL;
    int found = 0;

    while (tmp != NULL) {
        if (tmp->studentID == silinecekID) {
            found = 1;

            if (prev == NULL) {
                *head = tmp->next;
                if (*head != NULL) {
                    (*head)->prev = NULL;
                }
            } else {
                prev->next = tmp->next;
                if (tmp->next != NULL) {
                    tmp->next->prev = prev;
                }
            }
            free(tmp);
            tmp = NULL; 
        } else {
            prev = tmp;
            tmp = tmp->next;
        }
    }

    if (!found) {
        printf("Öðrenci bulunamadý!\n");
        return;
    }

    ogrenci_dosyaya_yaz(*head);
    printf("Öðrenci silindi.\n");
}


void ogrenci_guncelle(STUDENT *head) {
    int guncellenecekID;
    printf("Güncellenecek öðrencinin ID'sini girin: ");
    scanf("%d", &guncellenecekID);
    if (!validate_id(guncellenecekID)) {
        printf("Geçersiz öðrenci ID!\n");
        return;
    }

    STUDENT *tmp = head;
    int found = 0;

    while (tmp != NULL) {
        if (tmp->studentID == guncellenecekID) {
            printf("Yeni ad ve soyad girin: ");
            scanf("%s %s", tmp->isim, tmp->soyisim);
            if (!validate_string(tmp->isim) || !validate_string(tmp->soyisim)) {
                printf("Geçersiz yeni isim veya soyisim!\n");
                return;
            }
            printf("Yeni puan girin: ");
            scanf("%d", &tmp->puan);
            found = 1;
            
            tmp = NULL; 
        } else {
            tmp = tmp->next;
        }
    }

    if (!found) {
        printf("Öðrenci bulunamadý!\n");
        return;
    }

    ogrenci_dosyaya_yaz(head);
    printf("Öðrenci bilgisi güncellendi.\n");
}


void ogrenci_listele_fp(void *head) {
    STUDENT *tmp = (STUDENT*)head;
    printf("\n--- ogrenci Listesi ---\n");
    if (tmp == NULL) {
        printf("ogrenci listesi bos!\n");
    } else {
        while (tmp != NULL) {
            printf("%d,%s,%s,%d\n", tmp->studentID, tmp->isim, tmp->soyisim, tmp->puan);
            tmp = tmp->next;
        }
    }
    printf("\n");
}


void ogrenci_dosyaya_yaz(STUDENT *head) {
    FILE *fp = fopen("Ogrenciler.csv", "w");
    if (!fp) {
        printf("Dosya acilamadi!\n");
        return;
    }

    STUDENT *tmp = head;
    while (tmp != NULL) {
        fprintf(fp, "%d,%s,%s,%d\n", tmp->studentID, tmp->isim, tmp->soyisim, tmp->puan);
        tmp = tmp->next;
    }
    fclose(fp);
}

void ogrenci_dosyadan_oku(STUDENT **head) {
    FILE *fp = fopen("Ogrenciler.csv", "r");
    if (!fp) {
        printf("Ogrenciler.csv dosyasi acilamadi!\n");
        return;
    }

    char satir[256];
    int hata_var = 0;

    while (fgets(satir, sizeof(satir), fp) != NULL && !hata_var) {
        STUDENT *yeni = malloc(sizeof(STUDENT));
        if (!yeni) {
            printf("Bellek ayrilamadi!\n");
            hata_var = 1;
        } else {
            
            int scanned;
            scanned = sscanf(satir, "%d,%[^,],%[^,],%d",&yeni->studentID, yeni->isim, yeni->soyisim,&yeni->puan);
            if (scanned != 4) {
                printf("Satir okunamadi: %s", satir);
                free(yeni);
            } else {
                yeni->next = NULL;
                yeni->prev = NULL;
                
                if (*head == NULL) {
                    *head = yeni;
                } else {
                    STUDENT *tmp = *head;
                    STUDENT *prev = NULL;
                    while (tmp != NULL && tmp->studentID < yeni->studentID) {
                        prev = tmp;
                        tmp = tmp->next;
                    }
                    if (prev == NULL) {
                        yeni->next = *head;
                        (*head)->prev = yeni;
                        *head = yeni;
                    } else {
                        yeni->next = tmp;
                        yeni->prev = prev;
                        prev->next = yeni;
                        if (tmp != NULL) {
                            tmp->prev = yeni;
                        }
                    }
                }
            }
        }
    }
    fclose(fp);
}

void ogrenci_bilgisi_goruntule(STUDENT *head, BORROW_LOG *logHead) {
    int ogrenciID;
    printf("Bilgisi görüntülenecek öðrencinin ID'sini girin: ");
    scanf("%d", &ogrenciID);
    if (!validate_id(ogrenciID)) {
        printf("Geçersiz öðrenci ID!\n");
        return;
    }

    STUDENT *tmp = head;
    int found = 0;

    while (tmp != NULL) {
        if (tmp->studentID == ogrenciID) {
            printf("Öðrenci: %s %s, ID: %d, Puan: %d\n", tmp->isim, tmp->soyisim, tmp->studentID, tmp->puan);
            printf("Kitap Hareketleri:\n");

            BORROW_LOG *log = logHead;
            while (log != NULL) {
                if (log->studentID == tmp->studentID) {
                    printf("Etiket: %s, Ýþlem: %s, Tarih: %s\n", log->etiket,
                           log->islemTuru == 0 ? "Ödünç Alma" : "Teslim", log->tarih);
                }
                log = log->next;
            }
            found = 1;
            tmp = NULL; 
        } else {
            tmp = tmp->next;
        }
    }

    if (!found) {
        printf("Öðrenci bulunamadý!\n");
    }
}

void teslim_etmemis_ogrencileri_listele_fp(void *args) {
    
    typedef struct {
        STUDENT *ogrenciHead;
        BORROW_LOG *logHead;
    } OgrLogArg;

    OgrLogArg *arg = (OgrLogArg*)args;

    STUDENT *ogr = arg->ogrenciHead;
    BORROW_LOG *logHead = arg->logHead;

    printf("\n--- Teslim Etmemiþ Öðrenciler ---\n");
    while (ogr != NULL) {
        int has_borrowed = 0;
        BORROW_LOG *log = logHead;
        while (log != NULL) {
            if (log->studentID == ogr->studentID && log->islemTuru == 0) {
                int returned = 0;
                BORROW_LOG *check = logHead;
                while (check != NULL && !returned) {
                    if (check->islemTuru == 1 && check->studentID == ogr->studentID &&
                        strcmp(check->etiket, log->etiket) == 0) {
                        returned = 1;
                    }
                    check = check->next;
                }
                if (!returned) {
                    has_borrowed = 1;
                }
            }
            log = log->next;
        }
        if (has_borrowed) {
            printf("%s %s, ID: %d\n", ogr->isim, ogr->soyisim, ogr->studentID);
        }
        ogr = ogr->next;
    }
}

void cezali_ogrencileri_listele_fp(void *head) {
    STUDENT *tmp = (STUDENT*)head;
    printf("\n--- Cezalý Öðrenciler ---\n");
    while (tmp != NULL) {
        if (tmp->puan < 0) {
            printf("%s %s, ID: %d, Puan: %d\n", tmp->isim, tmp->soyisim, tmp->studentID, tmp->puan);
        }
        tmp = tmp->next;
    }
}

void kitap_ekleme(BOOK **head) {
    getchar(); 
    BOOK *s3 = malloc(sizeof(BOOK));
    if (!s3) {
        printf("Bellek ayrýlamadý!\n");
        return;
    }

    printf("Kitabýn adýný giriniz: ");
    fgets(s3->kitapad, sizeof(s3->kitapad), stdin);
    s3->kitapad[strcspn(s3->kitapad, "\n")] = '\0';
    if (!validate_string(s3->kitapad)) {
        printf("Geçersiz kitap adý!\n");
        free(s3);
        return;
    }

    printf("Kitap adedini giriniz: ");
    scanf("%d", &s3->adet);
    if (s3->adet <= 0) {
        printf("Geçersiz adet!\n");
        free(s3);
        return;
    }

    s3->next = NULL;
    s3->copiesHead = NULL;

    long long max_isbn = 1000000000000LL;
    BOOK *tmp = *head;
    while (tmp != NULL) {
        if (tmp->isbn > max_isbn) {
            max_isbn = tmp->isbn;
        }
        tmp = tmp->next;
    }
    s3->isbn = max_isbn + 1;
   int i;
    for (i = 1; i <= s3->adet; i++) {
        BOOK_COPY *newCopy = malloc(sizeof(BOOK_COPY));
        if (!newCopy) {
            printf("Bellek ayrýlamadý!\n");
            
            BOOK_COPY *copy = s3->copiesHead;
            while (copy != NULL) {
                BOOK_COPY *temp = copy;
                copy = copy->next;
                free(temp);
            }
            free(s3);
            return;
        }
        sprintf(newCopy->etiket, "%lld_%d", s3->isbn, i);
        newCopy->odunc_durumu = 0;
        newCopy->next = NULL;

        if (s3->copiesHead == NULL) {
            s3->copiesHead = newCopy;
        } else {
            BOOK_COPY *copyTmp = s3->copiesHead;
            while (copyTmp->next != NULL) {
                copyTmp = copyTmp->next;
            }
            copyTmp->next = newCopy;
        }
    }

    if (*head == NULL) {
        *head = s3;
    } else {
        tmp = *head;
        BOOK *prev = NULL;
        while (tmp != NULL && (strcmp(tmp->kitapad, s3->kitapad) < 0 ||
               (strcmp(tmp->kitapad, s3->kitapad) == 0 && tmp->isbn < s3->isbn))) {
            prev = tmp;
            tmp = tmp->next;
        }
        if (prev == NULL) {
            s3->next = *head;
            *head = s3;
        } else {
            prev->next = s3;
            s3->next = tmp;
        }
    }
printf("Dosyaya yazma baþlýyor...\n");
    kitaplari_dosyaya_yaz(*head);
    printf("Kitap eklendi: %s, ISBN: %lld\n", s3->kitapad, s3->isbn);
}

void kitap_sil(BOOK **head) {
    long long isbn;
    printf("Silinecek kitabýn ISBN'sini giriniz: ");
    scanf("%lld", &isbn);

    if (!validate_isbn(isbn)) {
        printf("Geçersiz ISBN!\n");
        return;
    }

    BOOK *tmp = *head;
    BOOK *prev = NULL;
    int found = 0;

    while (tmp != NULL && !found) {
        if (tmp->isbn == isbn) {
            found = 1;
        } else {
            prev = tmp;
            tmp = tmp->next;
        }
    }

    if (!found) {
        printf("Kitap bulunamadý!\n");
        return;
    }

    if (prev == NULL) {
        *head = tmp->next;
    } else {
        prev->next = tmp->next;
    }

    
    BOOK_COPY *copy = tmp->copiesHead;
    while (copy != NULL) {
        BOOK_COPY *temp = copy;
        copy = copy->next;
        free(temp);
    }

    int i;
    int eleman_sayisi;
    KitapYazar *liste = csv_oku("KitapYazar.csv", &eleman_sayisi);
    if (liste) {
        for (i = 0; i < eleman_sayisi; i++) {
            if (liste[i].isbn == tmp->isbn) {
                liste[i].yazarID = -1; 
            }
        }
        csv_yaz("KitapYazar.csv", liste, eleman_sayisi);
        free(liste);
    }

    free(tmp);
    kitaplari_dosyaya_yaz(*head);
    printf("Kitap silindi.\n");
}

void kitaplari_dosyaya_yaz(BOOK *head) {
    FILE *kitap_fp = fopen("Kitaplar.csv", "w");
    FILE *kopya_fp = fopen("Kopyalar.csv", "w");

    if (!kitap_fp || !kopya_fp) {
        printf("Dosya yazýlamadý!\n");
        if (kitap_fp) fclose(kitap_fp);
        if (kopya_fp) fclose(kopya_fp);
        return;
    }

    BOOK *tmp = head;
    while (tmp != NULL) {
        fprintf(kitap_fp, "%s,%lld,%d\n", tmp->kitapad, tmp->isbn, tmp->adet);
        BOOK_COPY *copy = tmp->copiesHead;
        while (copy != NULL) {
            fprintf(kopya_fp, "%s,%d\n", copy->etiket, copy->odunc_durumu);
            copy = copy->next;
        }
        tmp = tmp->next;
    }

    fclose(kitap_fp);
    fclose(kopya_fp);
}

void kitap_guncelle(BOOK *head) {
    long long isbn;
    printf("Güncellenecek kitabýn ISBN'sini giriniz: ");
    scanf("%lld", &isbn);

    if (!validate_isbn(isbn)) {
        printf("Geçersiz ISBN!\n");
        return;
    }

    BOOK *tmp = head;
    int found = 0;

    while (tmp != NULL && !found) {
        if (tmp->isbn == isbn) {
            getchar(); 
            printf("Yeni adý girin: ");
            fgets(tmp->kitapad, sizeof(tmp->kitapad), stdin);
            tmp->kitapad[strcspn(tmp->kitapad, "\n")] = '\0';

            if (!validate_string(tmp->kitapad)) {
                printf("Geçersiz yeni kitap adý!\n");
                return;
            }

            printf("Yeni adet girin: ");
            scanf("%d", &tmp->adet);
            if (tmp->adet <= 0) {
                printf("Geçersiz adet!\n");
                return;
            }

         
            BOOK_COPY *copy = tmp->copiesHead;
            while (copy != NULL) {
                BOOK_COPY *temp = copy;
                copy = copy->next;
                free(temp);
            }
            tmp->copiesHead = NULL;

       
            int i;
            for (i = 1; i <= tmp->adet; i++) {
                BOOK_COPY *newCopy = malloc(sizeof(BOOK_COPY));
                if (!newCopy) {
                    printf("Bellek ayrýlamadý!\n");
                    return;
                }
                sprintf(newCopy->etiket, "%lld_%d", tmp->isbn, i);
                newCopy->odunc_durumu = 0;
                newCopy->next = NULL;

                if (tmp->copiesHead == NULL) {
                    tmp->copiesHead = newCopy;
                } else {
                    BOOK_COPY *copyTmp = tmp->copiesHead;
                    while (copyTmp->next != NULL) {
                        copyTmp = copyTmp->next;
                    }
                    copyTmp->next = newCopy;
                }
            }
            found = 1;
        }
        tmp = tmp->next;
    }

    if (!found) {
        printf("Kitap bulunamadý!\n");
        return;
    }

    kitaplari_dosyaya_yaz(head);
    printf("Kitap güncellendi.\n");
}


void kitap_bilgisi_goruntule(BOOK *head) {
    getchar();
    char aranan[50];
    printf("Kitap adýný giriniz: ");
    fgets(aranan, sizeof(aranan), stdin);
    aranan[strcspn(aranan, "\n")] = '\0';
    if (!validate_string(aranan)) {
        printf("Geçersiz kitap adý!\n");
        return;
    }

    BOOK *tmp = head;
    int found = 0;

    while (tmp != NULL && !found) {
        if (strcmp(tmp->kitapad, aranan) == 0) {
            printf("Kitap Adý: %s\n", tmp->kitapad);
            printf("ISBN: %lld\n", tmp->isbn);
            printf("Adet: %d\n", tmp->adet);
            BOOK_COPY *copy = tmp->copiesHead;
            while (copy != NULL) {
                printf("Kopya Etiketi: %s - Durum: %s\n",
                       copy->etiket, copy->odunc_durumu == 0 ? "Rafta" : "Ödünçte");
                copy = copy->next;
            }
            found = 1;
        }
        tmp = tmp->next;
    }

    if (!found) {
        printf("Kitap bulunamadý!\n");
    }
}

void raftaki_kitaplari_listele_fp(void *head) {
    BOOK *tmp = (BOOK*)head;
    printf("\n--- Raftaki Kitaplar ---\n");
    while (tmp != NULL) {
        BOOK_COPY *copy = tmp->copiesHead;
        while (copy != NULL) {
            if (copy->odunc_durumu == 0) {
                printf("%s - %s\n", tmp->kitapad, copy->etiket);
            }
            copy = copy->next;
        }
        tmp = tmp->next;
    }
}



void kitaplari_dosyadan_oku(BOOK **head) {
  
    FILE *kitap_fp = fopen("Kitaplar.csv", "r");
    if (!kitap_fp) {
        printf("Kitaplar.csv dosyasi acilamadi!\n");
        return;
    }

    char satir[256];
    while (fgets(satir, sizeof(satir), kitap_fp)) {
        BOOK *yeni = malloc(sizeof(BOOK));
        if (!yeni) {
            fclose(kitap_fp);
            return;
        }
        yeni->copiesHead = NULL;
        if (sscanf(satir, "%[^,],%lld,%d", yeni->kitapad, &yeni->isbn, &yeni->adet) != 3) {
            free(yeni);
        } else {
            yeni->next = NULL;

            if (*head == NULL) {
                *head = yeni;
            } else {
                BOOK *tmp = *head;
                BOOK *prev = NULL;
                while (tmp != NULL && tmp->isbn < yeni->isbn) {
                    prev = tmp;
                    tmp = tmp->next;
                }
                if (prev == NULL) {
                    yeni->next = *head;
                    *head = yeni;
                } else {
                    yeni->next = tmp;
                    prev->next = yeni;
                }
            }
        }
    }
    fclose(kitap_fp);

    
    FILE *kopya_fp = fopen("KitapOrnekleri.csv", "r");
    if (!kopya_fp) {
        printf("KitapOrnekleri.csv dosyasi acilamadi!\n");
        return;
    }

    while (fgets(satir, sizeof(satir), kopya_fp)) {
        char etiket[30];
        char durum[20];
        if (sscanf(satir, "%[^,],%s", etiket, durum) != 2) {
           
        } else {
            BOOK_COPY *newCopy = malloc(sizeof(BOOK_COPY));
            if (!newCopy) {
                fclose(kopya_fp);
                return;
            }
            strcpy(newCopy->etiket, etiket);
            newCopy->next = NULL;

            if (strcmp(durum, "Rafta") == 0) {
                newCopy->odunc_durumu = 0;
            } else {
                newCopy->odunc_durumu = atoi(durum);
            }

            long long isbn;
            sscanf(etiket, "%lld", &isbn);
            BOOK *tmp = *head;
            while (tmp != NULL && tmp->isbn != isbn) {
                tmp = tmp->next;
            }
            if (tmp == NULL) {
                free(newCopy);
            } else {
                if (tmp->copiesHead == NULL) {
                    tmp->copiesHead = newCopy;
                } else {
                    BOOK_COPY *copyTmp = tmp->copiesHead;
                    while (copyTmp->next != NULL) {
                        copyTmp = copyTmp->next;
                    }
                    copyTmp->next = newCopy;
                }
            }
        }
    }
    fclose(kopya_fp);
}



KitapYazar* csv_oku(const char* dosya_adi, int* eleman_sayisi) {
    FILE *dosya = fopen(dosya_adi, "r");
    if (!dosya) {
        printf("CSV dosyasý açýlamadý!\n");
        return NULL;
    }

    int kapasite = 10;
    *eleman_sayisi = 0;
    KitapYazar *liste = malloc(kapasite * sizeof(KitapYazar));
    if (!liste) {
        printf("Bellek ayrýlamadý!\n");
        fclose(dosya);
        return NULL;
    }

    char satir[100];
    while (fgets(satir, sizeof(satir), dosya) != NULL) {
        if (*eleman_sayisi >= kapasite) {
            kapasite *= 2;
            KitapYazar *temp = realloc(liste, kapasite * sizeof(KitapYazar));
            if (!temp) {
                printf("Bellek ayrýlamadý!\n");
                free(liste);
                fclose(dosya);
                return NULL;
            }
            liste = temp;
        }

        long long isbn;
        int yazarID;
        int scanned = sscanf(satir, "%lld,%d", &isbn, &yazarID);
        if (scanned == 2) {
            liste[*eleman_sayisi].isbn = isbn;
            liste[*eleman_sayisi].yazarID = yazarID;
            (*eleman_sayisi)++;
        }
    }

    fclose(dosya);
    return liste;
}

void csv_yaz(const char* dosya_adi, KitapYazar* liste, int eleman_sayisi) {
    FILE *dosya = fopen(dosya_adi, "w");
    if (!dosya) {
        printf("Dosya yazýlamadý!\n");
        return;
    }
  int i;
    for ( i = 0; i < eleman_sayisi; i++) {
        fprintf(dosya, "%lld,%d\n", liste[i].isbn, liste[i].yazarID);
    }

    fclose(dosya);
}

void kitap_yazar_eslestir(BOOK *bookHead, YAZAR *yazarHead) {
    long long isbn;
    int yazarID;
    printf("Eþleþtirilecek kitabýn ISBN'sini girin: ");
    scanf("%lld", &isbn);
    if (!validate_isbn(isbn)) {
        printf("Geçersiz ISBN!\n");
        return;
    }

    printf("Yazar ID'sini girin: ");
    scanf("%d", &yazarID);
    if (!validate_id(yazarID)) {
        printf("Geçersiz yazar ID!\n");
        return;
    }

    
    BOOK *book = bookHead;
    int book_found = 0;
    while (book != NULL && !book_found) {
        if (book->isbn == isbn) {
            book_found = 1;
        }
        book = book->next;
    }
    if (!book_found) {
        printf("Kitap bulunamadý!\n");
        return;
    }

   
    YAZAR *yazar = yazarHead;
    int yazar_found = 0;
    while (yazar != NULL && !yazar_found) {
        if (yazar->yazarID == yazarID) {
            yazar_found = 1;
        }
        yazar = yazar->next;
    }
    if (!yazar_found) {
        printf("Yazar bulunamadý!\n");
        return;
    }

  
    FILE *fp = fopen("KitapYazar.csv", "a");
    if (!fp) {
        printf("Dosya açýlamadý!\n");
        return;
    }
    fprintf(fp, "%lld,%d\n", isbn, yazarID);
    fclose(fp);
    printf("Eþleþtirme tamamlandý.\n");
}

void kitabin_yazarini_guncelle(KitapYazar *liste, int eleman_sayisi) {
    long long isbn;
    int yeni_id;
    printf("ISBN girin: ");
    scanf("%lld", &isbn);
    if (!validate_isbn(isbn)) {
        printf("Geçersiz ISBN!\n");
        return;
    }

    printf("Yeni yazar ID girin: ");
    scanf("%d", &yeni_id);
    if (!validate_id(yeni_id)) {
        printf("Geçersiz yazar ID!\n");
        return;
    }
    int i;
    int found = 0;
    for ( i = 0; i < eleman_sayisi; i++) {
        if (liste[i].isbn == isbn && liste[i].yazarID != -1) {
            liste[i].yazarID = yeni_id;
            found = 1;
        }
    }

    if (!found) {
        printf("Eþleþtirme bulunamadý!\n");
        return;
    }

    csv_yaz("KitapYazar.csv", liste, eleman_sayisi);
    printf("Yazar güncellendi.\n");
}

void yazar_bilgisi_goruntule(YAZAR *yazarHead, KitapYazar *liste, int eleman_sayisi) {
    char isim[50], soyisim[50];
    printf("Yazarýn ismini ve soyismini girin: ");
    scanf("%s %s", isim, soyisim);
    if (!validate_string(isim) || !validate_string(soyisim)) {
        printf("Geçersiz isim veya soyisim!\n");
        return;
    }
    int i;
    YAZAR *tmp = yazarHead;
    int found = 0;

    while (tmp != NULL && !found) {
        if (strcmp(tmp->isim, isim) == 0 && strcmp(tmp->soyisim, soyisim) == 0) {
            printf("Yazar: %s %s, ID: %d\n", tmp->isim, tmp->soyisim, tmp->yazarID);
            printf("Yazdýðý Kitaplar:\n");
            for ( i = 0; i < eleman_sayisi; i++) {
                if (liste[i].yazarID == tmp->yazarID) {
                    printf(" -> ISBN: %lld\n", liste[i].isbn);
                }
            }
            found = 1;
        }
        tmp = tmp->next;
    }

    if (!found) {
        printf("Yazar bulunamadý!\n");
    }
}


BORROW_LOG* odunc_log_dosyadan_oku(const char* logFile) {
    FILE *fp = fopen(logFile, "r");
    if (!fp) return NULL;

    char satir[256];
    BORROW_LOG *head = NULL;
    int hata_var = 0;

    while (fgets(satir, sizeof(satir), fp) != NULL && !hata_var) {
        BORROW_LOG *node = malloc(sizeof(BORROW_LOG));
        if (!node) {
            printf("Bellek ayrýlamadý!\n");
            hata_var = 1;
        } else {
            int scanned = sscanf(satir, "%[^,],%d,%d,%[^,\n]", node->etiket, &node->studentID, &node->islemTuru, node->tarih);
            if (scanned != 4) {
                free(node);
            } else {
                node->next = head;
                head = node;
            }
        }
    }

    fclose(fp);
    return head;
}

void kitap_odunc_islemi(STUDENT *ogrenciHead, BOOK *kitapHead, BORROW_LOG **logHead, const char* logFile) {
    int studentID, islem;
    char etiket[30], tarih[12];
    printf("Öðrenci ID girin: ");
    scanf("%d", &studentID);
    if (!validate_id(studentID)) {
        printf("Geçersiz öðrenci ID!\n");
        return;
    }

    printf("Kitap etiketini girin (örn: 1000000000001_1): ");
    scanf("%s", etiket);
    printf("Ýþlem türü (0=ödünç alma, 1=teslim): ");
    scanf("%d", &islem);
    if (islem != 0 && islem != 1) {
        printf("Geçersiz iþlem türü!\n");
        return;
    }
    printf("Tarih (gg.aa.yyyy): ");
    scanf("%s", tarih);

    STUDENT *ogr = ogrenciHead;
    int ogr_found = 0;
    while (ogr != NULL && !ogr_found) {
        if (ogr->studentID == studentID) {
            ogr_found = 1;
        }
        ogr = ogr->next;
    }
    if (!ogr_found) {
        printf("Öðrenci bulunamadý!\n");
        return;
    }
    if (islem == 0 && ogr->puan < 0) {
        printf("Öðrenci puaný negatif. Kitap verilemez!\n");
        return;
    }

    BOOK *kitap = kitapHead;
    BOOK_COPY *copy = NULL;
    int book_found = 0;
    while (kitap != NULL && !book_found) {
        BOOK_COPY *c = kitap->copiesHead;
        while (c != NULL && !book_found) {
            if (strcmp(c->etiket, etiket) == 0) {
                copy = c;
                book_found = 1;
            }
            c = c->next;
        }
        kitap = kitap->next;
    }
    if (!book_found) {
        printf("Kitap bulunamadý!\n");
        return;
    }

    if (islem == 0 && copy->odunc_durumu != 0) {
        printf("Kitap zaten ödünçte. Ýþlem baþarýsýz!\n");
        return;
    }
    if (islem == 1 && copy->odunc_durumu == 0) {
        printf("Kitap zaten rafta. Ýþlem baþarýsýz!\n");
        return;
    }

    copy->odunc_durumu = islem == 0 ? studentID : 0;

    if (islem == 1) {
        BORROW_LOG *tmp = *logHead;
        while (tmp != NULL) {
            if (tmp->islemTuru == 0 && tmp->studentID == studentID && strcmp(tmp->etiket, etiket) == 0) {
                int days = compare_dates(tmp->tarih, tarih);
                if (days > 15) {
                    ogr->puan -= 10;
                    printf("15 günü geçti, -10 puan cezasý verildi.\n");
                    ogrenci_dosyaya_yaz(ogrenciHead);
                }
            }
            tmp = tmp->next;
        }
    }

    BORROW_LOG *yeni = malloc(sizeof(BORROW_LOG));
    if (!yeni) {
        printf("Bellek ayrýlamadý!\n");
        return;
    }
    strcpy(yeni->etiket, etiket);
    yeni->studentID = studentID;
    yeni->islemTuru = islem;
    strcpy(yeni->tarih, tarih);
    yeni->next = *logHead;
    *logHead = yeni;
 
    FILE *fp = fopen(logFile, "a");
    if (!fp) {
        printf("Dosya açýlamadý!\n");
        return;
    }
    fprintf(fp, "%s,%d,%d,%s\n", etiket, studentID, islem, tarih);
    fclose(fp);

    kitaplari_dosyaya_yaz(kitapHead);
    printf("Ýþlem baþarýyla kaydedildi.\n");
}

void zamaninda_teslim_edilmeyenleri_listele(BORROW_LOG *logHead) {
    printf("\n--- Zamanýnda Teslim Edilmeyen Kitaplar ---\n");
    BORROW_LOG *log = logHead;
    while (log != NULL) {
        if (log->islemTuru == 1) {
            BORROW_LOG *borrow = logHead;
            while (borrow != NULL) {
                if (borrow->islemTuru == 0 && strcmp(borrow->etiket, log->etiket) == 0 &&
                    borrow->studentID == log->studentID) {
                    int days = compare_dates(borrow->tarih, log->tarih);
                    if (days > 15) {
                        printf("Etiket: %s, Öðrenci ID: %d, Ödünç: %s, Teslim: %s\n",
                               log->etiket, log->studentID, borrow->tarih, log->tarih);
                    }
                }
                borrow = borrow->next;
            }
        }
        log = log->next;
    }
}




int main() {
    int count = 1; 
    int secim = -1;
    YAZAR *yazarHead = NULL;
    STUDENT *ogrenciHead = NULL;
    ListFunc listFunc = NULL;
    BOOK *kitapHead = NULL;
    int eleman_sayisi = 0;
    BORROW_LOG *logHead = odunc_log_dosyadan_oku("KitapOdunc.csv");
    KitapYazar *liste = csv_oku("KitapYazar.csv", &eleman_sayisi);

    yazar_dosyadan_oku(&yazarHead, &count);
    ogrenci_dosyadan_oku(&ogrenciHead);
    kitaplari_dosyadan_oku(&kitapHead);

    while (secim != 0) {
        printf("\n----- KUTUPHANE OTOMASYON MENUSU -----\n");
        printf(" 1 - Ogrenci Ekle\n");
        printf(" 2 - Ogrenci Sil\n");
        printf(" 3 - Ogrenci Guncelle\n");
        printf(" 4 - Tum Ogrencileri Listele\n");
        printf(" 5 - Ogrenci Bilgisi Goruntule\n");
        printf(" 6 - Kitap Teslim Etmemis Ogrencileri Listele\n");
        printf(" 7 - Cezali Ogrencileri Listele\n");
        printf(" 8 - Kitap Ekle\n");
        printf(" 9 - Kitap Sil\n");
        printf("10 - Kitap Guncelle\n");
        printf("11 - Kitap Bilgisi Goruntule\n");
        printf("12 - Raftaki Kitaplarý Listele\n");
        printf("13 - Yazar Ekle\n");
        printf("14 - Yazar Sil\n");
        printf("15 - Yazar Guncelle\n");
        printf("16 - Yazar Listele\n");
        printf("17 - Kitap-Yazar Eslestir\n");
        printf("18 - Kitabin Yazarini Guncelle\n");
        printf("19 - Yazar Bilgisi Goruntule\n");
        printf("20 - Kitap Odunc/Iade Islemi\n");
        printf("21 - Zamanýnda Teslim Edilmeyen Kitaplar\n");
        printf(" 0 - Cikis\n");
        printf("Seçiminiz: ");
        scanf("%d", &secim);

        if (secim == 1) {
            ogrenci_ekle(&ogrenciHead);
        } else if (secim == 2) {
            ogrenci_sil(&ogrenciHead);
        } else if (secim == 3) {
            ogrenci_guncelle(ogrenciHead);
        } else if (secim == 4) {
            listFunc = ogrenci_listele_fp;
    listFunc((void*)ogrenciHead);
        } else if (secim == 5) {
            ogrenci_bilgisi_goruntule(ogrenciHead, logHead);
        } else if (secim == 6) {
             typedef struct {
        STUDENT *ogrenciHead;
        BORROW_LOG *logHead;
    } OgrLogArg;

    OgrLogArg arg = {ogrenciHead, logHead};
    listFunc = teslim_etmemis_ogrencileri_listele_fp;
    listFunc((void*)&arg);
        } else if (secim == 7) {
            listFunc = cezali_ogrencileri_listele_fp;
    listFunc((void*)ogrenciHead);
        } else if (secim == 8) {
            kitap_ekleme(&kitapHead);
        } else if (secim == 9) {
            kitap_sil(&kitapHead);
        } else if (secim == 10) {
            kitap_guncelle(kitapHead);
        } else if (secim == 11) {
            kitap_bilgisi_goruntule(kitapHead);
        } else if (secim == 12) {
             listFunc = raftaki_kitaplari_listele_fp;
    listFunc((void*)kitapHead);
        } else if (secim == 13) {
            yazar_ekle(&yazarHead, &count);
        } else if (secim == 14) {
            yazar_sil(&yazarHead);
        } else if (secim == 15) {
            yazar_guncelle(yazarHead);
        } else if (secim == 16) {
         listFunc = yazar_listele_fp;
    listFunc((void*)yazarHead);
        } else if (secim == 17) {
            kitap_yazar_eslestir(kitapHead, yazarHead);
        } else if (secim == 18) {
            kitabin_yazarini_guncelle(liste, eleman_sayisi);
        } else if (secim == 19) {
            yazar_bilgisi_goruntule(yazarHead, liste, eleman_sayisi);
        } else if (secim == 20) {
            kitap_odunc_islemi(ogrenciHead, kitapHead, &logHead, "KitapOdunc.csv");
        } else if (secim == 21) {
            zamaninda_teslim_edilmeyenleri_listele(logHead);
        } else if (secim == 0) {
            printf("Programdan çýkýlýyor...\n");
        } else {
            printf("Geçersiz seçim! Tekrar deneyin.\n");
        }
    }


    YAZAR *yazar_tmp = yazarHead;
    while (yazar_tmp != NULL) {
        YAZAR *temp = yazar_tmp;
        yazar_tmp = yazar_tmp->next;
        free(temp);
    }

    STUDENT *ogr_tmp = ogrenciHead;
    while (ogr_tmp != NULL) {
        STUDENT *temp = ogr_tmp;
        ogr_tmp = ogr_tmp->next;
        free(temp);
    }

    BOOK *kitap_tmp = kitapHead;
    while (kitap_tmp != NULL) {
        BOOK_COPY *copy = kitap_tmp->copiesHead;
        while (copy != NULL) {
            BOOK_COPY *temp_copy = copy;
            copy = copy->next;
            free(temp_copy);
        }
        BOOK *temp = kitap_tmp;
        kitap_tmp = kitap_tmp->next;
        free(temp);
    }

    BORROW_LOG *log_tmp = logHead;
    while (log_tmp != NULL) {
        BORROW_LOG *temp = log_tmp;
        log_tmp = log_tmp->next;
        free(temp);
    }

    if (liste) {
        free(liste);
    }

    return 0;
}
