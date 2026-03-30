    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0xd6
    stw r0, 0x14(r1)
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80203D68
    li r3, 0x0
    b @L_80203D78
@L_80203D68:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80203D78:
    cmplwi r3, 0x0
    bne @L_80203D88
    li r3, 0x0
    b @L_80203D9C
@L_80203D88:
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
@L_80203D9C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
