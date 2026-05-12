// ============================================================
// balok.cpp
// Menggambar balok 3D dengan 2 versi:
//   - BalokWarna   : warna solid (r, g, b)
//   - BalokTekstur : satu tekstur BMP untuk semua sisi
//
// Koordinat menggunakan titik awal (x1,y1,z1) dan titik akhir (x2,y2,z2)
// sehingga posisi dan ukuran bisa dikontrol penuh.
//
// Alur tekstur:
//   1. Image* img = loadBMP("namafile.bmp");   // load sekali di init
//   2. GLuint id  = loadTexture(img);           // upload ke GPU
//   3. delete img;                              // bebaskan memori
//   4. BalokTekstur(..., id);                   // pakai kapan saja
// ============================================================

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

// ============================================================
// Struct Image — menyimpan pixel RGB hasil baca BMP
// ============================================================
struct Image {
    int   width;
    int   height;
    unsigned char* pixels; // RGB, 3 byte per pixel

    Image(int w, int h) : width(w), height(h) {
        pixels = new unsigned char[w * h * 3];
    }
    ~Image() {
        delete[] pixels;
    }
};

// ============================================================
// loadBMP(filename)
// Membaca file .bmp 24-bit, mengembalikan pointer Image*.
// Caller bertanggung jawab memanggil delete setelah selesai.
// ============================================================
//Image* loadBMP(const char* filename)
//{
//    FILE* f = fopen(filename, "rb");
//    if (!f) {
//        printf("ERROR loadBMP: tidak bisa buka '%s'\n", filename);
//        return NULL;
//    }
//
//    unsigned char header[54];
//    if (fread(header, 1, 54, f) != 54 ||
//        header[0] != 'B' || header[1] != 'M') {
//        printf("ERROR loadBMP: bukan file BMP valid\n");
//        fclose(f);
//        return NULL;
//    }
//
//    int dataOffset = *(int*)&header[10];
//    int width      = *(int*)&header[18];
//    int height     = *(int*)&header[22];
//    int bpp        = *(short*)&header[28];
//
//    if (bpp != 24) {
//        printf("ERROR loadBMP: hanya support 24-bit BMP (bpp=%d)\n", bpp);
//        fclose(f);
//        return NULL;
//    }
//
//    fseek(f, dataOffset, SEEK_SET);
//
//    // BMP disimpan BGR, tiap baris dipad ke kelipatan 4 byte
//    int rowSize = ((width * 3 + 3) / 4) * 4;
//    unsigned char* raw = (unsigned char*)malloc(rowSize * height);
//    fread(raw, 1, rowSize * height, f);
//    fclose(f);
//
//    Image* img = new Image(width, height);
//
//    // Konversi BGR -> RGB
//    for (int y = 0; y < height; y++) {
//        for (int x = 0; x < width; x++) {
//            int src = y * rowSize + x * 3;
//            int dst = (y * width + x) * 3;
//            img->pixels[dst + 0] = raw[src + 2]; // R
//            img->pixels[dst + 1] = raw[src + 1]; // G
//            img->pixels[dst + 2] = raw[src + 0]; // B
//        }
//    }
//
//    free(raw);
//    return img;
//}

// ============================================================
// loadTexture(image)
// Upload Image ke GPU, kembalikan GLuint texture ID.
// Setelah ini caller boleh delete image-nya.
// ============================================================
//GLuint loadTexture(Image* image)
//{
//    if (!image) return 0;
//
//    GLuint texID;
//    glGenTextures(1, &texID);
//    glBindTexture(GL_TEXTURE_2D, texID);
//
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
//                 image->width, image->height, 0,
//                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
//
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//    return texID;
//}

// ======= Helper internal: gambar 6 sisi balok =======
// Dipanggil setelah warna/tekstur sudah di-set di luar fungsi ini.
static void _drawBalokFaces(
    float x1, float y1, float z1,
    float x2, float y2, float z2)
{
    // Pastikan urutan min/max benar
    float minX = x1 < x2 ? x1 : x2;
    float maxX = x1 > x2 ? x1 : x2;
    float minY = y1 < y2 ? y1 : y2;
    float maxY = y1 > y2 ? y1 : y2;
    float minZ = z1 < z2 ? z1 : z2;
    float maxZ = z1 > z2 ? z1 : z2;

    glBegin(GL_QUADS);

    // DEPAN (+Z)
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,0); glVertex3f(minX, minY, maxZ);
    glTexCoord2f(1,0); glVertex3f(maxX, minY, maxZ);
    glTexCoord2f(1,1); glVertex3f(maxX, maxY, maxZ);
    glTexCoord2f(0,1); glVertex3f(minX, maxY, maxZ);

    // BELAKANG (-Z)
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,0); glVertex3f(minX, minY, minZ);
    glTexCoord2f(0,1); glVertex3f(minX, maxY, minZ);
    glTexCoord2f(1,1); glVertex3f(maxX, maxY, minZ);
    glTexCoord2f(1,0); glVertex3f(maxX, minY, minZ);

    // ATAS (+Y)
    glNormal3f(0, 1, 0);
    glTexCoord2f(0,0); glVertex3f(minX, maxY, minZ);
    glTexCoord2f(0,1); glVertex3f(minX, maxY, maxZ);
    glTexCoord2f(1,1); glVertex3f(maxX, maxY, maxZ);
    glTexCoord2f(1,0); glVertex3f(maxX, maxY, minZ);

    // BAWAH (-Y)
    glNormal3f(0, -1, 0);
    glTexCoord2f(0,0); glVertex3f(minX, minY, minZ);
    glTexCoord2f(1,0); glVertex3f(maxX, minY, minZ);
    glTexCoord2f(1,1); glVertex3f(maxX, minY, maxZ);
    glTexCoord2f(0,1); glVertex3f(minX, minY, maxZ);

    // KANAN (+X)
    glNormal3f(1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(maxX, minY, minZ);
    glTexCoord2f(0,1); glVertex3f(maxX, maxY, minZ);
    glTexCoord2f(1,1); glVertex3f(maxX, maxY, maxZ);
    glTexCoord2f(1,0); glVertex3f(maxX, minY, maxZ);

    // KIRI (-X)
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(minX, minY, minZ);
    glTexCoord2f(1,0); glVertex3f(minX, minY, maxZ);
    glTexCoord2f(1,1); glVertex3f(minX, maxY, maxZ);
    glTexCoord2f(0,1); glVertex3f(minX, maxY, minZ);

    glEnd();
}

// ============================================================
// BalokWarna
// Menggambar balok dengan warna solid RGB.
// (x1,y1,z1) = sudut minimum, (x2,y2,z2) = sudut maksimum
// r, g, b : nilai 0.0 – 1.0
// ============================================================
void BalokWarna(
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    float r,  float g,  float b)
{
    glDisable(GL_TEXTURE_2D);
    glColor3f(r, g, b);
    _drawBalokFaces(x1, y1, z1, x2, y2, z2);
}

// ============================================================
// BalokTekstur
// Menggambar balok dengan satu tekstur (semua sisi sama).
// texID : hasil loadBMP() yang disimpan sebelumnya
// ============================================================
void BalokTekstur(
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    GLuint texID)
{
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f); // reset warna agar tekstur tidak terpengaruh
    glBindTexture(GL_TEXTURE_2D, texID);
    _drawBalokFaces(x1, y1, z1, x2, y2, z2);
    glDisable(GL_TEXTURE_2D);
}
