    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, lbl_80466BC0@ha
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    addi r3, r4, lbl_80466BC0@l
    lwz r31, 0x1e8(r3)
    bl OSDisableInterrupts
    lis r4, lbl_80466BC0@ha
    addi r4, r4, lbl_80466BC0@l
    stw r30, 0x1e8(r4)
    bl OSRestoreInterrupts
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
