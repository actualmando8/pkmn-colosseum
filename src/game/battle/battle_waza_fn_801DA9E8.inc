    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r6, -32698
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r26, r3
    addi r3, r6, 31936
    mr r27, r4
    mr r28, r5
    li r30, 0x0
    lwz r31, 0x0(r3)
    lhz r29, 0x4(r3)
    b @L_801DAA58
@L_801DAA1C:
    lbz r0, 0x74(r31)
    cmplwi r0, 0x0
    beq @L_801DAA50
    mr r3, r31
    mr r4, r27
    mr r5, r28
    bl fn_801DD0C8
    cmplwi r3, 0x0
    beq @L_801DAA50
    lbz r0, 0x14(r3)
    cmplwi r0, 0x0
    beq @L_801DAA50
    bl fn_801DBB10
@L_801DAA50:
    addi r30, r30, 0x1
    addi r31, r31, 0x8c
@L_801DAA58:
    cmpw r30, r29
    blt @L_801DAA1C
    cmplwi r26, 0x0
    beq @L_801DAA98
    mr r3, r26
    mr r4, r27
    mr r5, r28
    bl fn_801DD0C8
    cmplwi r26, 0x0
    beq @L_801DAA8C
    li r0, 0x0
    stb r0, 0x16(r26)
    stb r0, 0x17(r26)
@L_801DAA8C:
    cmplwi r3, 0x0
    beq @L_801DAA98
    bl fn_801DBCCC
@L_801DAA98:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
