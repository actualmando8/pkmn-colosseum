    nofralloc
    lbz r3, lbl_8047A470(r13)
    li r0, 0x1
    clrlwi r3, r3, 24
    stb r0, lbl_8047A470(r13)
    subfic r0, r3, 0x1
    blr
