    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x30(r1)
    stmw r26, 0x18(r1)
    li r4, 0x0
    li r3, 0x0
    li r0, 0x0
    li r28, 0x0
    li r29, 0x0
    b @L_800AC754
    @L_800AC754:
    lis r31, 0xcc00
    b @L_800AC75C
    @L_800AC75C:
    b @L_800AC8C0
    @L_800AC760:
    lwz r0, 0x6c00(r31)
    addi r30, r31, 0x6c00
    addi r30, r30, 0x8
    rlwinm r0, r0, 0, 27, 25
    ori r0, r0, 0x20
    stw r0, 0x6c00(r31)
    lwz r0, 0x6c00(r31)
    rlwinm r0, r0, 0, 31, 29
    stw r0, 0x6c00(r31)
    lwz r0, 0x6c00(r31)
    clrrwi r0, r0, 1
    ori r0, r0, 0x1
    stw r0, 0x6c00(r31)
    lwz r3, 0x0(r30)
    b @L_800AC79C
    @L_800AC79C:
    b @L_800AC7A0
    @L_800AC7A0:
    lwz r0, 0x0(r30)
    cmplw r3, r0
    beq @L_800AC7A0
    bl OSGetTime
    lwz r0, 0x6c00(r31)
    mr r26, r4
    mr r27, r3
    rlwinm r0, r0, 0, 31, 29
    ori r0, r0, 0x2
    stw r0, 0x6c00(r31)
    lwz r0, 0x6c00(r31)
    clrrwi r0, r0, 1
    ori r0, r0, 0x1
    stw r0, 0x6c00(r31)
    lwz r3, 0x0(r30)
    b @L_800AC7E0
    @L_800AC7E0:
    b @L_800AC7E4
    @L_800AC7E4:
    lwz r0, 0x0(r30)
    cmplw r3, r0
    beq @L_800AC7E4
    bl OSGetTime
    subfc r8, r26, r4
    lwz r12, lbl_8047A8E4(r13)
    lwz r5, 0x6c00(r31)
    subfe r7, r27, r3
    lwz r10, lbl_8047A904(r13)
    xoris r7, r7, 0x8000
    rlwinm r5, r5, 0, 31, 29
    lwz r11, lbl_8047A8E0(r13)
    subfc r6, r10, r12
    lwz r9, lbl_8047A900(r13)
    stw r5, 0x6c00(r31)
    subfe r0, r9, r11
    xoris r5, r0, 0x8000
    subfc r0, r6, r8
    lwz r0, 0x6c00(r31)
    subfe r5, r5, r7
    subfe r5, r7, r7
    neg r5, r5
    clrrwi r0, r0, 1
    cmpwi r5, 0x0
    stw r0, 0x6c00(r31)
    beq @L_800AC85C
    lwz r29, lbl_8047A8F0(r13)
    li r0, 0x1
    lwz r28, lbl_8047A8F4(r13)
    b @L_800AC8C0
    @L_800AC85C:
    addc r6, r12, r10
    adde r0, r11, r9
    xoris r5, r0, 0x8000
    subfc r0, r6, r8
    subfe r5, r5, r7
    subfe r5, r7, r7
    neg r5, r5
    cmpwi r5, 0x0
    bne @L_800AC8BC
    lwz r5, lbl_8047A8EC(r13)
    lwz r0, lbl_8047A8E8(r13)
    subfc r6, r10, r5
    subfe r0, r9, r0
    xoris r5, r0, 0x8000
    subfc r0, r6, r8
    subfe r5, r5, r7
    subfe r5, r7, r7
    neg r5, r5
    cmpwi r5, 0x0
    beq @L_800AC8BC
    lwz r29, lbl_8047A8F8(r13)
    li r0, 0x1
    lwz r28, lbl_8047A8FC(r13)
    b @L_800AC8C0
    @L_800AC8BC:
    li r0, 0x0
    @L_800AC8C0:
    cmplwi r0, 0x0
    beq @L_800AC760
    addc r27, r4, r28
    adde r26, r3, r29
    b @L_800AC8D4
    @L_800AC8D4:
    b @L_800AC8D8
    @L_800AC8D8:
    bl OSGetTime
    xoris r5, r3, 0x8000
    xoris r3, r26, 0x8000
    subfc r0, r27, r4
    subfe r3, r3, r5
    subfe r3, r5, r5
    neg r3, r3
    cmpwi r3, 0x0
    bne @L_800AC8D8
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    addi r1, r1, 0x30
    mtlr r0
    blr
