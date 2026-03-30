    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r5
    stw r30, 0x8(r1)
    mr r30, r4
    bne @L_80134F20
    li r3, 0x0
    li r4, 0x3
    bl fn_80129280
    @L_80134F20:
    extsb. r0, r30
    blt @L_80134F34
    extsb r4, r30
    cmpwi r4, 0x3
    blt @L_80134F3C
    @L_80134F34:
    li r0, 0x0
    b @L_80134F6C
    @L_80134F3C:
    extsb. r0, r31
    blt @L_80134F50
    extsb r0, r31
    cmpwi r0, 0x1e
    blt @L_80134F58
    @L_80134F50:
    li r0, 0x0
    b @L_80134F6C
    @L_80134F58:
    mulli r4, r4, 0x24a4
    mulli r0, r0, 0x138
    add r4, r4, r0
    addi r0, r4, 0x14
    add r0, r3, r0
    @L_80134F6C:
    mr r3, r0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
