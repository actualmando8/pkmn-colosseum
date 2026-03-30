    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r3, 0x1
    subi r3, r3, 0x5740
    bl fn_800E3534
    rlwinm r0, r3, 0, 16, 31
    sth r3, lbl_8047AD18(r13)
    cmplwi r0, 0x0
    bne @L_8010939C
    lis r3, lbl_80271F18@ha
    lis r4, lbl_8035B3F0@ha
    addi r3, r3, lbl_80271F18@l
    addi r4, r4, lbl_8035B3F0@l
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801093B8
@L_8010939C:
    mr r3, r0
    bl fn_800E27B0
    lis r4, 0x1
    stw r3, lbl_8047AD1C(r13)
    subi r5, r4, 0x5740
    li r4, 0x0
    bl memset
@L_801093B8:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

