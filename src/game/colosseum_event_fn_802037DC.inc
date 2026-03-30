    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0xd6
    stw r0, 0x14(r1)
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80203808
    li r3, 0x0
    b @L_80203818
@L_80203808:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80203818:
    cmplwi r3, 0x0
    bne @L_80203828
    li r3, 0x0
    b @L_80203838
@L_80203828:
    li r4, 0x0
    li r5, 0x77
    li r6, 0x0
    bl fn_8012640C
@L_80203838:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
