    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    stw r30, 0x10(r1)
    mr r30, r3
    bl OSDisableInterrupts
    lwz r4, lbl_8047A8B8(r13)
    li r7, 0x0
    lwz r5, lbl_8047A8B0(r13)
    addi r31, r3, 0x0
    lwz r0, lbl_8047A8B4(r13)
    or r30, r30, r4
    lwz r4, lbl_8047A8A8(r13)
    or r5, r5, r0
    lwz r0, lbl_80478A18(r13)
    andc r30, r30, r5
    lwz r5, lbl_8047A8A4(r13)
    or r6, r4, r30
    stw r7, lbl_8047A8B8(r13)
    andc r4, r5, r30
    stw r6, lbl_8047A8A8(r13)
    cmplwi r0, 0x4
    lwz r0, lbl_8047A8A8(r13)
    stw r4, lbl_8047A8A4(r13)
    and r3, r0, r5
    bne @L_800AADAC
    lwz r0, lbl_8047A8AC(r13)
    or r0, r0, r30
    stw r0, lbl_8047A8AC(r13)
    @L_800AADAC:
    bl fn_800D0464
    lwz r0, lbl_80478A0C(r13)
    cmpwi r0, 0x20
    bne @L_800AAE10
    lwz r5, lbl_8047A8A8(r13)
    cntlzw r0, r5
    stw r0, lbl_80478A0C(r13)
    lwz r4, lbl_80478A0C(r13)
    cmpwi r4, 0x20
    beq @L_800AAE10
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
    @L_800AAE10:
    mr r3, r31
    bl OSRestoreInterrupts
    lwz r0, 0x1c(r1)
    li r3, 0x1
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
