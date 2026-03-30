    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x24(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r28, 0x10(r1)
    bl fn_801F54A4
    lwz r5, lbl_8047B610(r13)
    rlwinm r28, r3, 0, 16, 31
    li r4, 0x0
    lbz r3, 0x1(r5)
    lwz r30, 0x2(r5)
    bl fn_801F025C
    mr r0, r3
    li r3, 0x0
    mr r29, r0
    mr r4, r29
    bl fn_801F4354
    mr r0, r3
    mr r3, r29
    mr r31, r0
    li r4, 0x0
    li r5, 0x119
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021F2D4
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x6
    stw r0, lbl_8047B610(r13)
    b @L_8021F388
@L_8021F2D4:
    li r0, 0x0
    mr r3, r31
    sth r0, 0x8(r1)
    addi r4, r1, 0x8
    bl fn_801F8A18
    cmplwi r3, 0x0
    bne @L_8021F2F8
    stw r30, lbl_8047B610(r13)
    b @L_8021F388
@L_8021F2F8:
    mr r3, r31
    mr r4, r28
    mr r6, r29
    li r5, 0x0
    bl fn_801F9600
    mr r31, r3
    extsh. r0, r31
    bge @L_8021F334
    extsh r0, r31
    cmpwi r0, -0x2
    bne @L_8021F32C
    li r3, 0x0
    bl fn_801F150C
@L_8021F32C:
    stw r30, lbl_8047B610(r13)
    b @L_8021F388
@L_8021F334:
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x45
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r29
    extsh r7, r31
    li r4, 0x0
    li r5, 0x121
    li r6, 0x0
    bl fn_801254B4
    mr r3, r29
    li r4, 0x0
    li r5, 0x119
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x6
    stw r0, lbl_8047B610(r13)
@L_8021F388:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
