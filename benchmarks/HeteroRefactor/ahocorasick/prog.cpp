
struct __dst_alloc_list_base_t 
{
  unsigned int prev;
  unsigned int next;
}
;
// type __didxc was char *
typedef unsigned long long __didxc;
// type __didxclass_node was struct ::node{int substring_index;struct node *fail;struct node *next[26];}*
typedef unsigned long long __didxclass_node;
// === BEGIN REC SUPPORT LIBRARY ===
#include <stdio.h>
#include <stdlib.h>
const char *__dst_filename = "";
unsigned long long __dst_file = 0;

struct __dst_alloc_list__dmemc 
{
  struct __dst_alloc_list_base_t _link;
  char _data;
}
;
struct __dst_alloc_list__dmemc __dmemc[1025U];
# 1 "<stdin>"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "<stdin>"
# 54 "<stdin>"
typedef unsigned long long __dst_alloc_size_t;
typedef unsigned char __dst_alloc_bucket_size_t;
# 73 "<stdin>"
unsigned char __dst_alloc_node_split__dmemc[(1 << (10)) / 8];
static unsigned char __dst_alloc_test_parent_split__dmemc(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    return (__dst_alloc_node_split__dmemc[index / 8] >> (index % 8)) & 1;
}
static void __dst_alloc_flip_parent_split__dmemc(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    __dst_alloc_node_split__dmemc[index / 8] ^= 1 << (index % 8);
}
static void __dst_alloc_clr_parent_split__dmemc(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    __dst_alloc_node_split__dmemc[index / 8] &= ~(1 << (index % 8));
}
struct __dst_alloc_list_base_t __dst_alloc_buckets__dmemc[10 + 1] = {{1, 1}};
static __dst_alloc_bucket_size_t __dst_alloc_bucket_for_request__dmemc(
        __dst_alloc_size_t request) {
    __dst_alloc_bucket_size_t bucket = 10;
    __dst_alloc_size_t size = 1;
    while (size < request) {
        size <<= 1;
        bucket -= 1;
    }
    return bucket;
}
static void __dst_alloc_list_push__dmemc(
        __dst_alloc_bucket_size_t bucket,
        __dst_alloc_size_t entry) {
    __dst_alloc_size_t prev = __dst_alloc_buckets__dmemc[bucket].prev;
    __dmemc[entry]._link.prev = prev;
    __dmemc[entry]._link.next = 0;
    if (prev == 0) __dst_alloc_buckets__dmemc[bucket].next = entry;
    else __dmemc[prev]._link.next = entry;
    __dst_alloc_buckets__dmemc[bucket].prev = entry;
}
static void __dst_alloc_list_remove__dmemc(
        __dst_alloc_bucket_size_t bucket,
        __dst_alloc_size_t entry) {
    __dst_alloc_size_t prev = __dmemc[entry]._link.prev;
    __dst_alloc_size_t next = __dmemc[entry]._link.next;
    if (prev == 0) __dst_alloc_buckets__dmemc[bucket].next = next;
    else __dmemc[prev]._link.next = next;
    if (next == 0) __dst_alloc_buckets__dmemc[bucket].prev = prev;
    else __dmemc[next]._link.prev = prev;
}
static __dst_alloc_size_t __dst_alloc_list_pop__dmemc(
        __dst_alloc_bucket_size_t bucket) {
    __dst_alloc_size_t back = __dst_alloc_buckets__dmemc[bucket].prev;
    if (back == 0) return 0;
    __dst_alloc_list_remove__dmemc(bucket, back);
    return back;
}
static __dst_alloc_size_t __dst_alloc_index_for_node__dmemc(
        __dst_alloc_size_t bucket_index,
        __dst_alloc_bucket_size_t bucket) {
    return 1 + ((bucket_index - (1 << bucket) + 1) << (10 - bucket));
}
static __dst_alloc_size_t __dst_alloc_node_for_index__dmemc(
        __dst_alloc_size_t global_index,
        __dst_alloc_bucket_size_t bucket) {
    return ((global_index - 1) >> (10 - bucket)) + (1 << bucket) - 1;
}
void __dst_alloc_init__dmemc() {
    for (int i = 0; i <= 10; i++) {
        __dst_alloc_buckets__dmemc[i].prev = 0;
        __dst_alloc_buckets__dmemc[i].next = 0;
    }
    __dst_alloc_list_push__dmemc(0, 1);
}
__dst_alloc_size_t __dst_alloc_malloc__dmemc(__dst_alloc_size_t request) {
    request = request / sizeof(char);
    if (request > (1 << 10)) return 0;
    __dst_alloc_bucket_size_t bucket =
        __dst_alloc_bucket_for_request__dmemc(request);
    __dst_alloc_bucket_size_t original_bucket = bucket;
    while ((__dst_alloc_bucket_size_t)(bucket + 1) != 0) {
        __dst_alloc_size_t ptr = __dst_alloc_list_pop__dmemc(bucket);
        if (!ptr) {
            bucket--; continue;
        }
        __dst_alloc_size_t i = __dst_alloc_node_for_index__dmemc(ptr, bucket);
        if (i != 0) __dst_alloc_flip_parent_split__dmemc(i);
        while (bucket < original_bucket) {
            i = ((i) * 2 + 1);
            __dst_alloc_clr_parent_split__dmemc(i);
            __dst_alloc_flip_parent_split__dmemc(i);
            bucket++;
            __dst_alloc_list_push__dmemc(bucket, __dst_alloc_index_for_node__dmemc(((i) + 1), bucket));
        }
        __dmemc[ptr]._link.prev = request;
        return ptr;
    }
    return 0;
}
void __dst_alloc_free__dmemc(__dst_alloc_size_t ptr) {
    if (ptr == 0) return;
    __dst_alloc_bucket_size_t bucket =
        __dst_alloc_bucket_for_request__dmemc(__dmemc[ptr]._link.prev);
    __dst_alloc_size_t i = __dst_alloc_node_for_index__dmemc(ptr, bucket);
    while (i != 0) {
        __dst_alloc_flip_parent_split__dmemc(i);
        if (__dst_alloc_test_parent_split__dmemc(i)) break;
        __dst_alloc_list_remove__dmemc(bucket, __dst_alloc_index_for_node__dmemc(((((i) - 1) ^ 1) + 1), bucket));
        i = (((i) - 1) / 2);
        bucket--;
    }
    __dst_alloc_list_push__dmemc(bucket, __dst_alloc_index_for_node__dmemc(i, bucket));
}
unsigned long long __dst_alloc_malloc__dmemc(unsigned long long );
void __dst_alloc_free__dmemc(unsigned long long );
// === END REC SUPPORT LIBRARY ===
// #include "header.hpp"
#include <cstdlib>
/*
 * Trie tree with Aho-Corasick state machine.
 *
 * de la Briandais, René (1959). File searching using variable length keys.
 * Proc. Western J. Computer Conf. pp. 295–298. Cited by Brass.
 *
 * Aho, Alfred V.; Corasick, Margaret J. (June 1975). "Efficient string
 * matching: An aid to bibliographic search". Communications of the ACM.
 * 18 (6): 333–340. doi:10.1145/360825.360855. MR 0371172.
 */

