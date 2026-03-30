    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r28, r4
    mr r27, r3
    mr r3, r28
    bl fn_800D67BC
    clrlwi r31, r28, 16
    li r28, 0x0
    b @L_800D7A4C
    @L_800D796C:
    lwz r0, 0x28(r27)
    cmpwi r0, 0x2
    bne @L_800D7984
    mr r3, r28
    bl fn_800D6028
    b @L_800D798C
    @L_800D7984:
    clrlwi r3, r28, 24
    bl fn_800D5FA4
    @L_800D798C:
    lbz r0, 0x40(r27)
    cmplwi r0, 0x1
    bne @L_800D79B8
    lwz r0, 0x44(r27)
    cmpwi r0, 0x2
    bne @L_800D79B0
    mr r3, r28
    bl fn_800D5DD0
    b @L_800D79B8
    @L_800D79B0:
    clrlwi r3, r28, 24
    bl fn_800D5D6C
    @L_800D79B8:
    li r29, 0x4
    addi r30, r27, 0x70
    @L_800D79C0:
    lbz r0, 0x8(r30)
    cmplwi r0, 0x1
    bne @L_800D79F0
    lwz r0, 0xc(r30)
    subi r3, r29, 0x4
    cmpwi r0, 0x2
    bne @L_800D79E8
    mr r4, r28
    bl fn_800D5AB0
    b @L_800D79F0
    @L_800D79E8:
    clrlwi r4, r28, 24
    bl fn_800D5A38
    @L_800D79F0:
    addi r29, r29, 0x1
    addi r30, r30, 0x1c
    cmpwi r29, 0x5
    ble @L_800D79C0
    li r29, 0x6
    addi r30, r27, 0xa8
    @L_800D7A08:
    lbz r0, 0x8(r30)
    cmplwi r0, 0x1
    bne @L_800D7A38
    lwz r0, 0xc(r30)
    subi r3, r29, 0x6
    cmpwi r0, 0x2
    bne @L_800D7A30
    mr r4, r28
    bl fn_800D579C
    b @L_800D7A38
    @L_800D7A30:
    clrlwi r4, r28, 24
    bl fn_800D5724
    @L_800D7A38:
    addi r29, r29, 0x1
    addi r30, r30, 0x1c
    cmpwi r29, 0xd
    ble @L_800D7A08
    addi r28, r28, 0x1
    @L_800D7A4C:
    clrlwi r0, r28, 16
    cmplw r0, r31
    blt @L_800D796C
    bl fn_800D6728
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
