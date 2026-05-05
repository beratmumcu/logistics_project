#ifndef TYPES_H
#define TYPES_H

#define MAX_CITY_NAME 50
#define MAX_CONTENT_INFO 100

/* =========================================
 * 1. PAKET YAPISI (Veri Modeli)
 * ========================================= */
typedef struct Package {
    int package_id;                         /* Benzersiz paket numarası (Hash Key olarak kullanılacak) */
    char city_name[MAX_CITY_NAME];          /* Paketin şu an bulunduğu veya gideceği şehir */
    char content_info[MAX_CONTENT_INFO];    /* Paket detay bilgisi / açıklaması */
    float weight;                           /* Paket ağırlığı */
} Package;


/* =========================================
 * 2. HASH TABLOSU YAPILARI (Arama ve Çakışma Çözümü)
 * Çakışma Çözümü: Zincirleme (Chaining - Linked List)
 * ========================================= */
typedef struct HashNode {
    Package* package;                       /* Pakete işaret eden pointer (Dinamik bellek ile yönetilecek) */
    struct HashNode* next;                  /* Çakışma (Collision) durumunda bir sonraki düğüme işaretçi */
} HashNode;

typedef struct HashTable {
    int table_size;                         /* Tablo boyutu (Bucket sayısı) */
    HashNode** buckets;                     /* Bağlı listelerin başlarını tutacak pointer dizisi (HashNode pointer pointer) */
} HashTable;


/* =========================================
 * 3. GRAF YAPILARI (Lojistik Ağı - Şehirler ve Rotalar)
 * Yöntem: Komşuluk Listesi (Adjacency List)
 * Gezinme: DFS (Depth-First Search)
 * ========================================= */

/* Bir şehirden diğer şehre olan bağlantıyı (Kenar - Edge) temsil eden düğüm */
typedef struct EdgeNode {
    int dest_city_id;                       /* Bağlantının gittiği hedef şehrin ID'si veya indeksi */
    int distance;                           /* Şehirler arası mesafe (Ağırlık) */
    struct EdgeNode* next;                  /* Aynı şehirden çıkan bir sonraki bağlantıya işaretçi */
} EdgeNode;

/* Graftaki her bir şehri (Düğüm - Vertex) temsil eden yapı */
typedef struct CityNode {
    int city_id;                            /* Şehrin sistemdeki benzersiz numarası */
    char city_name[MAX_CITY_NAME];          /* Şehrin adı */
    EdgeNode* edge_head;                    /* Bu şehirden diğer şehirlere giden yolların (bağlı liste) başlangıcı */
    int visited;                            /* DFS algoritmasında düğümün ziyaret edilip edilmediğini kontrol bayrağı */
} CityNode;

/* Tüm Grafiği (Lojistik Ağını) yönetecek ana yapı */
typedef struct Graph {
    int total_cities;                       /* Sistemde kayıtlı toplam şehir sayısı */
    CityNode** cities;                      /* Şehirlerin (CityNode) tutulduğu dinamik dizi (CityNode pointer dizisi) */
} Graph;

#endif /* TYPES_H */
