# BlaterPong
**BlaterPong** adalah versi modern dari permainan klasik *Pong*, yang memungkinkan pemain untuk bersaing melawan *bot* dengan tiga tingkat kesulitan yang berbeda. Dirancang dengan antarmuka yang menarik dan musik latar yang asik, BlaterPong menawarkan pengalaman bermain yang lebih seru dan menantang. Dengan fitur highscore dan gameplay yang responsif, game ini cocok untuk menguji kemampuan refleks Anda sekaligus bersenang-senang.

## Fitur Utama
- **Highscore**: Catat skor tertinggi Anda dan coba untuk mengalahkan rekor tersebut.
- **Tiga Tingkat Kesulitan**: Pilih tingkat kesulitan sesuai dengan kemampuan Anda, dari bot yang mudah hingga yang sangat menantang.
- **UI Menarik**: Antarmuka grafis yang bersih dan modern, dengan tampilan yang mudah dipahami dan menyenangkan untuk dilihat.
- **Backsound Asyik**: Musik latar yang menyenangkan untuk meningkatkan suasana permainan.
- **Kontrol yang Sederhana**: Gunakan keyboard untuk mengendalikan paddle dan menghindari bola yang datang dengan kecepatan yang semakin meningkat.

## Teknologi
- **Bahasa Pemrograman**: C++
- **Library**: Raylib (untuk grafis dan audio)
- **Platform**: Dapat dijalankan di berbagai platform seperti Windows, Linux, dan macOS.

## Cara Memulai
1. **Persiapkan Lingkungan Pengembangan**:
   - Pastikan Anda telah menginstal compiler C++ dan Raylib di mesin Anda.
   - Ikuti petunjuk instalasi Raylib di [website resmi Raylib](https://www.raylib.com/).

2. **Clone Repositori**:
   Clone repositori ini untuk mendapatkan kode sumber game:
   ```bash
   git clone https://github.com/username/blaterpong.git
   ```
   
3. **Menyusun dan Menjalankan Game**:
   Setelah meng-clone repositori, buka folder proyek dan kompilasi dengan perintah:
   ```bash
   g++ -o BlaterPong main.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
   ```
   Kemudian jalankan game dengan:
   ```bash
   ./BlaterPong
   ```

   PASTIKAN FILE BIN(.EXE) BERADA PADA SATU FOLDER YANG SAMA DENGAN MAIN.CPP

## Kontrol
- **Paddle Pemain**: Gunakan tombol **Panah Atas** untuk bergerak ke atas dan **Panah Bawah** untuk bergerak ke bawah.
- **Paddle Bot**: Bot akan secara otomatis bergerak berdasarkan tingkat kesulitan yang dipilih.
- **Back To Menu**: Tekan **SPACE** untuk menjeda permainan dan kembali ke menu utama.

## Fitur Highscore
- Skor tertinggi akan dicatat secara otomatis setelah setiap permainan selesai.
- Anda dapat melihat daftar skor tertinggi dari menu utama.

## Kontribusi
Kami sangat terbuka untuk kontribusi dari para pengembang dan pemain! Jika Anda memiliki ide atau perbaikan untuk fitur baru, silakan buka issue atau ajukan pull request. Pastikan untuk melakukan pengujian sebelum mengirimkan kontribusi Anda.

## Lisensi
Proyek ini dilisensikan di bawah **MIT License** - lihat [LICENSE](LICENSE) untuk lebih lanjut.

## Kolaborator
<a href="https://github.com/arrvy/BlaterPong/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=arrvy/BlaterPong" />
</a>
