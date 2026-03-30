    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    bne @L_80203C78
    li r3, 0x0
    b @L_80203CA8
@L_80203C78:
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80203C98
    li r3, 0x0
    b @L_80203CA8
@L_80203C98:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80203CA8:
    cmplwi r3, 0x0
    bne @L_80203CB8
    li r3, 0x0
    b @L_80203CBC
@L_80203CB8:
    bl fn_80122C64
@L_80203CBC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
