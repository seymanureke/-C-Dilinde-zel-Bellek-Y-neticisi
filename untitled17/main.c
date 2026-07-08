#include <stdio.h>
#include "custom_allocator.h"

int main() {
    printf("--- Bellek Yönetim Sistemi Testi Başlıyor ---\n");

    // 1. Havuzun ilk durumunu görelim
    visualize_memory();

    // 2. Üç farklı bellek bloğu isteyelim
    printf("100, 250 ve 500 baytlık alanlar isteniyor...\n");
    int* dizi1 = (int*)my_malloc(100);
    char* metin = (char*)my_malloc(250);
    double* dizi2 = (double*)my_malloc(500);

    // Belleğin durumunu kontrol edelim (Bloklar bölünmüş olmalı)
    visualize_memory();

    // 3. Ortadaki bloğu serbest bırakalım (Parçalanma - Fragmentation oluşacak)
    printf("Ortadaki 250 baytlık alan (metin) serbest bırakılıyor...\n");
    my_free(metin);
    visualize_memory();

    // 4. Son bloğu da serbest bırakalım (Ardışık boş bloklar birleşmeli - Coalescing)
    printf("Son 500 baytlık alan (dizi2) serbest bırakılıyor...\n");
    my_free(dizi2);
    visualize_memory();

    // 5. İlk bloğu da bırakınca havuz tamamen eski orijinal haline dönmeli
    printf("İlk 100 baytlık alan (dizi1) serbest bırakılıyor...\n");
    my_free(dizi1);
    visualize_memory();

    printf("--- Test Başarıyla Tamamlandı ---\n");
    return 0;
}