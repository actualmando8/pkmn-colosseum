    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r21, 0x14(r1)
    mr r31, r3
    mr r22, r5
    cmplwi r22, 0x0
    li r23, 0x0
    bne @L_800F9710
    li r3, 0x0
    b @L_800F9AD8
@L_800F9710:
    cmplwi r31, 0x0
    bne @L_800F9720
    li r3, 0x0
    b @L_800F9AD8
@L_800F9720:
    cmpwi r4, 0x0
    bgt @L_800F9730
    li r3, 0x0
    b @L_800F9AD8
@L_800F9730:
    cmplwi r22, 0x0
    subi r0, r4, 0x1
    mr r28, r31
    slwi r24, r0, 1
    bne @L_800F974C
    li r25, 0x0
    b @L_800F97DC
@L_800F974C:
    lwz r3, lbl_80478B08(r13)
    srwi r4, r22, 20
    clrlwi r5, r22, 12
    lwz r9, 0x8(r3)
    b @L_800F97D0
@L_800F9760:
    lhz r0, 0x0(r9)
    cmplw r0, r4
    bne @L_800F97CC
    lhz r6, 0x4(r9)
    addi r8, r9, 0x10
    li r7, 0x0
    b @L_800F97C4
@L_800F977C:
    add r0, r7, r6
    srwi r3, r0, 1
    slwi r10, r3, 3
    lwzx r0, r8, r10
    cmplw r0, r5
    bne @L_800F97B4
    addi r0, r1, 0x8
    cmplwi r0, 0x0
    beq @L_800F97A4
    stw r9, 0x8(r1)
@L_800F97A4:
    add r3, r8, r10
    lwz r0, 0x4(r3)
    add r25, r9, r0
    b @L_800F97DC
@L_800F97B4:
    bge @L_800F97C0
    addi r7, r3, 0x1
    b @L_800F97C4
@L_800F97C0:
    mr r6, r3
@L_800F97C4:
    cmplw r7, r6
    blt @L_800F977C
@L_800F97CC:
    lwz r9, 0x8(r9)
@L_800F97D0:
    cmplwi r9, 0x0
    bne @L_800F9760
    li r25, 0x0
@L_800F97DC:
    cmplwi r25, 0x0
    bne @L_800F97EC
    li r3, 0x0
    b @L_800F9AD8
@L_800F97EC:
    lis r3, lbl_80401DE0@ha
    li r4, 0x0
    addi r3, r3, lbl_80401DE0@l
    li r5, 0x68
    mr r29, r3
    bl memset
    lis r3, lbl_80401DE0@ha
    lfs f0, lbl_8047CD08(r2)
    addi r4, r3, lbl_80401DE0@l
    li r5, 0x1
    li r0, -0x1
    stb r5, 0x0(r4)
    lwz r3, 0x8(r1)
    stfs f0, 0x60(r4)
    stfs f0, 0x64(r4)
    stw r0, 0x24(r4)
    stw r25, 0x28(r4)
    stw r25, 0x2c(r4)
    stw r25, 0x30(r4)
    lbz r0, 0x3(r3)
    sth r0, 0x20(r4)
    stw r22, 0x1c(r4)
    stb r5, 0x1(r4)
@L_800F9848:
    mr r27, r28
    mr r22, r23
@L_800F9850:
    lwz r3, 0x30(r29)
    lhz r4, 0x0(r3)
    cmplwi r4, 0x0
    bne @L_800F9894
    lbz r3, 0x40(r29)
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_800F9874
    b @L_800F989C
@L_800F9874:
    subi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0x40(r29)
    slwi r3, r0, 2
    addi r0, r3, 0x34
    lwzx r0, r29, r0
    stw r0, 0x30(r29)
    b @L_800F9850
@L_800F9894:
    addi r0, r3, 0x2
    stw r0, 0x30(r29)
@L_800F989C:
    clrlwi r0, r4, 16
    cmplwi r0, 0x0
    beq @L_800F9ACC
    addi r23, r23, 0x2
    cmplw r24, r23
    blt @L_800F9ACC
    cmplwi r0, 0xffff
    sth r4, 0x0(r28)
    addi r28, r28, 0x2
    bne @L_800F9848
    lwz r3, 0x30(r29)
    addi r23, r23, 0x1
    cmplw r24, r23
    addi r0, r3, 0x1
    stw r0, 0x30(r29)
    lbz r4, 0x0(r3)
    blt @L_800F98EC
    stb r4, 0x0(r28)
    addi r28, r28, 0x1
    b @L_800F98F4
