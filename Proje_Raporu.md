# Lojistik Ağı Yönetim Sistemi - Proje Raporu

Bu rapor, üniversite veri yapıları (Data Structures) dersi gereksinimleri doğrultusunda geliştirilen **Lojistik Ağı** projesinin mimari özelliklerini ve bellek içi (RAM) organizasyonunu içermektedir.

## 1. Projenin Amacı ve Temel İsterler
Bu projenin amacı, kargoların ve kargo rotalarının yönetimini sağlamaktır. Sistemde büyük veri kümeleri üzerinde **O(1)** arama performansı sağlamak için **Hash Tablosu (Hash Table)**, lojistik şehir ağı rotalaması için ise **Graf (Graph)** yapısı tercih edilmiştir. İsterlerde belirtildiği gibi;
- **Hash Fonksiyonu** olarak Tip B (Çarpım Yöntemi) tasarlanmıştır.
- **Çakışma (Collision)** çözümü olarak Zincirleme (Linked List) kullanılmıştır.
- **Graf Gezinimi** işlemleri DFS (Depth-First Search) ile çözümlenmiştir.
- Programın başından sonuna kadar **Dinamik Bellek Kullanımı (`malloc` / `free`)** katı bir şekilde uygulanmıştır.

---

## 2. Mimari ve Bileşenler

Proje üç ana katmandan oluşmaktadır (`types.h`, `hash_table.h/.c`, `graph.h/.c`, `main.c`).

### 2.1. Hash Tablosu ve Zincirleme
Hash tablosu, paketlerin ID numaralarına göre anında bulunabilmesini sağlar.
* **Hash Fonksiyonu (Çarpım Yöntemi)**: Altın oran sabiti (`A = 0.618033...`) kullanılarak Paket ID'sinin rastgele dağılımı sağlanır ve tabloya ekleneceği İndeks (Bucket ID) hesaplanır.
* **Zincirleme (Chaining)**: Çakışan paket ID'lerinin silinmesi veya açık adreslemeye girmesi yerine, tabloda o indiste yer alan `HashNode` isimli bir **Bağlı Liste (Linked List)** düğümüne dinamik olarak eklenir.

```c
typedef struct HashNode {
    Package* package;      // Paketin bellekteki dinamik adresi
    struct HashNode* next; // Sonraki çakışan pakete olan işaretçi (Zincirleme)
} HashNode;
```

### 2.2. Graf ve DFS Algoritması
Şehirler arası mesafeler ve lojistik ağı Komşuluk Listesi (Adjacency List) kullanılarak modellenmiştir.
* **Komşuluk Listesi**: Bir matris (N x N) oluşturmak yerine bağlı listeler kullanılması, bellek tüketiminde (Memory Optimization) büyük fayda sağlar. Yalnızca var olan rotalar için bellekte (`malloc` ile) yer açılır.
* **Derinlik Öncelikli Arama (DFS)**: Graf yapısı içinde sistem, belirtilen şehirden başlayarak gidebileceği en derine (rekürsif fonksiyonlarla) inerek ağdaki tüm şehirleri ziyaret eder. Ziyaret durumu `visited` değişkenleriyle kontrol edilerek sonsuz döngüler (Cycle) önlenmiştir.

---

## 3. Bellek İzleme ve Pointer Yönetimi

Sistemin en önemli isterlerinden biri **Bellek İzleme Raporu**'nun sunulmasıydı. Bunun için oluşturulan `print_hash_table()` ve `print_graph()` fonksiyonları ile oluşturulan her bir yapının RAM adresleri Hexadecimal (Onaltılık - `0x...`) formatında takip edilebilir durumdadır:

- **Paketler**, Hash node'larından ayrık bir şekilde bellekte tahsis edilir (`Package*`).
- **Hash Zincirlerindeki** her çakışma, yepyeni bir pointer adresini (`HashNode*`) beraberinde getirir.
- **Graf Düğümleri (Şehirler)**, `Graph->cities` dizisinde `CityNode*` olarak depolanır.
- **Rotalar (Yollar)**, her bir şehirden çıkan bir linked list elemanı olan `EdgeNode*` olarak RAM'e yerleşir.

*Bu güçlü ayrım sayesinde, program kapanmadan önce çağrılan `free()` fonksiyonlarıyla bellek sızıntıları (Memory Leak) tamamen önlenmektedir.*