struct node 
{
  int substring_index;
  __didxclass_node fail;
  __didxclass_node next[26];
}
;

struct __dst_alloc_list__dmemclass_node 
{
  struct __dst_alloc_list_base_t _link;
  struct node _data;
}
;
struct __dst_alloc_list__dmemclass_node __dmemclass_node[1025U];
# 1 "<stdin>"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "<stdin>"
# 54 "<stdin>"
typedef unsigned long long __dst_alloc_size_t;
typedef unsigned char __dst_alloc_bucket_size_t;
# 73 "<stdin>"
unsigned char __dst_alloc_node_split__dmemclass_node[(1 << (10)) / 8];
static unsigned char __dst_alloc_test_parent_split__dmemclass_node(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    return (__dst_alloc_node_split__dmemclass_node[index / 8] >> (index % 8)) & 1;
}
static void __dst_alloc_flip_parent_split__dmemclass_node(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    __dst_alloc_node_split__dmemclass_node[index / 8] ^= 1 << (index % 8);
}
static void __dst_alloc_clr_parent_split__dmemclass_node(__dst_alloc_size_t index) {
    index = (index - 1) / 2;
    __dst_alloc_node_split__dmemclass_node[index / 8] &= ~(1 << (index % 8));
}
struct __dst_alloc_list_base_t __dst_alloc_buckets__dmemclass_node[10 + 1] = {{1, 1}};
static __dst_alloc_bucket_size_t __dst_alloc_bucket_for_request__dmemclass_node(
        __dst_alloc_size_t request) {
    __dst_alloc_bucket_size_t bucket = 10;
    __dst_alloc_size_t size = 1;
    while (size < request) {
        size <<= 1;
        bucket -= 1;
    }
    return bucket;
}
static void __dst_alloc_list_push__dmemclass_node(
        __dst_alloc_bucket_size_t bucket,
        __dst_alloc_size_t entry) {
    __dst_alloc_size_t prev = __dst_alloc_buckets__dmemclass_node[bucket].prev;
    __dmemclass_node[entry]._link.prev = prev;
    __dmemclass_node[entry]._link.next = 0;
    if (prev == 0) __dst_alloc_buckets__dmemclass_node[bucket].next = entry;
    else __dmemclass_node[prev]._link.next = entry;
    __dst_alloc_buckets__dmemclass_node[bucket].prev = entry;
}
static void __dst_alloc_list_remove__dmemclass_node(
        __dst_alloc_bucket_size_t bucket,
        __dst_alloc_size_t entry) {
    __dst_alloc_size_t prev = __dmemclass_node[entry]._link.prev;
    __dst_alloc_size_t next = __dmemclass_node[entry]._link.next;
    if (prev == 0) __dst_alloc_buckets__dmemclass_node[bucket].next = next;
    else __dmemclass_node[prev]._link.next = next;
    if (next == 0) __dst_alloc_buckets__dmemclass_node[bucket].prev = prev;
    else __dmemclass_node[next]._link.prev = prev;
}
static __dst_alloc_size_t __dst_alloc_list_pop__dmemclass_node(
        __dst_alloc_bucket_size_t bucket) {
    __dst_alloc_size_t back = __dst_alloc_buckets__dmemclass_node[bucket].prev;
    if (back == 0) return 0;
    __dst_alloc_list_remove__dmemclass_node(bucket, back);
    return back;
}
static __dst_alloc_size_t __dst_alloc_index_for_node__dmemclass_node(
        __dst_alloc_size_t bucket_index,
        __dst_alloc_bucket_size_t bucket) {
    return 1 + ((bucket_index - (1 << bucket) + 1) << (10 - bucket));
}
static __dst_alloc_size_t __dst_alloc_node_for_index__dmemclass_node(
        __dst_alloc_size_t global_index,
        __dst_alloc_bucket_size_t bucket) {
    return ((global_index - 1) >> (10 - bucket)) + (1 << bucket) - 1;
}
void __dst_alloc_init__dmemclass_node() {
    for (int i = 0; i <= 10; i++) {
        __dst_alloc_buckets__dmemclass_node[i].prev = 0;
        __dst_alloc_buckets__dmemclass_node[i].next = 0;
    }
    __dst_alloc_list_push__dmemclass_node(0, 1);
}
__dst_alloc_size_t __dst_alloc_malloc__dmemclass_node(__dst_alloc_size_t request) {
    request = request / sizeof(struct ::node);
    if (request > (1 << 10)) return 0;
    __dst_alloc_bucket_size_t bucket =
        __dst_alloc_bucket_for_request__dmemclass_node(request);
    __dst_alloc_bucket_size_t original_bucket = bucket;
    while ((__dst_alloc_bucket_size_t)(bucket + 1) != 0) {
        __dst_alloc_size_t ptr = __dst_alloc_list_pop__dmemclass_node(bucket);
        if (!ptr) {
            bucket--; continue;
        }
        __dst_alloc_size_t i = __dst_alloc_node_for_index__dmemclass_node(ptr, bucket);
        if (i != 0) __dst_alloc_flip_parent_split__dmemclass_node(i);
        while (bucket < original_bucket) {
            i = ((i) * 2 + 1);
            __dst_alloc_clr_parent_split__dmemclass_node(i);
            __dst_alloc_flip_parent_split__dmemclass_node(i);
            bucket++;
            __dst_alloc_list_push__dmemclass_node(bucket, __dst_alloc_index_for_node__dmemclass_node(((i) + 1), bucket));
        }
        __dmemclass_node[ptr]._link.prev = request;
        return ptr;
    }
    return 0;
}
void __dst_alloc_free__dmemclass_node(__dst_alloc_size_t ptr) {
    if (ptr == 0) return;
    __dst_alloc_bucket_size_t bucket =
        __dst_alloc_bucket_for_request__dmemclass_node(__dmemclass_node[ptr]._link.prev);
    __dst_alloc_size_t i = __dst_alloc_node_for_index__dmemclass_node(ptr, bucket);
    while (i != 0) {
        __dst_alloc_flip_parent_split__dmemclass_node(i);
        if (__dst_alloc_test_parent_split__dmemclass_node(i)) break;
        __dst_alloc_list_remove__dmemclass_node(bucket, __dst_alloc_index_for_node__dmemclass_node(((((i) - 1) ^ 1) + 1), bucket));
        i = (((i) - 1) / 2);
        bucket--;
    }
    __dst_alloc_list_push__dmemclass_node(bucket, __dst_alloc_index_for_node__dmemclass_node(i, bucket));
}
unsigned long long __dst_alloc_malloc__dmemclass_node(unsigned long long );
void __dst_alloc_free__dmemclass_node(unsigned long long );
int node_count = 1;
bool g_fallback = false;

