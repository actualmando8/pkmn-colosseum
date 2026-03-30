    nofralloc
    cmplwi r3, 0x0
    beqlr
    li r6, 0x0
    li r5, 0x0
    li r4, 0x1
    b @L_800F040C
    @L_800F03EC:
    lwz r0, lbl_8047AC28(r13)
    add r7, r0, r5
    lwz r0, 0xc(r7)
    cmplw r0, r3
    bne @L_800F0404
    stb r4, 0xa(r7)
    @L_800F0404:
    addi r5, r5, 0x24
    addi r6, r6, 0x1
    @L_800F040C:
    lwz r0, lbl_8047AC30(r13)
    cmplw r6, r0
    blt @L_800F03EC
    li r0, 0x1
    stb r0, lbl_8047AC0C(r13)
    blr
