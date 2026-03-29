    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    mr r26, r4
    mr r30, r5
    mulli r0, r3, 0x4c
    lis r3, lbl_80266918@ha
    addi r3, r3, lbl_80266918@l
    add r3, r3, r0
    lwz r0, 0x4(r3)
    cmpwi r0, 0x0
    blt @L_80013FD4
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_80013FEC
    @L_80013FD4:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_80013FEC:
    mr r27, r3
    li r29, -0x1
    li r28, 0x0
    b @L_80014030
    @L_80013FFC:
    mr r3, r27
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80014028
    addi r29, r29, 0x1
    cmpw r29, r26
    blt @L_80014028
    mr r3, r27
    bl fn_80143C68
    b @L_80014040
    @L_80014028:
    addi r28, r28, 0x1
    addi r27, r27, 0x4
    @L_80014030:
    lhz r0, 0x8(r1)
    cmpw r28, r0
    blt @L_80013FFC
    li r3, 0x0
    @L_80014040:
    clrlwi r27, r3, 16
    mr r3, r27
    bl fn_801440A0
    lwz r0, lbl_8047A2E0(r13)
    cmpwi r0, 0x0
    bne @L_80014060
    bl fn_80143DE4
    b @L_80014064
    @L_80014060:
    bl fn_80143DCC
    @L_80014064:
    cmplwi r3, 0x0
    bne @L_80014098
    li r3, 0x2
    li r4, 0x4261
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r0, 0x0
    li r3, 0x3
    stw r0, 0x0(r30)
    b @L_800140E8
    @L_80014098:
    mr r12, r3
    mr r3, r27
    mr r4, r30
    mtctr r12
    bctrl
    cmpwi r3, 0x1
    beq @L_800140D8
    bge @L_800140C4
    cmpwi r3, 0x0
    bge @L_800140D0
    b @L_800140E4
    @L_800140C4:
    cmpwi r3, 0x3
    bge @L_800140E4
    b @L_800140E0
    @L_800140D0:
    li r31, 0x0
    b @L_800140E4
    @L_800140D8:
    li r31, 0x3
    b @L_800140E4
    @L_800140E0:
    li r31, 0x4
    @L_800140E4:
    mr r3, r31
    @L_800140E8:
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
