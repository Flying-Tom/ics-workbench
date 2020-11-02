int64_t std_add(int64_t a, int64_t b) {
  return a + b;
}
int std_popcnt(uint64_t x) {
  int s = 0;
  for (int i = 0; i < 64; i++) {
    if ((x >> i) & 1) s++;
  }
  return s;
}
void *std_memcpy(void *dest, const void *src, size_t n) {
  return memcpy(dest, src, n);
}