    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r25, r3
    mr r24, r4
    li r3, 0x7f
    bl fn_801B25C4
    bl fn_800D2584
    mr. r31, r3
    beq @L_800E3744
    lwz r3, 0xc(r31)
    bl fn_80195A6C
    cmpwi r3, 0x0
    beq @L_800E3744
    lwz r3, 0xc(r31)
    bl fn_800DD174
    clrlwi r29, r24, 24
    rlwinm r28, r25, 0, 27, 27
    rlwinm r27, r25, 0, 19, 19
    rlwinm r26, r25, 0, 18, 18
    li r25, 0x0
    li r30, 0x0
    b @L_800E3734
    @L_800E3664:
    lwz r0, lbl_8047AB74(r13)
    add r24, r0, r30
    lwz r4, 0x0(r24)
    clrlwi. r0, r4, 31
    beq @L_800E372C
    rlwinm. r0, r4, 0, 30, 30
    beq @L_800E372C
    lis r3, 0x40
    addi r0, r3, 0x400
    and. r0, r4, r0
    beq @L_800E372C
    extrwi r0, r4, 1, 22
    cmpw r0, r29
    bne @L_800E372C
    mr r3, r24
    li r4, 0x1
    bl fn_800E9148
    lwz r0, 0x0(r24)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_800E36BC
    lwz r31, 0xc(r24)
    b @L_800E36C0
    @L_800E36BC:
    lwz r31, 0x8(r24)
    @L_800E36C0:
    cmpwi r28, 0x0
    beq @L_800E36E8
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_801A13CC
    lwz r3, 0x168(r24)
    lwz r4, 0x16c(r24)
    bl fn_800D6A5C
    @L_800E36E8:
    cmpwi r27, 0x0
    beq @L_800E3704
    mr r3, r31
    li r4, 0x0
    li r5, 0x4
    li r6, 0x0
    bl fn_801A13CC
    @L_800E3704:
    cmpwi r26, 0x0
    beq @L_800E3720
    mr r3, r31
    li r4, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801A13CC
    @L_800E3720:
    mr r3, r24
    li r4, 0x0
    bl fn_800E9148
    @L_800E372C:
    addi r30, r30, 0x170
    addi r25, r25, 0x1
    @L_800E3734:
    lwz r0, lbl_8047AB78(r13)
    cmplw r25, r0
    blt @L_800E3664
    bl fn_80195A48
    @L_800E3744:
    li r3, -0x1
    bl fn_800D87AC
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
