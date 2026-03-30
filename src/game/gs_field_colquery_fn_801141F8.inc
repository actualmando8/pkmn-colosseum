    nofralloc
    lbz r6, lbl_8047AD60(r13)
    lis r5, 0x6666
    lis r8, lbl_8035B818@ha
    addi r7, r6, 0x1
    extsb r9, r6
    rlwinm r6, r7, 0, 24, 31
    addi r0, r5, 0x6667
    extsb r5, r6
    rlwinm r9, r9, 3, 0, 28
    mulhw r0, r0, r5
    addi r6, r8, lbl_8035B818@l
    stbx r3, r6, r9
    add r6, r6, r9
    stb r7, lbl_8047AD60(r13)
    srawi r0, r0, 2
    rlwinm r3, r0, 1, 31, 31
    stw r4, 0x4(r6)
    add r0, r0, r3
    mulli r0, r0, 0xa
    subf r0, r0, r5
    extsb r0, r0
    stb r0, lbl_8047AD60(r13)
    blr

