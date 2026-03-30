    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lbz r4, -21592(r13)
    addi r3, r4, 0x1
    extsb r0, r4
    stb r3, -21592(r13)
    oris r31, r0, 0x7ffe
    bl fn_80113F48
    mr r4, r31
    mr r5, r30
    bl fn_8018E050
    cmplwi r3, 0x0
    bne @L_801CBA58
    li r3, 0x0
    b @L_801CBA6C
@L_801CBA58:
    bl fn_8018D928
    bl fn_8018FBD4
    li r4, 0x0
    bl fn_800E3C94
    mr r3, r31
@L_801CBA6C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
