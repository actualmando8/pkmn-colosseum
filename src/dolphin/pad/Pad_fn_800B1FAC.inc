    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stw r31, 0x24(r1)
    stw r30, 0x20(r1)
    stw r29, 0x1c(r1)
    addi r29, r4, 0x0
    stw r28, 0x18(r1)
    addi r28, r3, 0x0
    mulli r5, r28, 0x110
    lis r3, lbl_803FC620@ha
    addi r0, r3, lbl_803FC620@l
    add r30, r0, r5
    lwz r0, 0x0(r30)
    cmpwi r0, 0x0
    bne @L_800B1FF4
    li r3, -0x3
    b @L_800B2050
    @L_800B1FF4:
    lwz r31, 0x84(r30)
    li r4, 0x1ffc
    lha r5, 0x1ffa(r31)
    addi r6, r31, 0x1fc0
    addi r3, r31, 0x0
    addi r0, r5, 0x1
    sth r0, 0x1ffa(r31)
    addi r5, r6, 0x3c
    addi r6, r6, 0x3e
    bl fn_800B2070
    addi r3, r31, 0x0
    li r4, 0x2000
    bl fn_8009B300
    stw r29, 0xd8(r30)
    lis r3, fn_800B1EE4@ha
    addi r5, r3, fn_800B1EE4@l
    lwz r0, 0x80(r30)
    mr r3, r28
    lwz r4, 0xc(r30)
    subf r0, r0, r31
    srwi r0, r0, 13
    mullw r4, r4, r0
    bl fn_800AFFE0
    @L_800B2050:
    lwz r0, 0x2c(r1)
    lwz r31, 0x24(r1)
    lwz r30, 0x20(r1)
    lwz r29, 0x1c(r1)
    lwz r28, 0x18(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