__didxclass_node new_node()
{
  __didxclass_node curr = (__didxclass_node )(__dst_alloc_malloc__dmemclass_node(sizeof(struct node )));
  if (!curr) {
    g_fallback = true;
    return 0L;
  }
  (&(__dmemclass_node + curr + 0U) -> _data) -> substring_index = - 1;
  (&(__dmemclass_node + curr + 0U) -> _data) -> fail = 0L;
  for (int i = 0; i < 26; i++) {
    (&(__dmemclass_node + curr + 0U) -> _data) -> next[i] = 0L;
  }
  return curr;
}
/*
 * Insert a new trie node with string as content.
 * The node will be inserted to the trie specified by root.
 *
 * Return: the '%' location of the string.
 */

struct __rect_packed_type_LAEKnwKyFGBzR 
{
  __didxclass_node local0;
  __didxc local1;
  int local2;
  char local3;
  int local4;
  unsigned int _location;
  int _return;
  int _ret0;
}
;

int insert_node(__didxclass_node root,__didxc str,int substring_index)
{
  volatile struct __rect_packed_type_LAEKnwKyFGBzR __rect_packed_var_LAEKnwKyFGBzR[1024U];
  volatile unsigned int __rect_packed_top_LAEKnwKyFGBzR = 0U;
  __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . _location = 1U;
  __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . local0 = root;
  __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . local1 = str;
  __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . local2 = substring_index;
  ++__rect_packed_top_LAEKnwKyFGBzR;
  __rect_func_L0_LAEKnwKyFGBzR:
  if (0U == __rect_packed_top_LAEKnwKyFGBzR) 
    return __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . _return;
  --__rect_packed_top_LAEKnwKyFGBzR;
  if (1U == __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . _location) 
    goto __rect_func_L1_LAEKnwKyFGBzR;
  if (2U == __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . _location) 
    goto __rect_func_L2_LAEKnwKyFGBzR;
  __rect_func_L1_LAEKnwKyFGBzR:
  __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . local3 =  *(&(__dmemc + __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . local1 + 0U) -> _data);
  if (__rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . local3 == '%') {
    (&(__dmemclass_node + __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . local0 + 0U) -> _data) -> substring_index = __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . local2;
    __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . _return = 0;
    goto __rect_func_L0_LAEKnwKyFGBzR;
  }
   else {
    __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . local4 = __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . local3 - 'a';
    if (!(&(__dmemclass_node + __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . local0 + 0U) -> _data) -> next[__rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . local4]) {
      (&(__dmemclass_node + __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . local0 + 0U) -> _data) -> next[__rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . local4] = new_node();
      if (!(&(__dmemclass_node + __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . local0 + 0U) -> _data) -> next[__rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . local4]) {
        __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . _return = 0;
        goto __rect_func_L0_LAEKnwKyFGBzR;
      }
      node_count += 1;
    }
    if (__rect_packed_top_LAEKnwKyFGBzR + 1U == 1024U) {;
g_fallback = true;
      int __temp0__;
      return __temp0__;
    }
    __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . _location = 2U;
    __rect_packed_var_LAEKnwKyFGBzR[1 + __rect_packed_top_LAEKnwKyFGBzR] . local2 = __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . local2;
    __rect_packed_var_LAEKnwKyFGBzR[1 + __rect_packed_top_LAEKnwKyFGBzR] . local1 = __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . local1 + 1;
    __rect_packed_var_LAEKnwKyFGBzR[1 + __rect_packed_top_LAEKnwKyFGBzR] . local0 = (&(__dmemclass_node + __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . local0 + 0U) -> _data) -> next[__rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . local4];
    ++__rect_packed_top_LAEKnwKyFGBzR;
    __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . _location = 1U;
    goto __rect_func_L1_LAEKnwKyFGBzR;
    __rect_func_L2_LAEKnwKyFGBzR:
    __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . _ret0 = (__rect_packed_var_LAEKnwKyFGBzR[1 + __rect_packed_top_LAEKnwKyFGBzR] . _return);
    __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . _return = __rect_packed_var_LAEKnwKyFGBzR[0 + __rect_packed_top_LAEKnwKyFGBzR] . _ret0 + 1;
    goto __rect_func_L0_LAEKnwKyFGBzR;
  }
  goto __rect_func_L0_LAEKnwKyFGBzR;
}
/*
 * Build Aho-Corasick state machine.
 */

