    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    clrlwi. r0, r4, 28
    stwu r1, -0x30(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    bne @L_800AA868
    lwz r30, lbl_80478A0C(r13)
    lis r31, 0x8000
    lwz r3, lbl_8047A8A4(r13)
    addi r4, r1, 0x1c
    srw r0, r31, r30
    or r0, r3, r0
    stw r0, lbl_8047A8A4(r13)
    mr r3, r30
    bl fn_800D05A4
    lwz r0, lbl_80478A14(r13)
    addi r3, r30, 0x0
    oris r4, r0, 0x40
    bl fn_800D0338
    lwz r3, lbl_8047A8A4(r13)
    bl fn_800D03C8
    lwz r0, lbl_80478A0C(r13)
    lwz r3, lbl_8047A8B0(r13)
    srw r0, r31, r0
    or r0, r3, r0
    stw r0, lbl_8047A8B0(r13)
    @L_800AA868:
    lwz r5, lbl_8047A8A8(r13)
    cntlzw r0, r5
    stw r0, lbl_80478A0C(r13)
    lwz r4, lbl_80478A0C(r13)
    cmpwi r4, 0x20
    beq @L_800AA8BC
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
    @L_800AA8BC:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    addi r1, r1, 0x30
    mtlr r0
    blr
