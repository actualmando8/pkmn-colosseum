    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    bne @L_80203B00
    li r3, 0x0
    b @L_80203B30
@L_80203B00:
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80203B20
    li r3, 0x0
    b @L_80203B30
@L_80203B20:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80203B30:
    cmplwi r3, 0x0
    bne @L_80203B40
    li r3, 0x0
    b @L_80203B48
@L_80203B40:
    mr r4, r31
    bl fn_80122AE0
@L_80203B48:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
