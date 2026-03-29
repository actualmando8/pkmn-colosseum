    nofralloc
    lbz r6, 0x67(r4)
    lis r5, 0x8081
    lbz r0, 0x8b(r3)
    subi r10, r5, 0x7f7f
    lbz r8, 0x66(r4)
    lbz r7, 0x8a(r3)
    mullw r9, r6, r0
    lbz r6, 0x64(r4)
    lbz r5, 0x88(r3)
    lbz r4, 0x65(r4)
    lbz r0, 0x89(r3)
    mullw r3, r8, r7
    mullw r5, r6, r5
    mullw r0, r4, r0
    mulhw r4, r10, r9
    mulhw r6, r10, r3
    add r4, r4, r9
    srawi r7, r4, 7
    mulhw r4, r10, r5
    srwi r8, r7, 31
    add r3, r6, r3
    add r7, r7, r8
    srawi r6, r3, 7
    clrlwi r7, r7, 24
    mulhw r3, r10, r0
    add r4, r4, r5
    srwi r5, r6, 31
    srawi r4, r4, 7
    add r6, r6, r5
    srwi r5, r4, 31
    add r0, r3, r0
    add r4, r4, r5
    srawi r0, r0, 7
    clrlwi r5, r6, 24
    srwi r3, r0, 31
    clrlwi r4, r4, 24
    add r0, r0, r3
    slwi r5, r5, 8
    clrlwi r0, r0, 24
    slwi r3, r4, 24
    slwi r0, r0, 16
    or r0, r3, r0
    or r0, r5, r0
    or r3, r7, r0
    blr
