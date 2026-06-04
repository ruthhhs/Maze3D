#include <cmath> // Butuh ini untuk fungsi sqrtf (akar pangkat dua)
#include "goal.h"
bool checkGoal(float playerX, float playerZ, float targetX, float targetZ) {
    // 1. Hitung jarak antara pemain dan posisi finish menggunakan rumus Pythagoras
    float dx = playerX - targetX;
    float dz = playerZ - targetZ;
    float distance = sqrtf(dx * dx + dz * dz);

    // 2. Jika jaraknya kurang dari 1.0 unit, anggap sudah sampai finish
    if (distance < 1.0f) { 
        return true;  // Mengembalikan true (1) jika sampai
    } else {
        return false; // Mengembalikan false (0) jika belum
    }
}
