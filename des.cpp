#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>

using namespace std;

using u32 = unsigned int;
using u64 = unsigned long long;

// Permutation Tables

constexpr int IP[] = { 58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8, 57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3, 61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7 };

constexpr int IP_INV[] = { 40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25 };

constexpr int E[] = { 32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1 };

constexpr int P[] = { 16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10, 2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25 };

// Key Schedule Tables

constexpr int PC_1[] = { 57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4 };

constexpr int PC_2[] = { 14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32 };

constexpr int ROT[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

// S-Boxes Table

constexpr u32 S[8][64] = {
    { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7, 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8, 4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0, 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 },
    { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10, 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5, 0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15, 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 },
    { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8, 13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1, 13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7, 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 },
    { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15, 13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9, 10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4, 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 },
    { 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9, 14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6, 4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14, 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 },
    { 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11, 10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8, 9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6, 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 },
    { 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1, 13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6, 1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2, 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 },
    { 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7, 1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2, 7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8, 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 },
};

// 28-bit left cyclic shift
inline u32 lcs(u32 x, int s) {
    return (x << s & ((1u << 28) - 1))
        | (x >> (28 - s) & ((1u << s) - 1));
}

u32 feistel_function(u64 block, u64 subkey) {
    // Expansion
    u64 x = 0;
    for (int i = 0; i < 48; i++) {
        x |= (block >> (32 - E[i]) & 1) << (47 - i);
    }

    // Mixing
    x ^= subkey;

    // S-Box
    u32 before_P = 0;
    for (int i = 0; i < 8; i++) {
        int y = x >> (42 - i * 6) & 63;
        int j = (y & 32) | ((y & 1) << 4) | (y >> 1 & 15);
        before_P |= S[i][j] << (28 - i * 4);
    }

    // P-Box
    u32 ret = 0;
    for (int i = 0; i < 32; i++) {
        ret |= (before_P >> (32 - P[i]) & 1) << (31 - i);
    }

    return ret;
}

u64 des(u64 plaintext, u64 key0) {
    u32 l = 0, r = 0;
    for (int i = 0; i < 32; i++) {
        l |= (plaintext >> (64 - IP[i]) & 1) << (31 - i);
    }
    for (int i = 0; i < 32; i++) {
        r |= (plaintext >> (64 - IP[i + 32]) & 1) << (31 - i);
    }

    u64 key = 0;
    for (int i = 0; i < 56; i++) {
        key |= (key0 >> (64 - PC_1[i]) & 1) << (55 - i);
    }

    for (int t = 0; t < 16; t++) {
        key = u64(lcs(key >> 28 & ((1u << 28) - 1), ROT[t])) << 28
            | lcs(key & ((1u << 28) - 1), ROT[t]);

        u64 subkey = 0;
        for (int i = 0; i < 48; i++) {
            subkey |= (key >> (56 - PC_2[i]) & 1) << (47 - i);
        }

        l ^= feistel_function(r, subkey);
        swap(l, r);
    }

    swap(l, r);

    u64 before_IP_INV = u64(l) << 32 | r;
    u64 ret = 0;
    for (int i = 0; i < 64; i++) {
        ret |= (before_IP_INV >> (64 - IP_INV[i]) & 1) << (63 - i);
    }

    return ret;
}

int main() {
    auto total_begin_time = chrono::high_resolution_clock::now();

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << uppercase;

    for (;;) {
        auto case_begin_time = chrono::high_resolution_clock::now();

        string key_str, plaintext_str;
        string line;
        if (!getline(cin, line) || line.size() < 17) break;

        key_str = line.substr(0, 8);
        plaintext_str = line.substr(9, 8);

        u64 key = 0, plaintext = 0;
        for (int i = 0; i < 8; i++) {
            key |= u64(key_str[i]) << (56 - i * 8);
        }
        for (int i = 0; i < 8; i++) {
            plaintext |= u64(plaintext_str[i]) << (56 - i * 8);
        }

        auto encrypt_begin_time = chrono::high_resolution_clock::now();
        u64 ciphertext = des(plaintext, key);
        auto encrypt_end_time = chrono::high_resolution_clock::now();

        cout << hex << setfill('0');
        cout << "Ciphertext: ";
        cout << setw(16) << ciphertext;

        auto case_end_time = chrono::high_resolution_clock::now();

        cout << dec << setfill(' ');

        auto case_dur = case_end_time - case_begin_time;
        cout << ", Time:";
        cout << setw(7) << chrono::nanoseconds(case_dur).count();
        cout << " ns";

        auto encrypt_dur = encrypt_end_time - encrypt_begin_time;
        cout << ", Encrypt:";
        cout << setw(7) << chrono::nanoseconds(encrypt_dur).count();
        cout << " ns\n";
    }

    auto total_end_time = chrono::high_resolution_clock::now();

    auto total_dur = total_end_time - total_begin_time;
    cout << "Total Time: ";
    cout << chrono::nanoseconds(total_dur).count();
    cout << " ns\n";
}
