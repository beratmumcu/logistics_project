#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "types.h"

/* 
 * Dinamik olarak Hash Tablosu oluşturur. 
 * size: Tablonun boyutu (Bucket sayısı)
 * Dönüş: Oluşturulan tablonun bellek adresi
 */
HashTable* create_hash_table(int size);

/* 
 * Tip B Hash Fonksiyonu: Çarpım Yöntemi (Multiplication Method)
 * Paket ID'sini alıp tablodaki indeksini (Bucket ID) hesaplar.
 */
int hash_function(int package_id, int table_size);

/* 
 * Hash tablosuna yeni bir paket ekler.
 * Çakışma durumunda bağlı listenin sonuna (Zincirleme) ekleme yapar.
 */
void insert_package(HashTable* ht, Package* pkg);

/* 
 * Paket ID'sine göre Hash tablosunda O(1) veya O(1+a) hızında arama yapar.
 * Bulursa Paketin pointer'ını, bulamazsa NULL döndürür.
 */
Package* search_package(HashTable* ht, int package_id);

/* 
 * Belirtilen ID'ye sahip paketi Hash tablosundan siler.
 * Zincirleme listesinden düğümü kopartıp serbest bırakır (free).
 */
void delete_package(HashTable* ht, int package_id);

/* 
 * İstenen "Bellek İzleme Raporu" gereksinimi için 
 * tabloyu, düğümleri ve RAM adreslerini (0x...) ekrana yazdırır.
 */
void print_hash_table(HashTable* ht);

/* 
 * Program bitiminde Hash tablosuna tahsis edilen 
 * tüm dinamik belleği sisteme iade eder (Memory Leak önleme).
 */
void free_hash_table(HashTable* ht);

#endif /* HASH_TABLE_H */
