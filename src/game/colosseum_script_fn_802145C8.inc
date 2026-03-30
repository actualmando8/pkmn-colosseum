    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    bl fn_801F025C
    mr r0, r3
    li r3, 0x12
    mr r29, r0
    li r4, 0x0
    bl fn_801F025C
    mr r31, r3
    li r3, 0x19
    li r4, 0x0
    bl fn_801F025C
    mr r27, r3
    mr r28, r29
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x47
    li r6, 0x0
    bl fn_801F4C14
    subf r0, r29, r31
    cntlzw r0, r0
    rlwinm. r0, r0, 27, 5, 31
    beq @L_80214640
    mr r30, r27
    mr r29, r27
@L_80214640:
    cmpwi r0, 0x0
    bne @L_8021464C
    mr r30, r27
@L_8021464C:
    mr r7, r28
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