void build_AhoCorasick(__didxclass_node root,int node_count)
{
  __didxclass_node queue[16384];
// initialize queue
  int head = 0;
  int tail = 1;
  queue[0] = root;
  for (; head < tail; head++) {
    __didxclass_node curr = queue[head];
    for (int i = 0; i < 26; i++) {
// non-existent node
      if (!(&(__dmemclass_node + curr + 0U) -> _data) -> next[i]) 
        continue; 
// Aho-Corasick fail link
      (&(__dmemclass_node + (&(__dmemclass_node + curr + 0U) -> _data) -> next[i] + 0U) -> _data) -> fail = root;
      for (__didxclass_node p = (&(__dmemclass_node + curr + 0U) -> _data) -> fail; p; p = (&(__dmemclass_node + p + 0U) -> _data) -> fail) {
        if ((&(__dmemclass_node + p + 0U) -> _data) -> next[i]) {
          (&(__dmemclass_node + (&(__dmemclass_node + curr + 0U) -> _data) -> next[i] + 0U) -> _data) -> fail = (&(__dmemclass_node + p + 0U) -> _data) -> next[i];
          break; 
        }
      }
// add to queue
      queue[tail++] = (&(__dmemclass_node + curr + 0U) -> _data) -> next[i];
    }
  }
}
/*
 * Query on Aho-Corasick state machine and write matched indexes.
 */

