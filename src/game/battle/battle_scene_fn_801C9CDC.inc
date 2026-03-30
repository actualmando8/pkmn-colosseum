    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    mr r30, r4
    bl fn_800F92D4
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_801C9D28
    lis r3, -32729
    mr r4, r29
    addi r3, r3, 22700
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_801C9D58
@L_801C9D28:
    mr r3, r31
    bl fn_800DCA9C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801C9D44
    li r3, 0x0
    b @L_801C9D58
@L_801C9D44:
    cmpwi r30, 0x0
    beq @L_801C9D54
    bl fn_800F0308
    b @L_801C9D28
@L_801C9D54:
    li r3, 0x0
@L_801C9D58:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
