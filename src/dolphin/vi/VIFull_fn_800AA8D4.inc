    nofralloc
    mflr r0
    lis r6, lbl_803FC5D0@ha
    stw r0, 0x4(r1)
    clrlwi. r0, r4, 28
    stwu r1, -0x40(r1)
    stw r31, 0x3c(r1)
    lis r31, 0x8000
    stw r30, 0x38(r1)
    addi r30, r6, lbl_803FC5D0@l
    stw r29, 0x34(r1)
    stw r28, 0x30(r1)
    lwz r29, lbl_80478A0C(r13)
    lwz r5, lbl_8047A8AC(r13)
    srw r28, r31, r29
    andc r3, r5, r28
    stw r3, lbl_8047A8AC(r13)
    and r5, r5, r28
    li r3, 0x1
    beq @L_800AA970
    lwz r4, lbl_8047A8A8(r13)
    cntlzw r0, r4
    stw r0, lbl_80478A0C(r13)
    lwz r3, lbl_80478A0C(r13)
    cmpwi r3, 0x20
    beq @L_800AABE0
    mulli r0, r3, 0xc
    srw r3, r31, r3
    andc r4, r4, r3
    add r3, r30, r0
    stw r4, lbl_8047A8A8(r13)
    li r4, 0x0
    li r5, 0xc
    addi r3, r3, 0x10
    bl memset
    lis r4, fn_800AA8D4@ha
    lwz r3, lbl_80478A0C(r13)
    addi r4, r4, fn_800AA8D4@l
    bl fn_800D0CBC
    b @L_800AABE0
    @L_800AA970:
    clrrwi r6, r4, 8
    rlwinm r4, r4, 0, 3, 4
    subis r0, r4, 0x800
    slwi r4, r29, 2
    cmplwi r0, 0x0
    stwx r6, r30, r4
    bne @L_800AA994
    rlwinm. r0, r6, 0, 7, 7
    bne @L_800AA9E8
    @L_800AA994:
    lwz r5, lbl_8047A8A8(r13)
    cntlzw r0, r5
    stw r0, lbl_80478A0C(r13)
    lwz r4, lbl_80478A0C(r13)
    cmpwi r4, 0x20
    beq @L_800AABE0
    lis r3, 0x8000
    mulli r0, r4, 0xc
    srw r3, r3, r4
    andc r4, r5, r3
    add r3, r30, r0
    stw r4, lbl_8047A8A8(r13)
    li r4, 0x0
    li r5, 0xc
    addi r3, r3, 0x10
    bl memset
    lis r4, fn_800AA8D4@ha
    lwz r3, lbl_80478A0C(r13)
    addi r4, r4, fn_800AA8D4@l
    bl fn_800D0CBC
    b @L_800AABE0
    @L_800AA9E8:
    lwz r0, lbl_80478A18(r13)
    cmplwi r0, 0x2
    bge @L_800AAA74
    lwz r0, lbl_8047A8A4(r13)
    addi r3, r29, 0x0
    addi r4, r1, 0x1c
    or r0, r0, r28
    stw r0, lbl_8047A8A4(r13)
    bl fn_800D05A4
    lwz r0, lbl_80478A14(r13)
    addi r3, r29, 0x0
    oris r4, r0, 0x40
    bl fn_800D0338
    lwz r3, lbl_8047A8A4(r13)
    bl fn_800D03C8
    lwz r4, lbl_8047A8A8(r13)
    cntlzw r0, r4
    stw r0, lbl_80478A0C(r13)
    lwz r3, lbl_80478A0C(r13)
    cmpwi r3, 0x20
    beq @L_800AABE0
    mulli r0, r3, 0xc
    srw r3, r31, r3
    andc r4, r4, r3
    add r3, r30, r0
    stw r4, lbl_8047A8A8(r13)
    li r4, 0x0
    li r5, 0xc
    addi r3, r3, 0x10
    bl memset
    lis r4, fn_800AA8D4@ha
    lwz r3, lbl_80478A0C(r13)
    addi r4, r4, fn_800AA8D4@l
    bl fn_800D0CBC
    b @L_800AABE0
    @L_800AAA74:
    clrrwi. r0, r6, 31
    beq @L_800AAA84
    rlwinm. r0, r6, 0, 5, 5
    beq @L_800AAAF4
    @L_800AAA84:
    cmplwi r5, 0x0
    beq @L_800AAAC0
    mulli r0, r29, 0xc
    lis r3, fn_800AA678@ha
    add r6, r30, r0
    addi r8, r3, fn_800AA678@l
    addi r3, r29, 0x0
    la r4, lbl_80478A24(r13)
    li r5, 0x3
    li r7, 0xa
    li r10, 0x0
    li r9, 0x0
    addi r6, r6, 0x10
    bl SITransfer
    b @L_800AAB7C
    @L_800AAAC0:
    mulli r0, r29, 0xc
    lis r3, fn_800AA678@ha
    add r6, r30, r0
    addi r8, r3, fn_800AA678@l
    addi r3, r29, 0x0
    la r4, lbl_80478A20(r13)
    li r5, 0x1
    li r7, 0xa
    li r10, 0x0
    li r9, 0x0
    addi r6, r6, 0x10
    bl SITransfer
    b @L_800AAB7C
    @L_800AAAF4:
    rlwinm. r0, r6, 0, 11, 11
    beq @L_800AAB7C
    rlwinm. r0, r6, 0, 12, 12
    bne @L_800AAB7C
    rlwinm. r0, r6, 0, 13, 13
    bne @L_800AAB7C
    rlwinm. r0, r6, 0, 1, 1
    beq @L_800AAB48
    mulli r0, r29, 0xc
    lis r3, fn_800AA678@ha
    add r6, r30, r0
    addi r8, r3, fn_800AA678@l
    addi r3, r29, 0x0
    la r4, lbl_80478A20(r13)
    li r5, 0x1
    li r7, 0xa
    li r10, 0x0
    li r9, 0x0
    addi r6, r6, 0x10
    bl SITransfer
    b @L_800AAB7C
    @L_800AAB48:
    mulli r0, r29, 0xc
    lis r3, fn_800AA7FC@ha
    add r4, r30, r4
    add r6, r30, r0
    addi r8, r3, fn_800AA7FC@l
    addi r3, r29, 0x0
    li r5, 0x3
    li r7, 0x8
    li r10, 0x0
    li r9, 0x0
    addi r4, r4, 0x40
    addi r6, r6, 0x10
    bl SITransfer
    @L_800AAB7C:
    cmpwi r3, 0x0
    bne @L_800AABE0
    lwz r5, lbl_8047A8A8(r13)
    lwz r3, lbl_8047A8B8(r13)
    cntlzw r0, r5
    stw r0, lbl_80478A0C(r13)
    or r0, r3, r28
    lwz r4, lbl_80478A0C(r13)
    stw r0, lbl_8047A8B8(r13)
    cmpwi r4, 0x20
    beq @L_800AABE0
    lis r3, 0x8000
    mulli r0, r4, 0xc
    srw r3, r3, r4
    andc r4, r5, r3
    add r3, r30, r0
    stw r4, lbl_8047A8A8(r13)
    li r4, 0x0
    li r5, 0xc
    addi r3, r3, 0x10
    bl memset
    lis r4, fn_800AA8D4@ha
    lwz r3, lbl_80478A0C(r13)
    addi r4, r4, fn_800AA8D4@l
    bl fn_800D0CBC
    @L_800AABE0:
    lwz r0, 0x44(r1)
    lwz r31, 0x3c(r1)
    lwz r30, 0x38(r1)
    lwz r29, 0x34(r1)
    lwz r28, 0x30(r1)
    addi r1, r1, 0x40
    mtlr r0
    blr
