    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    addi r4, r1, 0xc
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    addi r29, r3, 0x0
    bl fn_800B023C
    cmpwi r3, 0x0
    bge @L_800B39A8
    b @L_800B3A08
    @L_800B39A8:
    mulli r4, r29, 0x110
    lis r3, lbl_803FC620@ha
    addi r0, r3, lbl_803FC620@l
    add r30, r0, r4
    bl OSDisableInterrupts
    lwz r0, 0x0(r30)
    addi r31, r3, 0x0
    cmpwi r0, 0x0
    beq @L_800B39FC
    addi r3, r29, 0x0
    li r4, 0x0
    bl fn_8009870C
    mr r3, r29
    bl fn_80098AE8
    addi r3, r30, 0xe0
    bl fn_8009A590
    li r3, 0x0
    stw r3, 0x0(r30)
    li r0, -0x3
    stw r0, 0x4(r30)
    stw r3, 0x24(r30)
    @L_800B39FC:
    mr r3, r31
    bl OSRestoreInterrupts
    li r3, 0x0
    @L_800B3A08:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
