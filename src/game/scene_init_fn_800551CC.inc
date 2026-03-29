    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r26, r3
    mr r27, r4
    mr r28, r5
    lwz r0, 0x0(r28)
    lwz r30, 0x4(r28)
    cmpwi r0, 0x0
    beq @L_800551FC
    b @L_800552C0
    @L_800551FC:
    cmpwi r27, 0x1
    bne @L_80055254
    lwz r3, 0x8(r28)
    subi r31, r3, 0x1
    b @L_80055238
    @L_80055210:
    extsb r4, r30
    extsb r5, r31
    li r3, 0x0
    bl fn_80134EF0
    mr r29, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80055240
    subi r31, r31, 0x1
    @L_80055238:
    cmpwi r31, 0x0
    bge @L_80055210
    @L_80055240:
    cmpwi r31, 0x0
    blt @L_80055254
    stw r31, 0x8(r28)
    mr r3, r29
    b @L_800552C0
    @L_80055254:
    cmpwi r27, 0x2
    bne @L_800552BC
    lwz r3, 0x8(r28)
    addi r31, r3, 0x1
    b @L_80055290
    @L_80055268:
    extsb r4, r30
    extsb r5, r31
    li r3, 0x0
    bl fn_80134EF0
    mr r29, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_800552A0
    addi r31, r31, 0x1
    @L_80055290:
    bl fn_801347D8
    extsb r0, r3
    cmpw r31, r0
    blt @L_80055268
    @L_800552A0:
    bl fn_801347D8
    extsb r0, r3
    cmpw r31, r0
    bge @L_800552BC
    stw r31, 0x8(r28)
    mr r3, r29
    b @L_800552C0
    @L_800552BC:
    mr r3, r26
    @L_800552C0:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
