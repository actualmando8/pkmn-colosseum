    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    bne @L_80203A88
    li r3, 0x0
    b @L_80203AB8
@L_80203A88:
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80203AA8
    li r3, 0x0
    b @L_80203AB8
@L_80203AA8:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80203AB8:
    cmplwi r3, 0x0
    bne @L_80203AC8
    li r3, 0x0
    b @L_80203ACC
@L_80203AC8:
    bl fn_80122A70
@L_80203ACC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
