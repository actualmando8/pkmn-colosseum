    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0xd6
    stw r0, 0x14(r1)
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80203E38
    li r3, 0x0
    b @L_80203E48
@L_80203E38:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80203E48:
    cmplwi r3, 0x0
    bne @L_80203E58
    li r3, 0x0
    b @L_80203E6C
@L_80203E58:
    li r4, 0x0
    li r5, 0x7a
    li r6, 0x0
    bl fn_8012640C
    rlwinm r3, r3, 0, 24, 31
@L_80203E6C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
