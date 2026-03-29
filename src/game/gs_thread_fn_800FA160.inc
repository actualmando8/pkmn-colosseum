    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, lbl_8047CD04(r2)
    addi r4, r1, 0x8
    lwz r8, 0x24(r3)
    li r3, 0x0
    stw r0, 0xc(r1)
    srwi r7, r8, 24
    extrwi r6, r8, 8, 8
    extrwi r5, r8, 8, 16
    clrlwi r0, r8, 24
    stb r7, 0xc(r1)
    stb r6, 0xd(r1)
    stb r5, 0xe(r1)
    stb r0, 0xf(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x8(r1)
    bl fn_800DBEB4
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
