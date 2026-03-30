    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    lbz r0, lbl_8047A5E0(r13)
    stw r3, lbl_8047A5A4(r13)
    cmplwi r0, 0x0
    bne @L_8006B698
    lis r3, lbl_80267DD8@ha
    mr r30, r27
    addi r29, r3, lbl_80267DD8@l
    li r28, 0x0
    la r31, lbl_8047C038(r2)
    @L_8006B614:
    lhz r4, 0x0(r31)
    addi r3, r30, 0x24
    bl fn_8006AABC
    lwz r5, 0x0(r29)
    clrlwi r0, r28, 16
    addi r3, r30, 0x59a8
    addi r4, r30, 0x24
    stw r5, 0x48(r30)
    li r5, 0x1660
    sth r0, 0x26(r30)
    bl memcpy
    addi r31, r31, 0x2
    addi r30, r30, 0x1660
    addi r29, r29, 0x4
    addi r28, r28, 0x1
    cmplwi r28, 0x4
    blt @L_8006B614
    lwz r0, 0x0(r27)
    cmpwi r0, 0x3
    beq @L_8006B674
    bge @L_8006B688
    cmpwi r0, 0x0
    beq @L_8006B688
    b @L_8006B688
    @L_8006B674:
    li r3, 0xaf
    bl fn_80071644
    li r0, 0x0
    stw r0, 0x10(r27)
    b @L_8006B698
    @L_8006B688:
    li r3, 0xa8
    bl fn_80071644
    li r0, 0x4
    stw r0, 0x10(r27)
    @L_8006B698:
    li r0, 0x0
    stb r0, lbl_8047A5E0(r13)
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
