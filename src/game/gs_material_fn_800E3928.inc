    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    li r31, 0x0
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    li r28, 0x0
    b @L_800E39CC
    @L_800E3950:
    lwz r0, lbl_8047AB74(r13)
    add r29, r0, r31
    lwz r3, 0x0(r29)
    clrlwi. r0, r3, 31
    beq @L_800E39C4
    rlwinm. r0, r3, 0, 30, 30
    beq @L_800E39BC
    rlwinm. r0, r3, 0, 11, 11
    beq @L_800E39B0
    addi r3, r29, 0x4c
    bl fn_80191118
    neg r0, r3
    or r0, r0, r3
    srwi r0, r0, 31
    cmplwi r0, 0x1
    bne @L_800E39A0
    lwz r0, 0x0(r29)
    oris r0, r0, 0x40
    stw r0, 0x0(r29)
    b @L_800E39C4
    @L_800E39A0:
    lwz r0, 0x0(r29)
    rlwinm r0, r0, 0, 10, 8
    stw r0, 0x0(r29)
    b @L_800E39C4
    @L_800E39B0:
    oris r0, r3, 0x40
    stw r0, 0x0(r29)
    b @L_800E39C4
    @L_800E39BC:
    rlwinm r0, r3, 0, 10, 8
    stw r0, 0x0(r29)
    @L_800E39C4:
    addi r31, r31, 0x170
    addi r28, r28, 0x1
    @L_800E39CC:
    lwz r0, lbl_8047AB78(r13)
    cmplw r28, r0
    blt @L_800E3950
    li r29, 0x0
    lis r3, 0x40
    mr r30, r29
    addi r31, r3, 0x400
    b @L_800E3A14
    @L_800E39EC:
    lwz r0, lbl_8047AB74(r13)
    add r3, r0, r30
    lwz r4, 0x0(r3)
    clrlwi. r0, r4, 31
    beq @L_800E3A0C
    and. r0, r4, r31
    beq @L_800E3A0C
    bl fn_800ED1CC
    @L_800E3A0C:
    addi r30, r30, 0x170
    addi r29, r29, 0x1
    @L_800E3A14:
    lwz r0, lbl_8047AB78(r13)
    cmplw r29, r0
    blt @L_800E39EC
    li r29, 0x0
    mr r30, r29
    b @L_800E3ADC
    @L_800E3A2C:
    lwz r0, lbl_8047AB74(r13)
    add r28, r0, r30
    lwz r0, 0x0(r28)
    clrlwi. r0, r0, 31
    beq @L_800E3AD4
    mr r3, r28
    bl fn_800ED4D4
    lwz r0, 0x0(r28)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_800E3A68
    addi r3, r28, 0x3c
    addi r4, r28, 0x18
    li r5, 0xc
    bl memcpy
    b @L_800E3AB8
    @L_800E3A68:
    mr r3, r28
    li r4, 0x1
    bl fn_800EE150
    mr. r31, r3
    bne @L_800E3A90
    addi r3, r28, 0x3c
    addi r4, r28, 0x18
    li r5, 0xc
    bl memcpy
    b @L_800E3AB8
    @L_800E3A90:
    addi r4, r28, 0x3c
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    mr r3, r31
    bl fn_800EE828
    lfs f1, 0x40(r28)
    lfs f0, 0x48(r28)
    fsubs f0, f1, f0
    stfs f0, 0x40(r28)
    @L_800E3AB8:
    lwz r3, 0x0(r28)
    rlwinm. r0, r3, 0, 9, 9
    beq @L_800E3AD4
    rlwinm. r0, r3, 0, 10, 10
    beq @L_800E3AD4
    addi r3, r28, 0x4c
    bl fn_80190E60
    @L_800E3AD4:
    addi r30, r30, 0x170
    addi r29, r29, 0x1
    @L_800E3ADC:
    lwz r0, lbl_8047AB78(r13)
    cmplw r29, r0
    blt @L_800E3A2C
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
