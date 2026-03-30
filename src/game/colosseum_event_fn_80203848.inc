    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    bne @L_80203864
    li r3, 0x0
    b @L_80203874
@L_80203864:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80203874:
    cmplwi r3, 0x0
    bne @L_80203884
    li r3, 0x0
    b @L_80203894
@L_80203884:
    li r4, 0x0
    li r5, 0x77
    li r6, 0x0
    bl fn_8012640C
@L_80203894:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
