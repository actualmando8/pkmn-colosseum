    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    addi r30, r4, 0x0
    stw r29, 0x14(r1)
    addi r29, r3, 0x0
    bl OSDisableInterrupts
    addi r0, r13, -24036
    stw r0, -24040(r13)
    lis r4, 0x2
    addi r31, r3, 0x0
    lwz r0, -24040(r13)
    addi r3, r4, -0x8000
    stw r0, 0x0(r29)
    stw r30, -24056(r13)
    bl __OSMaskInterrupts
    lis r3, 0xcc00
    li r0, 0x0
    stw r0, 0x6828(r3)
    mr r3, r31
    bl OSRestoreInterrupts
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
