#ifndef GRAPH_H
#define GRAPH_H

#include "types.h"

/* 
 * Belirtilen kapasitede dinamik bir Graf (Lojistik Ağı) oluşturur.
 * total_cities: Lojistik ağındaki maksimum şehir kapasitesi
 */
Graph* create_graph(int total_cities);

/* 
 * Grafa yeni bir şehir (Vertex / Düğüm) ekler.
 * Şehir ID'sini ve ismini kullanarak Komşuluk Listesindeki (Adjacency List) başvuru noktasını başlatır.
 */
void add_city(Graph* graph, int city_id, const char* city_name);

/* 
 * İki şehir arasında yönlü/yönsüz bir bağlantı (Kenar / Edge) oluşturur.
 * O(1) hızında başa ekleme (prepend) yöntemi ile çalışır.
 * Lojistik ağlarında yollar gidiş-dönüş olduğundan, çift yönlü yol için iki kez çağrılmalıdır.
 */
void add_edge(Graph* graph, int src_city_id, int dest_city_id, int distance);

/* 
 * DFS (Depth-First Search) algoritması kullanarak lojistik ağı üzerinde Derinlik Öncelikli gezinme yapar.
 * Verilen başlangıç şehrinden itibaren gidilebilecek tüm rotaları ve mesafeleri ekrana yazdırır.
 */
void dfs_traverse(Graph* graph, int start_city_id);

/* 
 * İstenen "Bellek İzleme Raporu" gereksinimi doğrultusunda, Graf içindeki her bir şehrin 
 * ve o şehirden çıkan bağlantıların (Edge) dinamik bellek adreslerini (0x...) ekrana yazdırır.
 */
void print_graph(Graph* graph);

/* 
 * Dinamik olarak (malloc ile) tahsis edilmiş tüm şehirleri, 
 * yolları (kenarları) ve graf yapısını sisteme (free ile) iade eder.
 */
void free_graph(Graph* graph);

#endif /* GRAPH_H */