void query_AhoCorasick(__didxclass_node root,char query[1024],int substring_indexes[1024],int query_indexes[1024])
{
  __didxclass_node curr = root;
  for (int offset = 0; ; offset++) {
    char ch = query[offset];
    if (ch == '%') 
      break; 
    int idx = ch - 'a';
// follow fail link if not matched in curr
    while(!(&(__dmemclass_node + curr + 0U) -> _data) -> next[idx] && curr != root)
      curr = (&(__dmemclass_node + curr + 0U) -> _data) -> fail;
// if matched next char
    if ((&(__dmemclass_node + curr + 0U) -> _data) -> next[idx]) 
      curr = (&(__dmemclass_node + curr + 0U) -> _data) -> next[idx];
// follow fail link to check matches
    for (__didxclass_node follow = curr; follow != root; follow = (&(__dmemclass_node + follow + 0U) -> _data) -> fail) {
      if ((&(__dmemclass_node + follow + 0U) -> _data) -> substring_index != - 1) {
         *(substring_indexes++) = (&(__dmemclass_node + follow + 0U) -> _data) -> substring_index;
         *(query_indexes++) = offset;
      }
    }
  }
// end indexes
   *substring_indexes = - 1;
   *query_indexes = - 1;
}
/*
 * Delete the whole tree
 */

struct __rect_packed_type_L4lV9CXFW1bhR 
{
  __didxclass_node local0;
  int local1;
  unsigned int _location;
}
;

