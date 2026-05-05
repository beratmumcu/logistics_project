#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

Graph* create_graph(int total_cities) {
    /* Graf ana yapısı için bellek tahsisi */
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) return NULL;
    
    graph->total_cities = total_cities;
    
    /* Şehirlerin (Vertex) listesini tutacak dinamik pointer dizisi */
    graph->cities = (CityNode**)malloc(sizeof(CityNode*) * total_cities);
    if (!graph->cities) {
        free(graph);
        return NULL;
    }
    
    /* Başlangıçta tüm şehir alanlarını NULL ile güvenli hale getir */
    for (int i = 0; i < total_cities; i++) {
        graph->cities[i] = NULL;
    }
    
    return graph;
}

void add_city(Graph* graph, int city_id, const char* city_name) {
    /* Hatalı bir ID veya null graf gelirse işlemi iptal et */
    if (!graph || city_id >= graph->total_cities || city_id < 0) return;
    
    /* Yeni Şehir Düğümü (CityNode) için dinamik alan oluştur */
    CityNode* new_city = (CityNode*)malloc(sizeof(CityNode));
    if (!new_city) return;
    
    new_city->city_id = city_id;
    strncpy(new_city->city_name, city_name, MAX_CITY_NAME - 1);
    new_city->city_name[MAX_CITY_NAME - 1] = '\0';
    new_city->edge_head = NULL; /* Henüz bir bağlantısı yok */
    new_city->visited = 0;      /* DFS için ziyaret edilmedi işareti */
    
    /* Şehri diziye ekle */
    graph->cities[city_id] = new_city;
}

void add_edge(Graph* graph, int src_city_id, int dest_city_id, int distance) {
    /* Güvenlik kontrolü */
    if (!graph || src_city_id >= graph->total_cities || dest_city_id >= graph->total_cities) return;
    if (src_city_id < 0 || dest_city_id < 0) return;
    
    CityNode* src_city = graph->cities[src_city_id];
    if (!src_city) return; /* Çıkış yapılacak şehir yaratılmamışsa dur */
    
    /* Kenar (Bağlantı/Rota) için yeni bir EdgeNode oluştur */
    EdgeNode* new_edge = (EdgeNode*)malloc(sizeof(EdgeNode));
    if (!new_edge) return;
    
    new_edge->dest_city_id = dest_city_id;
    new_edge->distance = distance;
    
    /* Bağlı listeye (Komşuluk Listesi) eleman ekleme (O(1) için Başa Ekleme) */
    new_edge->next = src_city->edge_head;
    src_city->edge_head = new_edge;
}

/* DFS için ziyaret durumlarını sıfırlayan yardımcı fonksiyon */
static void reset_visited(Graph* graph) {
    if (!graph) return;
    for (int i = 0; i < graph->total_cities; i++) {
        if (graph->cities[i]) {
            graph->cities[i]->visited = 0;
        }
    }
}

/* Derinlik Öncelikli Arama (DFS) Yordamı (Rekürsif) */
static void dfs_recursive(Graph* graph, int current_city_id) {
    CityNode* city = graph->cities[current_city_id];
    
    /* Eğer düğüm null ise veya zaten ziyaret edilmişse (döngüyü kırmak için) geri dön */
    if (!city || city->visited) return;
    
    /* 1. Adım: Şehri Ziyaret Et (Visited işaretini 1 yap) */
    city->visited = 1;
    printf("[DFS Ziyareti] -> Sehrine ulasildi: %s (ID: %d)\n", city->city_name, city->city_id);
    
    /* 2. Adım: O şehre bağlı komşu rotaları incele */
    EdgeNode* edge = city->edge_head;
    while (edge != NULL) {
        int next_city = edge->dest_city_id;
        
        /* Gidilecek şehir daha önce ziyaret EDİLMEDİYSE oraya doğru derinleş (DFS) */
        if (graph->cities[next_city] && !graph->cities[next_city]->visited) {
            printf("  Lojistik Kamyonu %s sehrinden %s sehrine ilerliyor... (Mesafe: %d km)\n", 
                   city->city_name, graph->cities[next_city]->city_name, edge->distance);
                   
            /* Özyinelemeli (Recursive) Çağrı */
            dfs_recursive(graph, next_city);
        }
        edge = edge->next; /* Bir sonraki bağlantıya geç */
    }
}

void dfs_traverse(Graph* graph, int start_city_id) {
    if (!graph || start_city_id >= graph->total_cities || start_city_id < 0) return;
    
    printf("\n============== DFS LOJISTIK ROTA GEZINIMI ==============\n");
    reset_visited(graph); /* DFS öncesi geçmiş gezinmelerin izlerini temizle */
    
    /* Eğer istenirse başlangıç şehri ekrana basılabilir, biz direkt recursive çağırıyoruz */
    dfs_recursive(graph, start_city_id);
    printf("========================================================\n");
}

void print_graph(Graph* graph) {
    if (!graph) return;
    
    printf("\n========= GRAF (KOMSULUK LISTESI) BELLEK RAPORU =========\n");
    for (int i = 0; i < graph->total_cities; i++) {
        CityNode* city = graph->cities[i];
        if (city) {
            /* Şehrin (CityNode) RAM'deki yeri */
            printf("Sehir: %-15s | Düğüm Adresi: %p\n", city->city_name, (void*)city);
            
            EdgeNode* edge = city->edge_head;
            while (edge != NULL) {
                /* Şehre bağlı rotaların (EdgeNode) RAM'deki yeri ve detayı */
                printf("    +----( %3d km )----> [Hedef ID: %d] | Baglanti Adresi: %p\n", 
                       edge->distance, edge->dest_city_id, (void*)edge);
                edge = edge->next;
            }
        }
    }
    printf("=========================================================\n");
}

void free_graph(Graph* graph) {
    if (!graph) return;
    
    /* Her bir şehri ve içindeki bağlantı listelerini dolaşıp serbest bırak (Memory Leak önlemi) */
    for (int i = 0; i < graph->total_cities; i++) {
        CityNode* city = graph->cities[i];
        if (city) {
            EdgeNode* edge = city->edge_head;
            while (edge != NULL) {
                EdgeNode* temp = edge;
                edge = edge->next;
                free(temp); /* Kenarı (Yolu) yok et */
            }
            free(city); /* Şehri yok et */
        }
    }
    free(graph->cities); /* Dinamik şehir dizisini yok et */
    free(graph);         /* Grafı yok et */
}
