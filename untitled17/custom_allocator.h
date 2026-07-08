#ifndef CUSTOM_ALLOCATOR_H
#define CUSTOM_ALLOCATOR_H

#include <stddef.h> // size_t yapısı için

// Bellek havuzumuzun toplam boyutu (Örn: 64 KB)
#define POOL_SIZE 65536

// Her bellek bloğunun başında yer alacak metaveri (Header)
typedef struct BlockHeader {
    size_t size;               // Bloğun bayt cinsinden boyutu (Header hariç)
    int is_free;               // 1 ise boş, 0 ise dolu
    struct BlockHeader* next;  // Bir sonraki bloğun adresi (Bağlı Liste)
} BlockHeader;

// İşletim sisteminden kopardığımız büyük statik bellek havuzu
static char memory_pool[POOL_SIZE];

// Bağlı listemizin başlangıç noktası (Head)
static BlockHeader* free_list_head = NULL;

// ... önceki kodlar (struct tanımlamaları vs.) ...

// Fonksiyon Prototipleri
void initialize_memory_pool();
void* my_malloc(size_t size);
void my_free(void* ptr);
void visualize_memory();

#endif // CUSTOM_ALLOCATOR_H

