    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stw r31, 0x3c(r1)
    mr r31, r6
    stw r30, 0x38(r1)
    mr r30, r5
    stw r29, 0x34(r1)
    mr r29, r4
    stw r28, 0x30(r1)
    mr r28, r3
    lwz r7, lbl_8047AA80(r13)
    lwz r0, 0x0(r7)
    cmpwi r0, 0x1
    bne @L_800D9C84
    clrlwi r5, r28, 16
    clrlwi r6, r29, 16
    clrlwi r7, r30, 16
    clrlwi r8, r31, 16
    li r3, 0x38
    li r4, 0x4
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D9D48
    @L_800D9C84:
    clrlwi r7, r28, 16
    clrlwi r5, r29, 16
    clrlwi r3, r30, 16
    clrlwi r0, r31, 16
    subf r4, r7, r3
    lis r6, 0x4330
    subf r3, r5, r0
    stw r7, 0xc(r1)
    addi r4, r4, 0x1
    lfd f2, lbl_8047CA60(r2)
    addi r0, r3, 0x1
    stw r6, 0x8(r1)
    xoris r4, r4, 0x8000
    lfd f4, lbl_8047CA68(r2)
    xoris r0, r0, 0x8000
    lfd f0, 0x8(r1)
    stw r5, 0x14(r1)
    li r3, 0x0
    fsubs f1, f0, f2
    lfs f5, lbl_8047CA50(r2)
    stw r6, 0x10(r1)
    lfs f6, lbl_8047CA58(r2)
    lfd f0, 0x10(r1)
    stw r4, 0x1c(r1)
    fsubs f2, f0, f2
    stw r6, 0x18(r1)
    lfd f0, 0x18(r1)
    stw r0, 0x24(r1)
    fsubs f3, f0, f4
    stw r6, 0x20(r1)
    lfd f0, 0x20(r1)
    fsubs f4, f0, f4
    bl fn_800D3EC4
    lwz r3, lbl_8047AA80(r13)
    sth r28, 0x476(r3)
    lwz r3, lbl_8047AA80(r13)
    sth r29, 0x478(r3)
    lwz r3, lbl_8047AA80(r13)
    sth r30, 0x47a(r3)
    lwz r3, lbl_8047AA80(r13)
    sth r31, 0x47c(r3)
    bl fn_800D2584
    cmplwi r3, 0x0
    beq @L_800D9D48
    mr r4, r28
    mr r5, r29
    mr r6, r30
    mr r7, r31
    bl fn_800D21C8
    @L_800D9D48:
    lwz r0, 0x44(r1)
    lwz r31, 0x3c(r1)
    lwz r30, 0x38(r1)
    lwz r29, 0x34(r1)
    lwz r28, 0x30(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
