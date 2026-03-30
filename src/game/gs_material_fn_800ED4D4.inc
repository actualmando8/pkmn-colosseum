    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stw r31, 0x4c(r1)
    stw r30, 0x48(r1)
    mr r30, r3
    lwz r0, 0x114(r3)
    cmpwi r0, 0x0
    beq @L_800ED688
    lwz r0, 0x0(r30)
    rlwinm. r0, r0, 0, 12, 12
    beq @L_800ED688
    lwz r3, 0x118(r30)
    lwz r4, 0x11c(r30)
    bl fn_800EE150
    addi r4, r1, 0x3c
    mr r31, r3
    addi r5, r1, 0x30
    addi r6, r1, 0x24
    bl fn_800EE3BC
    mr r3, r31
    bl fn_800EE828
    addi r3, r1, 0x14
    addi r4, r1, 0x30
    bl fn_800E06EC
    addi r3, r1, 0x8
    addi r4, r1, 0x14
    addi r5, r30, 0x120
    bl fn_800DFEEC
    addi r3, r1, 0x3c
    addi r5, r1, 0x8
    mr r4, r3
    bl fn_800E019C
    addi r3, r1, 0x30
    addi r5, r30, 0x12c
    mr r4, r3
    bl fn_800E019C
    addi r3, r1, 0x24
    addi r5, r30, 0x138
    mr r4, r3
    bl fn_800E0108
    mr r3, r30
    addi r4, r30, 0x120
    bl fn_800E50A8
    mr r3, r30
    addi r4, r30, 0x12c
    bl fn_800E4E8C
    mr r3, r30
    addi r4, r30, 0x138
    bl fn_800E4DB0
    lwz r0, 0x114(r30)
    cmplwi r0, 0x7
    bgt @L_800ED668
    lis r3, jumptable_803155B0@ha
    slwi r0, r0, 2
    addi r3, r3, jumptable_803155B0@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r3, r30
    addi r4, r1, 0x3c
    bl fn_800E50A8
    b @L_800ED668
    mr r3, r30
    addi r4, r1, 0x30
    bl fn_800E4E8C
    b @L_800ED668
    mr r3, r30
    addi r4, r1, 0x24
    bl fn_800E4DB0
    b @L_800ED668
    mr r3, r30
    addi r4, r1, 0x3c
    bl fn_800E50A8
    mr r3, r30
    addi r4, r1, 0x30
    bl fn_800E4E8C
    b @L_800ED668
    mr r3, r30
    addi r4, r1, 0x30
    bl fn_800E4E8C
    mr r3, r30
    addi r4, r1, 0x24
    bl fn_800E4DB0
    b @L_800ED668
    mr r3, r30
    addi r4, r1, 0x3c
    bl fn_800E50A8
    mr r3, r30
    addi r4, r1, 0x24
    bl fn_800E4DB0
    b @L_800ED668
    mr r3, r30
    addi r4, r1, 0x3c
    bl fn_800E50A8
    mr r3, r30
    addi r4, r1, 0x30
    bl fn_800E4E8C
    mr r3, r30
    addi r4, r1, 0x24
    bl fn_800E4DB0
    @L_800ED668:
    lwz r3, 0x0(r30)
    rlwinm. r0, r3, 0, 13, 13
    beq @L_800ED680
    rlwinm r0, r3, 0, 13, 11
    stw r0, 0x0(r30)
    b @L_800ED688
    @L_800ED680:
    oris r0, r3, 0x8
    stw r0, 0x0(r30)
    @L_800ED688:
    lwz r0, 0x0(r30)
    oris r0, r0, 0x1
    stw r0, 0x0(r30)
    lwz r31, 0x4c(r1)
    lwz r30, 0x48(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
