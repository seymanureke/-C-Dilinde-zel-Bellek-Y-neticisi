# -C-Dilinde-özel-Bellek-Yöneticisi
# C Dilinde Özel Bellek Yöneticisi (Custom Memory Allocator)

Bu proje, C dilinde standart `malloc` ve `free` fonksiyonlarının temel çalışma mantığını simüle eden, platform bağımsız ve hafif bir özel bellek yönetimi kütüphanesidir. Önceden ayrılmış sabit bir bellek havuzu kullanarak; ileri seviye pointer aritmetiği, bağlı liste (linked list) ile blok yönetimi ve dinamik bellek optimizasyonu tekniklerini uygulamalı olarak göstermektedir.



## Özellikler

* **Sabit Boyutlu Bellek Havuzu:** İşletim sistemine özgü sistem çağrılarına (`brk`/`sbrk`) bağımlı kalmadan, maksimum taşınabilirlik için statik olarak ayrılmış 64 KB'lık bir dizi (`char memory_pool`) üzerinde çalışır.
* **First-Fit Tahsis Stratejisi:** Bellek isteklerinde bağlı listeyi baştan sona sırayla tarar ve istenen boyutu karşılayan ilk uygun boş bloğu seçer.
* **Blok Bölme (Splitting):** Büyük bir boş bloğun kullanıcıya yetecek kadarı ayrıldıktan sonra, geriye kalan alan yeni bir boş blok (Header + veri alanı) oluşturabilecek büyüklükteyse blok ikiye bölünerek bellek israfı önlenir.
* **Anında Birleştirme (Coalescing):** `my_free` fonksiyonu çağrıldığında, yan yana denk gelen ardışık boş bloklar otomatik olarak tek bir büyük blok halinde birleştirilir; böylece dış bellek parçalanmasının (fragmentation) önüne geçilir.
* **Bellek Görselleştirme Aracı:** Dahili `visualize_memory` fonksiyonu sayesinde bellek havuzundaki tüm blokların anlık durumunu, adreslerini ve boyutlarını terminal üzerinden gerçek zamanlı takip etmenizi sağlar.

---

##  Mimari ve Veri Yapıları

Bellek havuzundaki her bir veri bloğunun önünde, o bloğun metaverilerini tutan bir `BlockHeader` yapısı yer alır. Bu başlıklar bellek içinde bir tek yönlü bağlı liste (Singly Linked List) oluşturur.

```c
typedef struct BlockHeader {
    size_t size;               // Blok içindeki kullanılabilir veri alanı boyutu (bayt)
    int is_free;               // Bloğun durumu (1: BOŞ, 0: DOLU)
    struct BlockHeader* next;  // Bir sonraki blok başlığının adresi
} BlockHeader;
