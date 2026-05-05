#include "lz77.hpp"
#include <algorithm>

namespace KriptoSikistirma {

    // Kayan pencere (sliding window) sınır değerleri
    const int ARAMA_TAMPONU_BOYUTU = 4096;
    const int ILERI_BAKIS_TAMPONU_BOYUTU = 18;

    // Ham veriyi LZ77 algoritması ile küçülten fonksiyon[cite: 1]
    std::vector<Lz77Dugumu> Lz77Motoru::sikistir(const std::vector<uint8_t>& hamVeri) {
        std::vector<Lz77Dugumu> sikistirilmisVeri;
        
        if (hamVeri.empty()) {
            return sikistirilmisVeri; // Veri yoksa boş döndür
        }

        size_t mevcutKonum = 0;

        // Tüm veri bitene kadar döngüyü sürdür
        while (mevcutKonum < hamVeri.size()) {
            int enIyiMesafe = 0;
            int enIyiUzunluk = 0;

            // Arama tamponunun sınırlarını belirle
            size_t aramaBaslangici = (mevcutKonum > ARAMA_TAMPONU_BOYUTU) ? (mevcutKonum - ARAMA_TAMPONU_BOYUTU) : 0;

            // İleri bakış tamponunda eşleşme ara
            for (size_t gecmisKonum = aramaBaslangici; gecmisKonum < mevcutKonum; ++gecmisKonum) {
                int eslesmeUzunlugu = 0;

                // Hem ileri bakış sınırı aşılmayacak hem de veri sonuna gelinmeyecek
                while (eslesmeUzunlugu < ILERI_BAKIS_TAMPONU_BOYUTU && 
                       (mevcutKonum + eslesmeUzunlugu) < hamVeri.size() &&
                       hamVeri[gecmisKonum + eslesmeUzunlugu] == hamVeri[mevcutKonum + eslesmeUzunlugu]) {
                    eslesmeUzunlugu++;
                }

                // Daha uzun bir eşleşme bulduysak kaydet
                if (eslesmeUzunlugu > enIyiUzunluk) {
                    enIyiUzunluk = eslesmeUzunlugu;
                    enIyiMesafe = mevcutKonum - gecmisKonum;
                }
            }

            // Token oluştur
            Lz77Dugumu yeniDugum;
            yeniDugum.geriyeGidisMesafesi = enIyiMesafe;
            yeniDugum.eslesmeUzunlugu = enIyiUzunluk;

            // Sonraki karakteri al (eğer dosya sonuna gelmediysek)
            if (mevcutKonum + enIyiUzunluk < hamVeri.size()) {
                yeniDugum.sonrakiKarakter = hamVeri[mevcutKonum + enIyiUzunluk];
            } else {
                yeniDugum.sonrakiKarakter = 0; // Dosya sonu durumu
            }

            // Düğümü listeye ekle ve konumu ilerlet
            sikistirilmisVeri.push_back(yeniDugum);
            mevcutKonum += enIyiUzunluk + 1;
        }

        return sikistirilmisVeri;
    }

    // Sıkıştırılmış veriyi geri açan fonksiyon[cite: 1]
    std::vector<uint8_t> Lz77Motoru::geriAc(const std::vector<Lz77Dugumu>& sikistirilmisVeri) {
        std::vector<uint8_t> acilmisVeri;

        // Her bir sıkıştırılmış düğümü sırayla oku
        for (const auto& dugum : sikistirilmisVeri) {
            
            // Eğer bir eşleşme (geriye gidiş) varsa, eski verileri kopyala
            if (dugum.eslesmeUzunlugu > 0) {
                size_t kopyalamaBaslangici = acilmisVeri.size() - dugum.geriyeGidisMesafesi;
                for (int i = 0; i < dugum.eslesmeUzunlugu; ++i) {
                    acilmisVeri.push_back(acilmisVeri[kopyalamaBaslangici + i]);
                }
            }
            
            // Eşleşmeden sonraki yeni karakteri ekle (Eğer 0 ise ve veri sonuysa eklemeyebiliriz)
            acilmisVeri.push_back(dugum.sonrakiKarakter);
        }

        // Açılmış verinin en sonundaki gereksiz null byte (dosya sonu durumu) kontrol edilebilir
        return acilmisVeri;
    }

}