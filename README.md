# Smart-Lamp

**Smart-Lamp** adalah sistem IoT berbasis **ESP32** yang dirancang untuk mendeteksi gerakan di ruangan menggunakan **sensor PIR** dan mengontrol lampu (LED) berdasarkan aktivitas tersebut. Data dari sensor dan status LED dikirim ke **Firebase Realtime Database**, dan informasi real-time ditampilkan pada **LCD 20x4**.

## Komponen yang Digunakan
1. **ESP32**: Mikrokontroler utama untuk mengolah data sensor, mengontrol LED, dan mengirim data ke Firebase.
2. **Sensor PIR (2 unit)**: 
   - PIR 1: Untuk mendeteksi gerakan di ruang tamu.
   - PIR 2: Untuk mendeteksi gerakan di ruang kerja.
3. **LED (2 unit)**: 
   - LED 1: Indikator untuk ruang tamu.
   - LED 2: Indikator untuk ruang kerja.
4. **LCD 20x4 (I2C)**: Menampilkan status gerakan dan LED secara real-time.
5. **Resistor (2 unit)**: Untuk membatasi arus pada LED.
6. **Firebase Realtime Database**: Untuk menyimpan data sensor dan status LED secara cloud.

## Cara Kerja Alat
1. **Deteksi Gerakan**:
   - **Sensor PIR 1** mendeteksi gerakan di ruang tamu.
   - **Sensor PIR 2** mendeteksi gerakan di ruang kerja.
   - Jika gerakan terdeteksi, LED yang sesuai akan menyala.
   - Jika tidak ada gerakan, LED akan mati.

2. **Tampilan Data**:
   - Status gerakan dan LED ditampilkan pada **LCD 20x4** dengan pembagian:
     - Baris 1-2: Informasi ruang tamu.
     - Baris 3-4: Informasi ruang kerja.

3. **Integrasi Firebase**:
   - Data gerakan dari sensor PIR dikirim ke Firebase dengan struktur sebagai berikut:
     ```
     sensor/
       pir1: true/false
       pir2: true/false
     akuator/
       led1: true/false
       led2: true/false
     ```
   - Data status LED dapat diambil dari Firebase untuk monitoring jarak jauh.

## Koneksi Wi-Fi dan Firebase
- **SSID**: Wokwi-GUEST
- **Password**: (tidak diperlukan untuk jaringan ini)
- **Firebase Database URL**: `https://latihan-kirim-data-esp-e9c34-default-rtdb.asia-southeast1.firebasedatabase.app/`
- **API Key**: `AIzaSyCyJ2Lv-8sFr--k588Y1-1bI3LRN7zwVpA`

## Diagram Alur
```mermaid
flowchart TD
    A[Sensor PIR] -->|Gerakan terdeteksi| B[ESP32]
    B -->|Data dikirim ke| C[Firebase]
    C -->|Data disimpan| D[Realtime Database]
    B -->|Kontrol| E[LED]
    B -->|Tampilkan status| F[LCD 20x4]
