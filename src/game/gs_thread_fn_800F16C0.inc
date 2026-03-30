    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r26, 0x28(r1)
    mr r31, r3
    lwz r3, 0x14(r3)
    addi r0, r3, 0x2
    stw r0, 0x14(r31)
    lwz r3, 0x14(r31)
    lhz r30, 0x0(r3)
    addi r0, r3, 0x2
    stw r0, 0x14(r31)
    lwz r4, 0x28(r31)
    lwz r5, 0x1c(r31)
    cmpwi r4, 0x40
    ble @L_800F1714
    lis r3, lbl_80271068@ha
    addi r3, r3, lbl_80271068@l
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F1728
    @L_800F1714:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r31)
    add r3, r31, r0
    stw r5, 0x6c(r3)
    @L_800F1728:
    lwz r4, 0x28(r31)
    cmpwi r4, 0x40
    ble @L_800F1748
    lis r3, lbl_80271068@ha
    addi r3, r3, lbl_80271068@l
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F175C
    @L_800F1748:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r31)
    add r3, r31, r0
    stw r30, 0x6c(r3)
    @L_800F175C:
    lwz r0, 0x28(r31)
    addi r4, r30, 0x2
    lis r3, lbl_80401AB8@ha
    li r28, 0x1
    subf r0, r4, r0
    stw r0, 0x1c(r31)
    addi r3, r3, lbl_80401AB8@l
    mr r26, r3
    lwz r0, 0x1c(r31)
    addi r30, r3, 0xff
    slwi r3, r0, 2
    addi r0, r3, 0x6c
    lwzx r0, r31, r0
    stw r0, 0x1c(r1)
    mr r27, r0
    b @L_800F18F8
    @L_800F179C:
    extsb r0, r3
    cmpwi r0, 0x25
    bne @L_800F18A0
    lwz r0, 0x1c(r31)
    lis r3, lbl_80401A78@ha
    addi r4, r3, lbl_80401A78@l
    add r0, r0, r28
    li r3, 0x0
    slwi r5, r0, 2
    addi r28, r28, 0x1
    addi r0, r5, 0x6c
    lwzx r0, r31, r0
    stw r0, 0x8(r1)
    @L_800F17D0:
    lbz r0, 0x0(r27)
    addi r3, r3, 0x1
    stb r0, 0x0(r4)
    addi r4, r4, 0x1
    lbz r5, 0x0(r27)
    extsb r0, r5
    cmpwi r0, 0x64
    beq @L_800F1800
    cmpwi r0, 0x78
    beq @L_800F1800
    cmpwi r0, 0x63
    bne @L_800F1828
    @L_800F1800:
    lis r4, lbl_80401A78@ha
    li r0, 0x0
    addi r4, r4, lbl_80401A78@l
    lwz r5, 0x8(r1)
    stbx r0, r4, r3
    mr r3, r26
    crxor 6, 6, 6
    bl fn_800C8520
    mr r29, r3
    b @L_800F1898
    @L_800F1828:
    cmpwi r0, 0x66
    bne @L_800F1858
    lis r4, lbl_80401A78@ha
    li r0, 0x0
    addi r4, r4, lbl_80401A78@l
    lfs f1, 0x8(r1)
    stbx r0, r4, r3
    mr r3, r26
    creqv 6, 6, 6
    bl fn_800C8520
    mr r29, r3
    b @L_800F1898
    @L_800F1858:
    cmpwi r0, 0x73
    bne @L_800F1888
    lis r4, lbl_80401A78@ha
    li r0, 0x0
    addi r4, r4, lbl_80401A78@l
    lwz r5, 0x8(r1)
    stbx r0, r4, r3
    mr r3, r26
    crxor 6, 6, 6
    bl fn_800C8520
    mr r29, r3
    b @L_800F1898
    @L_800F1888:
    extsb. r0, r5
    beq @L_800F1898
    addi r27, r27, 0x1
    b @L_800F17D0
    @L_800F1898:
    add r26, r26, r29
    b @L_800F18EC
    @L_800F18A0:
    cmpwi r0, 0x5c
    bne @L_800F18E4
    lbz r0, 0x1(r27)
    cmpwi r0, 0x6e
    beq @L_800F18B8
    b @L_800F18D8
    @L_800F18B8:
    mr r3, r26
    la r4, lbl_8047CCB8(r2)
    crxor 6, 6, 6
    bl fn_800C8520
    mr r29, r3
    addi r27, r27, 0x1
    add r26, r26, r29
    b @L_800F18EC
    @L_800F18D8:
    stb r3, 0x0(r26)
    addi r26, r26, 0x1
    b @L_800F18EC
    @L_800F18E4:
    stb r3, 0x0(r26)
    addi r26, r26, 0x1
    @L_800F18EC:
    cmplw r26, r30
    addi r27, r27, 0x1
    bge @L_800F1904
    @L_800F18F8:
    lbz r3, 0x0(r27)
    extsb. r0, r3
    bne @L_800F179C
    @L_800F1904:
    li r0, 0x0
    lis r3, lbl_80401AB8@ha
    stb r0, 0x0(r26)
    addi r3, r3, lbl_80401AB8@l
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r3, 0x28(r31)
    cmpwi r3, 0x0
    bgt @L_800F1944
    lis r3, lbl_8027107C@ha
    addi r3, r3, lbl_8027107C@l
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r31)
    stw r0, 0x10(r1)
    b @L_800F195C
    @L_800F1944:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r31)
    add r3, r31, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x10(r1)
    @L_800F195C:
    lwz r3, 0x28(r31)
    lwz r0, 0x10(r1)
    cmpwi r3, 0x0
    stw r0, 0x14(r1)
    bgt @L_800F198C
    lis r3, lbl_8027107C@ha
    addi r3, r3, lbl_8027107C@l
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r31)
    stw r0, 0xc(r1)
    b @L_800F19A4
    @L_800F198C:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r31)
    add r3, r31, r0
    lwz r0, 0x6c(r3)
    stw r0, 0xc(r1)
    @L_800F19A4:
    lwz r0, 0xc(r1)
    lis r3, lbl_8027107C@ha
    lwz r26, 0x14(r1)
    addi r27, r3, lbl_8027107C@l
    stw r0, 0x18(r1)
    li r28, 0x0
    stw r0, 0x1c(r31)
    b @L_800F19EC
    @L_800F19C4:
    lwz r3, 0x28(r31)
    cmpwi r3, 0x0
    bgt @L_800F19E0
    mr r3, r27
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F19E8
    @L_800F19E0:
    subi r0, r3, 0x1
    stw r0, 0x28(r31)
    @L_800F19E8:
    addi r28, r28, 0x1
    @L_800F19EC:
    cmpw r28, r26
    blt @L_800F19C4
    lmw r26, 0x28(r1)
    li r3, 0x1
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
