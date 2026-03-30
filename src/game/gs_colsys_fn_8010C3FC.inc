    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, lbl_80478E70(r13)
    rlwinm r5, r3, 0, 16, 31
    lwz r0, 0x0(r4)
    cmplw r5, r0
    blt @L_8010C43C
    lis r3, lbl_80272000@ha
    lis r4, lbl_8035B4E8@ha
    addi r3, r3, lbl_80272000@l
    addi r4, r4, lbl_8035B4E8@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_8010C448
@L_8010C43C:
    lwz r3, lbl_80478E74(r13)
    rlwinm r0, r5, 3, 0, 28
    add r3, r3, r0
@L_8010C448:
    cmplwi r3, 0x0
    bne @L_8010C458
    li r3, 0x0
    b @L_8010C45C
@L_8010C458:
    lwz r3, 0x4(r3)
@L_8010C45C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

