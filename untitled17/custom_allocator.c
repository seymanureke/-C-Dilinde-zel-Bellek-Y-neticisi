#include "custom_allocator.h"
#include <stdio.h>  // İleride loglama ve test çıktısı almak için
void initialize_memory_pool() {
    // Havuzun en başına ilk Header yerleştiriliyor
    free_list_head = (BlockHeader*)memory_pool;

    // Bu bloğun boyutu: Toplam havuz boyutu - Header'ın kendi kapladığı alan
    free_list_head->size = POOL_SIZE - sizeof(BlockHeader);
    free_list_head->is_free = 1; // Başlangıçta tamamen boş
    free_list_head->next = NULL; // Kendisinden sonra başka blok yok
}
void* my_malloc(size_t size) {
    // 1. Havuz henüz hazır değilse ilklendir
    if (free_list_head == NULL) {
        initialize_memory_pool();
    }

    BlockHeader* current = free_list_head;

    // 2. Uygun bir boş blok bulana kadar listeyi tara
    while (current != NULL) {
        if (current->is_free && current->size >= size) {

            // 3. Blok bölme (Splitting) yapılabilir mi?
            // Kalan alan, yeni bir Header ve en az 1 bayt veri tutabilecek kadar büyükse bölüyoruz.
            if (current->size >= size + sizeof(BlockHeader) + 1) {
                // Yeni boş bloğun başlayacağı adresi hesapla (Pointer aritmetiği)
                // char* tipine cast ediyoruz ki bayt bayt ilerleyebilelim
                BlockHeader* next_block = (BlockHeader*)((char*)current + sizeof(BlockHeader) + size);

                // Yeni boş bloğun değerlerini ata
                next_block->size = current->size - size - sizeof(BlockHeader);
                next_block->is_free = 1;
                next_block->next = current->next;

                // Mevcut bloğu güncelle
                current->size = size;
                current->next = next_block;
            }

            // Bloğu dolu olarak işaretle
            current->is_free = 0;

            // 4. Kullanıcıya HEADER'ın adresini değil, veri alanının adresini dön!
            // Header'ın bittiği yer = current pointer'ına sizeof(BlockHeader) kadar bayt eklenmiş hali
            return (void*)((char*)current + sizeof(BlockHeader));
        }
        current = current->next;
    }

    // Uygun yer bulunamadıysa (Out of memory) NULL dön
    return NULL;
}
void my_free(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    // 1. Kullanıcının verdiği veri adresinden Header adresine geri dönüyoruz
    BlockHeader* block = (BlockHeader*)((char*)ptr - sizeof(BlockHeader));

    // 2. Bloğu serbest bırak
    block->is_free = 1;

    // 3. Coalescing (Ardışık boş blokları birleştirme)
    BlockHeader* current = free_list_head;
    while (current != NULL && current->next != NULL) {
        // Eğer mevcut blok boşsa VE bir sonraki blok da boşsa
        if (current->is_free && current->next->is_free) {
            // Bir sonraki bloğun boyutunu ve header alanını mevcut bloğa ekle
            current->size += sizeof(BlockHeader) + current->next->size;
            // Bir sonraki bloğu listeden atla (bağlantıyı güncelle)
            current->next = current->next->next;
        } else {
            // Eğer birleşme olmadıysa bir sonraki bloğa geç
            current = current->next;
        }
    }
}
void visualize_memory() {
    BlockHeader* current = free_list_head;
    int block_id = 1;

    printf("\n=== ANLIK BELLEK HAVUZU DURUMU ===\n");
    if (current == NULL) {
        printf("Bellek havuzu henüz ilklendirilmedi.\n");
        return;
    }

    while (current != NULL) {
        printf("[Blok %d] -> Adres: %p | Boyut: %zu Bayt | Durum: %s\n",
               block_id++,
               (void*)current,
               current->size,
               current->is_free ? "BOŞ 🟩" : "DOLU 🟥");
        current = current->next;
    }
    printf("==================================\n\n");
}