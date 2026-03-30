    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x40
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    bl fn_800E3534
    rlwinm. r0, r3, 0, 16, 31
    mr r31, r3
    beq @L_801DBFF8
    bl fn_800E27B0
    li r4, 0x0
    mr r30, r3
    li r5, 0x40
    bl memset
    sth r31, 0x2a(r30)
    mr r3, r30
    b @L_801DBFFC
@L_801DBFF8:
    li r3, 0x0
@L_801DBFFC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