void delete_tree(__didxclass_node root)
{
  volatile struct __rect_packed_type_L4lV9CXFW1bhR __rect_packed_var_L4lV9CXFW1bhR[1024U];
  volatile unsigned int __rect_packed_top_L4lV9CXFW1bhR = 0U;
  __rect_packed_var_L4lV9CXFW1bhR[0 + __rect_packed_top_L4lV9CXFW1bhR] . _location = 1U;
  __rect_packed_var_L4lV9CXFW1bhR[0 + __rect_packed_top_L4lV9CXFW1bhR] . local0 = root;
  ++__rect_packed_top_L4lV9CXFW1bhR;
  __rect_func_L0_L4lV9CXFW1bhR:
  if (0U == __rect_packed_top_L4lV9CXFW1bhR) 
    return ;
  --__rect_packed_top_L4lV9CXFW1bhR;
  if (1U == __rect_packed_var_L4lV9CXFW1bhR[0 + __rect_packed_top_L4lV9CXFW1bhR] . _location) 
    goto __rect_func_L1_L4lV9CXFW1bhR;
  if (2U == __rect_packed_var_L4lV9CXFW1bhR[0 + __rect_packed_top_L4lV9CXFW1bhR] . _location) 
    goto __rect_func_L2_L4lV9CXFW1bhR;
  __rect_func_L1_L4lV9CXFW1bhR:
  if (!__rect_packed_var_L4lV9CXFW1bhR[0 + __rect_packed_top_L4lV9CXFW1bhR] . local0) {
    goto __rect_func_L0_L4lV9CXFW1bhR;
  }
  for (__rect_packed_var_L4lV9CXFW1bhR[0 + __rect_packed_top_L4lV9CXFW1bhR] . local1 = 0; __rect_packed_var_L4lV9CXFW1bhR[0 + __rect_packed_top_L4lV9CXFW1bhR] . local1 < 26; __rect_packed_var_L4lV9CXFW1bhR[0 + __rect_packed_top_L4lV9CXFW1bhR] . local1++) {
    if (__rect_packed_top_L4lV9CXFW1bhR + 1U == 1024U) {;
g_fallback = true;
      return ;
    }
    __rect_packed_var_L4lV9CXFW1bhR[0 + __rect_packed_top_L4lV9CXFW1bhR] . _location = 2U;
    __rect_packed_var_L4lV9CXFW1bhR[1 + __rect_packed_top_L4lV9CXFW1bhR] . local0 = (&(__dmemclass_node + __rect_packed_var_L4lV9CXFW1bhR[0 + __rect_packed_top_L4lV9CXFW1bhR] . local0 + 0U) -> _data) -> next[__rect_packed_var_L4lV9CXFW1bhR[0 + __rect_packed_top_L4lV9CXFW1bhR] . local1];
    ++__rect_packed_top_L4lV9CXFW1bhR;
    __rect_packed_var_L4lV9CXFW1bhR[0 + __rect_packed_top_L4lV9CXFW1bhR] . _location = 1U;
    goto __rect_func_L1_L4lV9CXFW1bhR;
    __rect_func_L2_L4lV9CXFW1bhR:
    0;
  }
  __dst_alloc_free__dmemclass_node(__rect_packed_var_L4lV9CXFW1bhR[0 + __rect_packed_top_L4lV9CXFW1bhR] . local0);
  goto __rect_func_L0_L4lV9CXFW1bhR;
}
/*
 * Implementation of Aho-Corasick Algorithm
 *
 * Search substrings in query, and fill matches in result arrays,
 * Assume all strings contain only letters 'a'-'z'.
 *
 * Parameter Format:
 *   substrings: multiple strings (target substrings) each with '%' ending.
 *               e.g. "string1%string2%", substring_length = 7+1+7+1.
 *   query: a string (the query document) with '%' ending
 *   substring_indexes: an output array, the indexes of found substrings.
 *   query_indexes: an output array, the corresponding indexes in query.
 */

void process_top(int *substring_length_p,char substrings[1024],char query[1024],int substring_indexes[1024],int query_indexes[1024],int *fallback)
{
  
#pragma HLS INTERFACE ap_memory port=substring_length_p depth=1
  
#pragma HLS INTERFACE ap_memory port=substrings depth=1024
  
#pragma HLS INTERFACE ap_memory port=query depth=1024
  
#pragma HLS INTERFACE ap_memory port=substring_indexes depth=1024
  
#pragma HLS INTERFACE ap_memory port=query_indexes depth=1024
  
#pragma HLS INTERFACE ap_memory port=fallback depth=1
  int substring_length =  *substring_length_p;
  __didxclass_node root = new_node();
  __didxc substring_buf = (__didxc )(__dst_alloc_malloc__dmemc(sizeof(char ) * substring_length));
  if (!substring_buf) {
    g_fallback = true;
    goto fail;
  }
  for (int i = 0; i < substring_length; i++) {
    (&(__dmemc + substring_buf + i) -> _data)[0U] = substrings[i];
  }
  for (int offset = 0; offset < substring_length; ) {
    offset += insert_node(root,substring_buf + offset,offset) + 1;
    if (g_fallback) 
      goto fail;
  }
  build_AhoCorasick(root,node_count);
  query_AhoCorasick(root,query,substring_indexes,query_indexes);
  delete_tree(root);
  if (g_fallback) 
    goto fail;
  fail:
   *fallback = g_fallback;
}
