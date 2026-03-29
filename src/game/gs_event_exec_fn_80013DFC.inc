    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r30, r4
    mr r31, r5
    mulli r0, r3, 0x4c
    lis r3, lbl_80266918@ha
    addi r3, r3, lbl_80266918@l
    add r3, r3, r0
    lwz r0, 0x4(r3)
    cmpwi r0, 0x0
    blt @L_80013E50
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_80013E68
    @L_80013E50:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_80013E68:
    mr r27, r3
    li r29, -0x1
    li r28, 0x0
    b @L_80013EB0
    @L_80013E78:
    mr r3, r27
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80013EA8
    addi r29, r29, 0x1
    cmpw r29, r30
    blt @L_80013EA8
    mr r3, r27
    bl fn_80143C68
    mr r29, r3
    b @L_80013EC0
    @L_80013EA8:
    addi r28, r28, 0x1
    addi r27, r27, 0x4
    @L_80013EB0:
    lhz r0, 0x8(r1)
    cmpw r28, r0
    blt @L_80013E78
    li r29, 0x0
    @L_80013EC0:
    mr r3, r29
    bl fn_801440A0
    bl fn_80143E88
    clrlwi r0, r3, 24
    cmplwi r0, 0xff
    beq @L_80013F10
    clrlwi r4, r29, 16
    li r3, 0x2d
    bl fn_80132A38
    li r3, 0x2
    li r4, 0x4262
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r0, 0x0
    li r3, 0x0
    stw r0, 0x0(r31)
    b @L_80013F6C
    @L_80013F10:
    clrlwi r3, r29, 16
    bl fn_801440A0
    bl fn_80144014
    clrlwi r27, r3, 24
    li r3, 0x59
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    mr r3, r27
    mr r5, r29
    clrlwi r4, r30, 24
    li r6, 0x0
    bl fn_8001BCEC
    stw r3, lbl_8047A2EC(r13)
    bl fn_8001B184
    lwz r0, lbl_8047A2EC(r13)
    cmpwi r0, 0x0
    bge @L_80013F60
    li r3, 0x3
    b @L_80013F6C
    @L_80013F60:
    li r0, 0x0
    li r3, 0x0
    stw r0, 0x0(r31)
    @L_80013F6C:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
