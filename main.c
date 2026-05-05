#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Kendi oluşturduğumuz modüllerin header dosyaları */
#include "types.h"
#include "hash_table.h"
#include "graph.h"

int main() {
    printf("=========================================================\n");
    printf("     LOJISTIK AGI VE KARGO YONETIM SISTEMI BASLATILDI    \n");
    printf("=========================================================\n\n");

    /* =========================================================
     * 1. GRAF (LOJİSTİK AĞI) OLUŞTURMA
     * ========================================================= */
    int num_cities = 5;
    Graph* graph = create_graph(num_cities);
    
    /* Dinamik Şehirleri Ekleme (ID: 0-4) */
    add_city(graph, 0, "Istanbul");
    add_city(graph, 1, "Ankara");
    add_city(graph, 2, "Izmir");
    add_city(graph, 3, "Bursa");
    add_city(graph, 4, "Antalya");
    
    /* Şehirler Arası Rota Bağlantıları (Edge Ekleme - Çift Yönlü) */
    /* Istanbul <-> Ankara (Mesafe: 450 km) */
    add_edge(graph, 0, 1, 450);
    add_edge(graph, 1, 0, 450);
    /* Istanbul <-> Izmir (Mesafe: 480 km) */
    add_edge(graph, 0, 2, 480);
    add_edge(graph, 2, 0, 480);
    /* Istanbul <-> Bursa (Mesafe: 150 km) */
    add_edge(graph, 0, 3, 150);
    add_edge(graph, 3, 0, 150);
    /* Ankara <-> Antalya (Mesafe: 500 km) */
    add_edge(graph, 1, 4, 500);
    add_edge(graph, 4, 1, 500);
    /* Izmir <-> Antalya (Mesafe: 460 km) */
    add_edge(graph, 2, 4, 460);
    add_edge(graph, 4, 2, 460);

    /* =========================================================
     * 2. HASH TABLOSU OLUŞTURMA (Zincirleme/Collision Testi)
     * ========================================================= */
    /* Çakışma (Collision) garantisi olsun ve zincirleme mekanizması test 
       edilsin diye Tablo Boyutu kasıtlı olarak küçük (3) seçildi. */
    int hash_size = 3; 
    HashTable* ht = create_hash_table(hash_size);
    
    /* Dinamik Paketler Oluşturma (Heap Alanı Kullanımı) */
    Package* p1 = (Package*)malloc(sizeof(Package));
    p1->package_id = 101; strcpy(p1->city_name, "Istanbul"); strcpy(p1->content_info, "Elektronik Esya"); p1->weight = 2.5f;

    Package* p2 = (Package*)malloc(sizeof(Package));
    p2->package_id = 102; strcpy(p2->city_name, "Ankara"); strcpy(p2->content_info, "Kitap Kolisi"); p2->weight = 8.0f;

    Package* p3 = (Package*)malloc(sizeof(Package));
    p3->package_id = 103; strcpy(p3->city_name, "Izmir"); strcpy(p3->content_info, "Kiyafet"); p3->weight = 3.2f;

    Package* p4 = (Package*)malloc(sizeof(Package));
    p4->package_id = 104; strcpy(p4->city_name, "Antalya"); strcpy(p4->content_info, "Gida Urunleri"); p4->weight = 15.0f;

    Package* p5 = (Package*)malloc(sizeof(Package));
    p5->package_id = 105; strcpy(p5->city_name, "Bursa"); strcpy(p5->content_info, "Oto Yedek Parca"); p5->weight = 45.0f;

    /* Paketleri Hash Tablosuna Ekle (Zincirleme yöntemi devrede) */
    insert_package(ht, p1);
    insert_package(ht, p2);
    insert_package(ht, p3);
    insert_package(ht, p4);
    insert_package(ht, p5);

    /* =========================================================
     * 3. İSTENEN BELLEK İZLEME RAPORLARI (RAM ADRESLERİ: 0x...)
     * ========================================================= */
    printf("\n>>> PROJE ISTERI: SISTEM BELLEK IZLEME RAPORLARI <<<\n");
    print_hash_table(ht);
    print_graph(graph);

    /* =========================================================
     * 4. SENARYO: PAKET BULMA VE GRAFTA ROTA (DFS) CIKARMA
     * ========================================================= */
    int search_id = 104; /* Antalya'daki paket aransın */
    printf("\n>>> SENARYO: %d ID'LI PAKETIN BULUNMASI VE ROTA CIKARIMI <<<\n", search_id);
    
    /* Hash tablosundan paketi O(1+a) hızında anında bul! */
    Package* found = search_package(ht, search_id);
    if (found) {
        printf("[BASARILI] Paket Bulundu! (Hash O(1+a) erisimi)\n");
        printf("   - Icerik: %s\n   - Agirlik: %.1f kg\n   - Bulundugu Sehir: %s\n", 
               found->content_info, found->weight, found->city_name);
        
        /* Bulunan şehrin ID'sini graf sistemimizde arayalım */
        int start_id = -1;
        for (int i = 0; i < graph->total_cities; i++) {
            if (graph->cities[i] != NULL && strcmp(graph->cities[i]->city_name, found->city_name) == 0) {
                start_id = graph->cities[i]->city_id;
                break;
            }
        }
        
        /* Bulunan şehirden itibaren tüm lojistik ağını Derinlik Öncelikli (DFS) tara */
        if (start_id != -1) {
            printf("\n[%s] sehrinden baslayarak DFS Lojistik Rota Agi Taramasi baslatiliyor:", found->city_name);
            dfs_traverse(graph, start_id);
        } else {
            printf("\nPaketin bulundugu sehir lojistik agimizda tanimli degil!\n");
        }
    } else {
        printf("[HATA] Paket sistemde bulunamadi!\n");
    }

    /* =========================================================
     * 5. BELLEK TEMİZLEME (Memory Leak Önleme / Temiz RAM)
     * ========================================================= */
    printf("\n>>> SISTEM KAPATILIYOR: BELLEK (RAM) IADE EDILIYOR <<<\n");
    
    /* Modüllerin içerisindeki Node'ları sil */
    free_graph(graph);
    free_hash_table(ht);
    
    /* Main'de ayırdığımız asıl paketleri sil */
    free(p1); free(p2); free(p3); free(p4); free(p5);
    
    printf("Tüm bellek tahsisleri basariyla sisteme iade edildi (0 memory leak).\n");
    printf("=========================================================\n");
    
    system("pause");
    return 0;
}
