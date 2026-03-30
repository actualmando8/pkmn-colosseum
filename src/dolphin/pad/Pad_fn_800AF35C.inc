    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stw r31, 0x24(r1)
    addi r31, r3, 0x0
    mulli r4, r31, 0x110
    stw r30, 0x20(r1)
    lis r3, lbl_803FC620@ha
    stw r29, 0x1c(r1)
    addi r0, r3, lbl_803FC620@l
    add r30, r0, r4
    addi r3, r30, 0xe0
    bl fn_8009A590
    lwz r0, 0x0(r30)
    cmpwi r0, 0x0
    beq @L_800AF458
    addi r3, r31, 0x0
    li r4, 0x0
    li r5, 0x0
    bl EXILock
    cmpwi r3, 0x0
    bne @L_800AF3BC
    li r29, -0x80
    b @L_800AF430
    @L_800AF3BC:
    addi r3, r31, 0x0
    addi r4, r1, 0x10
    bl fn_800AF660
    mr. r29, r3
    blt @L_800AF428
    mr r3, r31
    bl fn_800AF750
    mr. r29, r3
    blt @L_800AF428
    lbz r0, 0x10(r1)
    rlwinm. r0, r0, 0, 27, 28
    beq @L_800AF3F4
    li r0, -0x5
    b @L_800AF3F8
    @L_800AF3F4:
    li r0, 0x0
    @L_800AF3F8:
    mr r29, r0
    cmpwi r29, -0x5
    bne @L_800AF428
    lwz r3, 0xa8(r30)
    subic. r0, r3, 0x1
    stw r0, 0xa8(r30)
    ble @L_800AF428
    mr r3, r31
    bl fn_800AF8A0
    mr. r29, r3
    blt @L_800AF430
    b @L_800AF458
    @L_800AF428:
    mr r3, r31
    bl EXIUnlock
    @L_800AF430:
    lwz r0, 0xcc(r30)
    cmplwi r0, 0x0
    mr r12, r0
    beq @L_800AF458
    li r0, 0x0
    mtlr r12
    stw r0, 0xcc(r30)
    addi r3, r31, 0x0
    addi r4, r29, 0x0
    blrl
    @L_800AF458:
    lwz r0, 0x2c(r1)
    lwz r31, 0x24(r1)
    lwz r30, 0x20(r1)
    lwz r29, 0x1c(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
