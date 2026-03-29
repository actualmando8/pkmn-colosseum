    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r27, r4
    mr r31, r5
    mulli r0, r3, 0x4c
    lis r3, lbl_80266918@ha
    addi r3, r3, lbl_80266918@l
    add r3, r3, r0
    lwz r0, 0x4(r3)
    cmpwi r0, 0x0
    blt @L_80013908
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_80013920
    @L_80013908:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_80013920:
    mr r28, r3
    li r30, -0x1
    li r29, 0x0
    b @L_80013968
    @L_80013930:
    mr r3, r28
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80013960
    addi r30, r30, 0x1
    cmpw r30, r27
    blt @L_80013960
    mr r3, r28
    bl fn_80143C68
    mr r30, r3
    b @L_80013978
    @L_80013960:
    addi r29, r29, 0x1
    addi r28, r28, 0x4
    @L_80013968:
    lhz r0, 0x8(r1)
    cmpw r29, r0
    blt @L_80013930
    li r30, 0x0
    @L_80013978:
    lwz r3, lbl_8047A2F8(r13)
    mr r4, r30
    bl fn_80129718
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_800139BC
    li r3, 0x2
    li r4, 0x425f
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r0, 0x0
    li r3, 0x2
    stw r0, 0x0(r31)
    b @L_80013A04
    @L_800139BC:
    lwz r3, lbl_8047A2F8(r13)
    mr r4, r30
    li r5, 0x1
    li r6, -0x1
    bl fn_80129650
    clrlwi r4, r30, 16
    li r3, 0x2d
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x4264
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r0, 0x1
    li r3, 0x0
    stw r0, 0x0(r31)
    @L_80013A04:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
