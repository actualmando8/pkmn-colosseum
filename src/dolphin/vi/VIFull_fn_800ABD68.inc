    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stw r31, 0x24(r1)
    stw r30, 0x20(r1)
    addi r30, r3, 0x0
    lwz r0, lbl_8047A8BC(r13)
    cmplwi r0, 0x0
    beq @L_800ABD94
    li r3, 0x0
    bl fn_800ABF5C
    @L_800ABD94:
    cmpwi r30, 0x0
    bne @L_800ABED8
    lwz r0, lbl_8047A8A8(r13)
    li r30, 0x0
    addi r3, r30, 0x0
    cmplwi r0, 0x0
    bne @L_800ABDC0
    lwz r0, lbl_80478A0C(r13)
    cmpwi r0, 0x20
    bne @L_800ABDC0
    li r3, 0x1
    @L_800ABDC0:
    cmpwi r3, 0x0
    beq @L_800ABDD8
    bl fn_800CF708
    cmpwi r3, 0x0
    bne @L_800ABDD8
    li r30, 0x1
    @L_800ABDD8:
    lwz r0, lbl_8047A8C0(r13)
    cmpwi r0, 0x0
    bne @L_800ABED0
    cmpwi r30, 0x0
    beq @L_800ABED0
    lis r30, 0xf000
    bl OSDisableInterrupts
    lwz r6, lbl_8047A8B8(r13)
    li r7, 0x0
    lwz r5, lbl_8047A8B0(r13)
    lis r4, 0x8000
    lwz r0, lbl_8047A8B4(r13)
    or r30, r30, r6
    lwz r6, lbl_8047A8A8(r13)
    or r5, r5, r0
    lbz r0, 0x30e3(r4)
    andc r30, r30, r5
    lwz r5, lbl_8047A8A4(r13)
    or r4, r6, r30
    stw r7, lbl_8047A8B8(r13)
    rlwinm. r0, r0, 0, 25, 25
    stw r4, lbl_8047A8A8(r13)
    andc r4, r5, r30
    addi r31, r3, 0x0
    lwz r6, lbl_8047A8A8(r13)
    stw r4, lbl_8047A8A4(r13)
    and r3, r6, r5
    bne @L_800ABE54
    lwz r0, lbl_8047A8AC(r13)
    or r0, r0, r30
    stw r0, lbl_8047A8AC(r13)
    @L_800ABE54:
    bl fn_800D0464
    lwz r0, lbl_80478A0C(r13)
    cmpwi r0, 0x20
    bne @L_800ABEB8
    lwz r5, lbl_8047A8A8(r13)
    cntlzw r0, r5
    stw r0, lbl_80478A0C(r13)
    lwz r4, lbl_80478A0C(r13)
    cmpwi r4, 0x20
    beq @L_800ABEB8
    lis r0, 0x8000
    srw r0, r0, r4
    andc r0, r5, r0
    mulli r4, r4, 0xc
    stw r0, lbl_8047A8A8(r13)
    lis r3, lbl_803FC5E0@ha
    addi r0, r3, lbl_803FC5E0@l
    add r3, r0, r4
    li r4, 0x0
    li r5, 0xc
    bl memset
    lis r4, fn_800AA8D4@ha
    lwz r3, lbl_80478A0C(r13)
    addi r4, r4, fn_800AA8D4@l
    bl fn_800D0CBC
    @L_800ABEB8:
    mr r3, r31
    bl OSRestoreInterrupts
    li r0, 0x1
    stw r0, lbl_8047A8C0(r13)
    li r3, 0x0
    b @L_800ABEE4
    @L_800ABED0:
    mr r3, r30
    b @L_800ABEE4
    @L_800ABED8:
    li r0, 0x0
    stw r0, lbl_8047A8C0(r13)
    li r3, 0x1
    @L_800ABEE4:
    lwz r0, 0x2c(r1)
    lwz r31, 0x24(r1)
    lwz r30, 0x20(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
