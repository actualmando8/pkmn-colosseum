    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_801A19EC
    cmplwi r31, 0x0
    bne @L_801A19B8
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A19B8:
    lwz r0, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A19D8
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A19D8
    li r3, 0x1
@L_801A19D8:
    cmpwi r3, 0x0
    bne @L_801A19E4
    b @L_801A19EC
@L_801A19E4:
    mr r3, r31
    bl fn_8019D9DC
@L_801A19EC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
