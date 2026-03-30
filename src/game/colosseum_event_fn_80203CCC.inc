    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    bne @L_80203CE8
    li r3, 0x0
    b @L_80203D18
@L_80203CE8:
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80203D08
    li r3, 0x0
    b @L_80203D18
@L_80203D08:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80203D18:
    cmplwi r3, 0x0
    bne @L_80203D28
    li r3, 0x0
    b @L_80203D2C
@L_80203D28:
    bl fn_80122DDC
@L_80203D2C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
