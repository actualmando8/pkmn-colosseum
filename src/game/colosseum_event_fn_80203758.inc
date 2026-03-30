    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0xd6
    stw r0, 0x14(r1)
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80203784
    li r3, 0x0
    b @L_80203794
@L_80203784:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80203794:
    cmplwi r3, 0x0
    bne @L_802037A4
    li r3, 0x0
    b @L_802037CC
@L_802037A4:
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8012640C
    bl fn_800FA280
@L_802037CC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
