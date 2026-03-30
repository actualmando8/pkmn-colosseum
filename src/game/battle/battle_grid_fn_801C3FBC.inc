    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r6, lbl_80466DE8@ha
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    addi r31, r6, lbl_80466DE8@l
    mr r27, r3
    mr r28, r4
    mr r29, r5
    lhz r0, 0x40(r31)
    cmplwi r0, 0x4
    bge @L_801C4064
    lwz r0, 0x0(r31)
    mr r30, r31
    cmplwi r0, 0x0
    beq @L_801C4028
    addi r30, r31, 0x10
    lwz r0, 0x10(r31)
    cmplwi r0, 0x0
    beq @L_801C4028
    lwzu r0, 0x10(r30)
    cmplwi r0, 0x0
    beq @L_801C4028
    lwzu r0, 0x10(r30)
    cmplwi r0, 0x0
    beq @L_801C4028
    addi r30, r30, 0x10
    @L_801C4028:
    mr r3, r30
    li r4, 0x0
    li r5, 0x10
    bl memset
    stw r27, 0x0(r30)
    clrlwi. r0, r28, 24
    li r0, 0x1
    stb r28, 0xe(r30)
    stb r29, 0xf(r30)
    beq @L_801C4054
    li r0, -0x1
    @L_801C4054:
    stb r0, 0x76(r27)
    lhz r3, 0x40(r31)
    addi r0, r3, 0x1
    sth r0, 0x40(r31)
    @L_801C4064:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
