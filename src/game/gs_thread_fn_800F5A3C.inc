    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r25, 0x34(r1)
    mr r25, r3
    lis r3, lbl_80271068@ha
    addi r29, r3, lbl_80271068@l
    lwz r4, 0x14(r25)
    addi r0, r4, 0x1
    stw r0, 0x14(r25)
    lwz r3, 0x14(r25)
    lbz r26, 0x0(r4)
    lhz r27, 0x0(r3)
    addi r0, r3, 0x2
    stw r0, 0x14(r25)
    lwz r3, 0x28(r25)
    cmpwi r3, 0x0
    bgt @L_800F5A9C
    addi r3, r29, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r25)
    stw r0, 0x20(r1)
    b @L_800F5AB4
    @L_800F5A9C:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r25)
    add r3, r25, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x20(r1)
    @L_800F5AB4:
    lwz r30, 0x20(r1)
    clrlwi r31, r26, 16
    li r28, 0x0
    stw r30, 0x24(r1)
    b @L_800F5C80
    @L_800F5AC8:
    lwz r3, 0x28(r25)
    add r4, r30, r28
    cmpwi r3, 0x40
    ble @L_800F5AE8
    addi r3, r29, 0x0
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F5AFC
    @L_800F5AE8:
    addi r0, r3, 0x1
    slwi r3, r3, 2
    stw r0, 0x28(r25)
    addi r0, r3, 0x6c
    stwx r4, r25, r0
    @L_800F5AFC:
    lwz r0, lbl_8047E710(r2)
    cmplwi r31, 0x0
    stw r0, 0x14(r1)
    bne @L_800F5B28
    mr r4, r31
    addi r3, r29, 0x28
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x14(r1)
    stw r0, 0x1c(r1)
    b @L_800F5C48
    @L_800F5B28:
    rlwinm. r0, r26, 0, 24, 24
    beq @L_800F5B78
    lwz r3, 0x28(r25)
    cmpwi r3, 0x0
    bgt @L_800F5B54
    addi r3, r29, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r25)
    stw r0, 0x10(r1)
    b @L_800F5B6C
    @L_800F5B54:
    subi r0, r3, 0x1
    slwi r3, r0, 2
    stw r0, 0x28(r25)
    addi r0, r3, 0x6c
    lwzx r0, r25, r0
    stw r0, 0x10(r1)
    @L_800F5B6C:
    lwz r0, 0x10(r1)
    stw r0, 0x8(r1)
    b @L_800F5C40
    @L_800F5B78:
    lwz r3, 0x28(r25)
    cmpwi r3, 0x0
    bgt @L_800F5B9C
    addi r3, r29, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r25)
    stw r0, 0xc(r1)
    b @L_800F5BB4
    @L_800F5B9C:
    subi r0, r3, 0x1
    slwi r3, r0, 2
    stw r0, 0x28(r25)
    addi r0, r3, 0x6c
    lwzx r0, r25, r0
    stw r0, 0xc(r1)
    @L_800F5BB4:
    lwz r4, 0xc(r1)
    rlwinm. r0, r31, 0, 26, 26
    stw r4, 0x18(r1)
    beq @L_800F5BFC
    rlwinm. r0, r31, 0, 25, 25
    beq @L_800F5BE8
    lwz r0, 0x1c(r25)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r0, r3, 0x6c
    add r0, r25, r0
    stw r0, 0x8(r1)
    b @L_800F5C40
    @L_800F5BE8:
    lwz r3, 0x18(r25)
    slwi r0, r4, 2
    add r0, r3, r0
    stw r0, 0x8(r1)
    b @L_800F5C40
    @L_800F5BFC:
    rlwinm. r0, r31, 0, 25, 25
    beq @L_800F5C1C
    lwz r0, 0x1c(r25)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r3, r3, 0x6c
    add r3, r25, r3
    b @L_800F5C28
    @L_800F5C1C:
    lwz r3, 0x18(r25)
    slwi r0, r4, 2
    add r3, r3, r0
    @L_800F5C28:
    rlwinm. r0, r31, 0, 23, 23
    beq @L_800F5C38
    stw r3, 0x8(r1)
    b @L_800F5C40
    @L_800F5C38:
    lwz r0, 0x0(r3)
    stw r0, 0x8(r1)
    @L_800F5C40:
    lwz r0, 0x8(r1)
    stw r0, 0x1c(r1)
    @L_800F5C48:
    lwz r3, 0x28(r25)
    cmpwi r3, 0x40
    ble @L_800F5C64
    addi r3, r29, 0x0
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F5C7C
    @L_800F5C64:
    addi r0, r3, 0x1
    slwi r3, r3, 2
    stw r0, 0x28(r25)
    addi r3, r3, 0x6c
    lwz r0, 0x1c(r1)
    stwx r0, r25, r3
    @L_800F5C7C:
    addi r28, r28, 0x1
    @L_800F5C80:
    cmpw r28, r27
    blt @L_800F5AC8
    lmw r25, 0x34(r1)
    li r3, 0x1
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
