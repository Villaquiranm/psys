void map_page(unsigned *pdir, void *physaddr, unsigned virtualaddr, unsigned flags){

  unsigned pd_index = virtualaddr >> 22;
  unsigned pt_index = (virtualaddr >> 12) & 0x3FFu;

  /* Get page table */
  unsigned *ptable = (unsigned*) (pdir[pd_index] & 0xFFFFF000);
  ptable[pt_index] = ((unsigned)physaddr & 0xFFFFF000) | flags;
}
