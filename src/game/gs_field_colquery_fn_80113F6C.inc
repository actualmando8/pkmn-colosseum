    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    bl fn_80115C48
    cmplwi r3, 0x0
    bne @L_80113F94
    li r3, 0x0
    b @L_80113FA0
@L_80113F94:
    bl fn_80115A80
    mr r4, r31
    bl fn_800F9318
@L_80113FA0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
