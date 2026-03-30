    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r4, lbl_80478E70(r13)
    rlwinm r5, r3, 0, 16, 31
    lwz r0, 0x0(r4)
    cmplw r5, r0
    blt @L_8010C3C8
    lis r3, lbl_80272000@ha
    lis r4, lbl_8035B4E8@ha
    addi r3, r3, lbl_80272000@l
    addi r4, r4, lbl_8035B4E8@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_8010C3D4
@L_8010C3C8:
    lwz r3, lbl_80478E74(r13)
    rlwinm r0, r5, 3, 0, 28
    add r3, r3, r0
@L_8010C3D4:
    cmplwi r3, 0x0
    bne @L_8010C3E4
    li r3, 0x0
    b @L_8010C3EC
@L_8010C3E4:
    lbz r0, 0x0(r3)
    rlwinm r3, r0, 25, 31, 31
@L_8010C3EC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

