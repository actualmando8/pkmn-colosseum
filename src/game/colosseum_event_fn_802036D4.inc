    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    bne @L_802036F0
    li r3, 0x0
    b @L_80203720
@L_802036F0:
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80203710
    li r3, 0x0
    b @L_80203720
@L_80203710:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80203720:
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x61
    li r6, 0x0
    bl fn_8012640C
    lwz r0, 0x14(r1)
    rlwinm r3, r3, 0, 16, 31
    mtlr r0
    addi r1, r1, 0x10
    blr
