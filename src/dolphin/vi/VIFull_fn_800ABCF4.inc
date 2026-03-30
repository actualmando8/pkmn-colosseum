    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    mr r31, r3
    bl OSDisableInterrupts
    lwz r5, lbl_8047A8A4(r13)
    slwi r6, r31, 8
    lwz r4, lbl_8047A8B0(r13)
    mr r31, r3
    addi r8, r5, 0x0
    nor r7, r8, r8
    lwz r0, lbl_8047A8B4(r13)
    andc r5, r5, r5
    stw r6, lbl_80478A14(r13)
    and r4, r4, r7
    and r0, r0, r7
    stw r5, lbl_8047A8A4(r13)
    mr r3, r8
    stw r4, lbl_8047A8B0(r13)
    stw r0, lbl_8047A8B4(r13)
    bl fn_800D0464
    mr r3, r31
    bl OSRestoreInterrupts
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
