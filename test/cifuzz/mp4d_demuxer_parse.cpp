#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "mp4d_demux.h"
#include "mp4d_internal.h"
#include <cifuzz/cifuzz.h>
#include <fuzzer/FuzzedDataProvider.h>

#include <inttypes.h>



mp4d_demuxer_ptr_t p_dmux;

void *static_mem_ptr = NULL, *dyn_mem_ptr = NULL;

FUZZ_TEST_SETUP() {
  uint64_t static_mem_size, dyn_mem_size;

  mp4d_demuxer_query_mem(&static_mem_size, &dyn_mem_size);

  if (static_mem_size) {
    static_mem_ptr = malloc((size_t) static_mem_size);
  }

  if (dyn_mem_size) {
    dyn_mem_ptr = malloc ((size_t) dyn_mem_size);
  }

  mp4d_demuxer_init(&p_dmux, static_mem_ptr, dyn_mem_ptr);
}

FUZZ_TEST(const uint8_t *Data, size_t Size) {
  FuzzedDataProvider fdp(Data, Size);
  int is_eof = fdp.PickValueInArray<int>({0, 1});
  uint64_t atom_size;
  auto bytes = fdp.ConsumeRemainingBytes<unsigned char>();

  mp4d_demuxer_parse(p_dmux, bytes.data(), bytes.size(), is_eof, 0, &atom_size);
}
