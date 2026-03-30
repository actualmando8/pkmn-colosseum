    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r26, r4
    mr r27, r5
    mr r31, r3
    bne @L_801345B8
    li r3, 0x0
    li r4, 0x3
    bl fn_80129280
    mr r31, r3
    @L_801345B8:
    clrlwi. r0, r27, 16
    bne @L_801345C8
    mr r3, r27
    b @L_80134668
    @L_801345C8:
    mr r3, r26
    bl fn_801440A0
    cmplwi r3, 0x0
    bne @L_801345E0
    mr r3, r27
    b @L_80134668
    @L_801345E0:
    mr r29, r31
    clrlwi r28, r26, 16
    li r30, 0x0
    @L_801345EC:
    addi r3, r29, 0x6dec
    bl fn_801429E8
    clrlwi. r0, r3, 24
    beq @L_80134618
    addi r3, r29, 0x6dec
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    cmpw r28, r3
    beq @L_80134628
    @L_80134618:
    addi r30, r30, 0x1
    addi r29, r29, 0x4
    cmpwi r30, 0xeb
    blt @L_801345EC
    @L_80134628:
    cmpwi r30, 0xeb
    li r7, -0x1
    bge @L_80134638
    extsh r7, r30
    @L_80134638:
    extsh. r0, r7
    bge @L_80134648
    mr r3, r27
    b @L_80134668
    @L_80134648:
    mr r5, r26
    mr r6, r27
    addi r3, r31, 0x6dec
    li r4, 0xeb
    li r8, 0x3e7
    li r9, 0x0
    bl fn_80140ACC
    clrlwi r3, r3, 16
    @L_80134668:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