@L_800F98EC:
    mr r28, r27
    b @L_800F9ACC
@L_800F98F4:
    lwz r3, lbl_80478B08(r13)
    lwz r26, 0x30(r29)
    lwz r3, 0x28(r3)
    lbz r25, 0x40(r29)
    cmplwi r3, 0x0
    beq @L_800F9A6C
    lbz r0, 0x1(r29)
    cmplwi r0, 0x0
    bne @L_800F9928
    slwi r0, r4, 3
    lbzx r0, r3, r0
    extrwi r0, r0, 1, 27
    b @L_800F9934
@L_800F9928:
    slwi r0, r4, 3
    lbzx r0, r3, r0
    extrwi r0, r0, 1, 28
@L_800F9934:
    cmplwi r0, 0x0
    beq @L_800F9A6C
    slwi r0, r4, 3
    add r21, r3, r0
    lwz r12, 0x4(r21)
    cmplwi r12, 0x0
    beq @L_800F9A6C
    mr r3, r29
    mtctr r12
    bctrl
    lbz r0, 0x0(r21)
    extrwi r0, r0, 2, 24
    cmplwi r0, 0x0
    beq @L_800F9A6C
    cmplwi r3, 0x0
    beq @L_800F9A6C
    cmpwi r0, 0x2
    beq @L_800F9994
    bge @L_800F9A24
    cmpwi r0, 0x1
    bge @L_800F998C
    b @L_800F9A24
@L_800F998C:
    mr r30, r3
    b @L_800F9A24
@L_800F9994:
    cmplwi r3, 0x0
    bne @L_800F99A4
    li r30, 0x0
    b @L_800F9A24
@L_800F99A4:
    lwz r4, lbl_80478B08(r13)
    srwi r5, r3, 20
    clrlwi r3, r3, 12
    lwz r9, 0x8(r4)
    b @L_800F9A18
@L_800F99B8:
    lhz r0, 0x0(r9)
    cmplw r0, r5
    bne @L_800F9A14
    lhz r6, 0x4(r9)
    addi r8, r9, 0x10
    li r7, 0x0
    b @L_800F9A0C
@L_800F99D4:
    add r0, r7, r6
    srwi r4, r0, 1
    slwi r0, r4, 3
    add r10, r8, r0
    lwz r0, 0x0(r10)
    cmplw r0, r3
    bne @L_800F99FC
    lwz r0, 0x4(r10)
    add r30, r9, r0
    b @L_800F9A24
@L_800F99FC:
    bge @L_800F9A08
    addi r7, r4, 0x1
    b @L_800F9A0C
@L_800F9A08:
    mr r6, r4
@L_800F9A0C:
    cmplw r7, r6
    blt @L_800F99D4
@L_800F9A14:
    lwz r9, 0x8(r9)
@L_800F9A18:
    cmplwi r9, 0x0
    bne @L_800F99B8
    li r30, 0x0
@L_800F9A24:
    lbz r4, 0x40(r29)
    extsb r3, r4
    cmpwi r3, 0x3
    blt @L_800F9A50
    lis r3, lbl_80271700@ha
    lis r4, lbl_80315678@ha
    addi r3, r3, lbl_80271700@l
    addi r4, r4, lbl_80315678@l
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_800F9A6C
@L_800F9A50:
    addi r0, r4, 0x1
    slwi r3, r3, 2
    stb r0, 0x40(r29)
    addi r0, r3, 0x34
    lwz r3, 0x30(r29)
    stwx r3, r29, r0
    stw r30, 0x30(r29)
@L_800F9A6C:
    lbz r0, 0x40(r29)
    extsb r3, r25
    extsb r0, r0
    cmpw r3, r0
    bne @L_800F9AC0
    lwz r0, 0x30(r29)
    subf r0, r26, r0
    add r23, r23, r0
    cmplw r24, r23
    blt @L_800F9AB8
    b @L_800F9AA8
@L_800F9A98:
    lbz r0, 0x0(r26)
    addi r26, r26, 0x1
    stb r0, 0x0(r28)
    addi r28, r28, 0x1
@L_800F9AA8:
    lwz r0, 0x30(r29)
    cmplw r0, r26
    bgt @L_800F9A98
    b @L_800F9848
@L_800F9AB8:
    mr r28, r27
    b @L_800F9ACC
@L_800F9AC0:
    mr r28, r27
    mr r23, r22
    b @L_800F9848
@L_800F9ACC:
    li r0, 0x0
    mr r3, r31
    sth r0, 0x0(r28)
@L_800F9AD8:
    lmw r21, 0x14(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
