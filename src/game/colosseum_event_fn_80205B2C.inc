    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    bne @L_80205B48
    li r3, -0x1
    b @L_80205B7C
@L_80205B48:
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80205B68
    li r3, -0x1
    b @L_80205B7C
@L_80205B68:
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    extsh r3, r3
@L_80205B7C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
