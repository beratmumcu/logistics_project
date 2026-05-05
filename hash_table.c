#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"

HashTable* create_hash_table(int size) {
    /* Tablo yapısı için RAM'de yer ayır */
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if (!ht) {
        printf("HATA: Hash tablosu icin bellek ayrilamadi!\n");
        return NULL;
    }
    
    ht->table_size = size;
    
    /* Tablodaki bucket'lar (bağlı liste başları) için pointer dizisi ayır */
    ht->buckets = (HashNode**)malloc(sizeof(HashNode*) * size);
    if (!ht->buckets) {
        printf("HATA: Hash bucket'lari icin bellek ayrilamadi!\n");
        free(ht);
        return NULL;
    }
    
    /* Tüm başlangıç pointer'larını güvenli olması için NULL yapıyoruz */
    for (int i = 0; i < size; i++) {
        ht->buckets[i] = NULL;
    }
    
    return ht;
}

int hash_function(int package_id, int table_size) {
    /* 
     * TİP B HASH FONKSİYONU: ÇARPIM YÖNTEMİ (Multiplication Method)
     * h(k) = floor( m * (k*A mod 1) )
     * A: Altın oran (Knuth tarafından önerilir) = (sqrt(5) - 1) / 2 = 0.618033...
     */
    const float A = 0.6180339887f;
    float val = package_id * A;
    val = val - (int)val; /* Sadece ondalıklı kısmı alıyoruz (k*A mod 1) */
    
    return (int)(table_size * val);
}

void insert_package(HashTable* ht, Package* pkg) {
    if (!ht || !pkg) return;
    
    /* İndeksi hesapla */
    int index = hash_function(pkg->package_id, ht->table_size);
    
    /* Eklenecek veriler için Heap alanında yeni bir Node oluştur (Dinamik Bellek) */
    HashNode* new_node = (HashNode*)malloc(sizeof(HashNode));
    if (!new_node) {
        printf("HATA: Yeni Hash Node icin bellek ayrilamadi!\n");
        return;
    }
    new_node->package = pkg;
    new_node->next = NULL;
    
    /* ÇAKIŞMA (COLLISION) KONTROLÜ VE ZİNCİRLEME (CHAINING) */
    if (ht->buckets[index] == NULL) {
        /* Kova boşsa direkt ilk eleman olarak ekle */
        ht->buckets[index] = new_node;
    } else {
        /* Çakışma var! Listenin sonuna kadar git ve oraya ekle */
        HashNode* current = ht->buckets[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

Package* search_package(HashTable* ht, int package_id) {
    if (!ht) return NULL;
    
    /* Arama O(1) maliyetindedir, paketin bulunduğu kovaya doğrudan gidilir */
    int index = hash_function(package_id, ht->table_size);
    HashNode* current = ht->buckets[index];
    
    /* O(1+a) - Eğer çakışma olduysa o kova içindeki bağlı listede arama yap */
    while (current != NULL) {
        if (current->package->package_id == package_id) {
            return current->package; /* Paket bulundu, pointer'ını döndür */
        }
        current = current->next;
    }
    
    return NULL; /* Paket bulunamadı */
}

void delete_package(HashTable* ht, int package_id) {
    if (!ht) return;
    
    int index = hash_function(package_id, ht->table_size);
    HashNode* current = ht->buckets[index];
    HashNode* prev = NULL;
    
    while (current != NULL) {
        if (current->package->package_id == package_id) {
            /* Silinecek düğüm bulundu, zincir koptuğu yerden birbirine bağlanmalı */
            if (prev == NULL) {
                /* Düğüm listenin en başındaysa */
                ht->buckets[index] = current->next;
            } else {
                /* Düğüm listenin ortasında veya sonundaysa */
                prev->next = current->next;
            }
            
            /* Düğümü hafızadan sil (Sadece Node silinir, paketin kendisi Graf için de lazım olabilir) */
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void print_hash_table(HashTable* ht) {
    if (!ht) return;
    
    printf("\n========== HASH TABLOSU BELLEK IZLEME RAPORU ==========\n");
    for (int i = 0; i < ht->table_size; i++) {
        HashNode* current = ht->buckets[i];
        if (current != NULL) {
            printf("Bucket [%02d]: ", i);
            while (current != NULL) {
                /* Node'un ve Paketin Bellek Adreslerini yazdır (İstenen 0x... formatı) */
                printf("[Paket ID: %d | Node Adres: %p | Paket Adres: %p] -> ", 
                       current->package->package_id, 
                       (void*)current, 
                       (void*)current->package);
                current = current->next;
            }
            printf("NULL\n");
        }
    }
    printf("=======================================================\n");
}

void free_hash_table(HashTable* ht) {
    if (!ht) return;
    
    for (int i = 0; i < ht->table_size; i++) {
        HashNode* current = ht->buckets[i];
        while (current != NULL) {
            HashNode* temp = current;
            current = current->next;
            free(temp); /* Node'u bellekten iade et */
        }
    }
    free(ht->buckets); /* Bucket pointer dizisini iade et */
    free(ht);          /* Tabloyu iade et */
}
