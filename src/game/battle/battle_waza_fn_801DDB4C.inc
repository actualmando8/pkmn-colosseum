    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr. r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    bne @L_801DDB78
    li r3, 0x0
    b @L_801DDBF4
@L_801DDB78:
    bl fn_801DBFB0
    mr. r31, r3
    bne @L_801DDB8C
    li r3, 0x0
    b @L_801DDBF4
@L_801DDB8C:
    li r0, 0x0
    mr r4, r30
    sth r0, 0x2c(r31)
    sth r0, 0x2e(r31)
    sth r0, 0x30(r31)
    stw r29, 0x3c(r31)
    bl fn_801DC014
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801DDBC0
    mr r3, r31
    bl fn_801DBDDC
    li r3, 0x0
    b @L_801DDBF4
@L_801DDBC0:
    li r0, 0x0
    stb r0, 0x14(r31)
    stb r0, 0x15(r31)
    lwz r0, 0x68(r29)
    stw r0, 0x34(r31)
    lwz r3, 0x68(r29)
    cmplwi r3, 0x0
    beq @L_801DDBE4
    stw r31, 0x38(r3)
@L_801DDBE4:
    li r0, 0x0
    li r3, 0x1
    stw r0, 0x38(r31)
    stw r31, 0x68(r29)
@L_801DDBF4:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
