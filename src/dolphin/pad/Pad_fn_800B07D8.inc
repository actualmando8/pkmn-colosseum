    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stw r31, 0x24(r1)
    stw r30, 0x20(r1)
    li r30, 0x1
    stw r29, 0x1c(r1)
    li r29, 0x0
    bl OSGetTick
    stw r3, lbl_80478A50(r13)
    lis r3, 0x41c6
    addi r31, r3, 0x4e6d
    lwz r0, lbl_80478A50(r13)
    mullw r3, r0, r31
    addi r0, r3, 0x3039
    stw r0, lbl_80478A50(r13)
    lwz r0, lbl_80478A50(r13)
    extrwi r3, r0, 5, 11
    addi r3, r3, 0x1
    b @L_800B0864
    @L_800B0828:
    bl OSGetTick
    slw r0, r3, r30
    addi r30, r30, 0x1
    cmplwi r30, 0x10
    ble @L_800B0840
    li r30, 0x1
    @L_800B0840:
    stw r0, lbl_80478A50(r13)
    addi r29, r29, 0x1
    lwz r0, lbl_80478A50(r13)
    mullw r3, r0, r31
    addi r0, r3, 0x3039
    stw r0, lbl_80478A50(r13)
    lwz r0, lbl_80478A50(r13)
    extrwi r3, r0, 5, 11
    addi r3, r3, 0x1
    @L_800B0864:
    cmpwi r3, 0x4
    bge @L_800B0874
    cmplwi r29, 0xa
    blt @L_800B0828
    @L_800B0874:
    cmpwi r3, 0x4
    bge @L_800B0880
    li r3, 0x4
    @L_800B0880:
    lwz r0, 0x2c(r1)
    lwz r31, 0x24(r1)
    lwz r30, 0x20(r1)
    lwz r29, 0x1c(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
