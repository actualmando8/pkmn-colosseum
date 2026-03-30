    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r6
    stw r30, 0x18(r1)
    mr r30, r5
    stw r29, 0x14(r1)
    mr r29, r4
    stw r28, 0x10(r1)
    mr r28, r3
    lwz r7, lbl_8047AA80(r13)
    lwz r0, 0x0(r7)
    cmpwi r0, 0x1
    bne @L_800D9DC8
    clrlwi r5, r28, 16
    clrlwi r6, r29, 16
    clrlwi r7, r30, 16
    clrlwi r8, r31, 16
    li r3, 0x37
    li r4, 0x4
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D9E2C
    @L_800D9DC8:
    clrlwi r3, r28, 16
    clrlwi r0, r30, 16
    subf r5, r3, r0
    clrlwi r4, r29, 16
    clrlwi r0, r31, 16
    subf r6, r4, r0
    addi r5, r5, 0x1
    addi r6, r6, 0x1
    bl fn_800BD7A0
    lwz r3, lbl_8047AA80(r13)
    sth r28, 0x46e(r3)
    lwz r3, lbl_8047AA80(r13)
    sth r29, 0x470(r3)
    lwz r3, lbl_8047AA80(r13)
    sth r30, 0x472(r3)
    lwz r3, lbl_8047AA80(r13)
    sth r31, 0x474(r3)
    bl fn_800D2584
    cmplwi r3, 0x0
    beq @L_800D9E2C
    mr r4, r28
    mr r5, r29
    mr r6, r30
    mr r7, r31
    bl fn_800D2150
    @L_800D9E2C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
