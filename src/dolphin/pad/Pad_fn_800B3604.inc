    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    addi r29, r4, 0x0
    cmpwi r29, -0x3
    stw r28, 0x10(r1)
    addi r28, r3, 0x0
    mulli r5, r28, 0x110
    lis r3, lbl_803FC620@ha
    addi r0, r3, lbl_803FC620@l
    add r31, r0, r5
    beq @L_800B36E4
    bge @L_800B3650
    cmpwi r29, -0x5
    beq @L_800B36E4
    b @L_800B36F0
    @L_800B3650:
    cmpwi r29, 0x1
    beq @L_800B36A0
    bge @L_800B36F0
    cmpwi r29, 0x0
    bge @L_800B3668
    b @L_800B36F0
    @L_800B3668:
    lwz r3, 0x24(r31)
    addi r0, r3, 0x1
    cmpwi r0, 0x7
    stw r0, 0x24(r31)
    bge @L_800B3690
    mr r3, r28
    bl fn_800B31F4
    mr. r29, r3
    blt @L_800B36F0
    b @L_800B371C
    @L_800B3690:
    mr r3, r31
    bl fn_800B2968
    mr r29, r3
    b @L_800B36F0
    @L_800B36A0:
    lis r3, fn_800B3604@ha
    addi r0, r3, fn_800B3604@l
    lis r3, fn_800AF51C@ha
    stw r0, 0xdc(r31)
    addi r5, r3, fn_800AF51C@l
    addi r3, r28, 0x0
    li r4, 0x0
    bl EXILock
    cmpwi r3, 0x0
    beq @L_800B371C
    li r0, 0x0
    stw r0, 0xdc(r31)
    mr r3, r28
    bl fn_800B31F4
    mr. r29, r3
    blt @L_800B36F0
    b @L_800B371C
    @L_800B36E4:
    addi r3, r28, 0x0
    addi r4, r29, 0x0
    bl fn_800B38DC
    @L_800B36F0:
    lwz r30, 0xd0(r31)
    li r0, 0x0
    addi r3, r31, 0x0
    stw r0, 0xd0(r31)
    mr r4, r29
    bl fn_800B02F4
    addi r12, r30, 0x0
    mtlr r12
    addi r3, r28, 0x0
    addi r4, r29, 0x0
    blrl
    @L_800B371C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
