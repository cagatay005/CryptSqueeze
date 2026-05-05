#ifndef LZ77_HPP
#define LZ77_HPP

#include <vector>
#include <cstdint>

// Sıkıştırma işlemleri için oluşturulmuş isim uzayı
namespace KriptoSikistirma {

    // LZ77 algoritmasında kullanılan temel veri yapısı (Mesafe, Uzunluk, Sonraki Karakter)
    struct Lz77Dugumu {
        uint16_t geriyeGidisMesafesi; // Eşleşmenin ne kadar geride olduğu (Offset)
        uint16_t eslesmeUzunlugu;     // Eşleşen kısmın uzunluğu (Length)
        uint8_t  sonrakiKarakter;     // Eşleşmeden sonra gelen ilk ham karakter
    };

    class Lz77Motoru {
    public:
        // Ham veriyi LZ77 algoritması ile küçülten fonksiyon
        static std::vector<Lz77Dugumu> sikistir(const std::vector<uint8_t>& hamVeri);

        // Sıkıştırılmış veriyi (düğümleri) geri açan fonksiyon
        static std::vector<uint8_t> geriAc(const std::vector<Lz77Dugumu>& sikistirilmisVeri);
    };

}

#endif