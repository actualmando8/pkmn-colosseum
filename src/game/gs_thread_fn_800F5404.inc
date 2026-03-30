    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r4, lbl_80271068@ha
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    addi r31, r4, lbl_80271068@l
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    mr r29, r3
    lwz r3, 0x14(r3)
    addi r0, r3, 0x1
    stw r0, 0x14(r29)
    lbz r4, 0x0(r3)
    lwz r0, lbl_8047E710(r2)
    cmplwi r4, 0x0
    stw r0, 0x14(r1)
    bne @L_800F5464
    addi r3, r31, 0x28
    clrlwi r4, r4, 16
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x14(r1)
    stw r0, 0x1c(r1)
    b @L_800F5588
    @L_800F5464:
    rlwinm. r0, r4, 0, 24, 24
    clrlwi r30, r4, 16
    beq @L_800F54B8
    lwz r3, 0x28(r29)
    cmpwi r3, 0x0
    bgt @L_800F5494
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r29)
    stw r0, 0x10(r1)
    b @L_800F54AC
    @L_800F5494:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r29)
    add r3, r29, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x10(r1)
    @L_800F54AC:
    lwz r0, 0x10(r1)
    stw r0, 0x8(r1)
    b @L_800F5580
    @L_800F54B8:
    lwz r3, 0x28(r29)
    cmpwi r3, 0x0
    bgt @L_800F54DC
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r29)
    stw r0, 0xc(r1)
    b @L_800F54F4
    @L_800F54DC:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r29)
    add r3, r29, r0
    lwz r0, 0x6c(r3)
    stw r0, 0xc(r1)
    @L_800F54F4:
    lwz r4, 0xc(r1)
    rlwinm. r0, r30, 0, 26, 26
    stw r4, 0x18(r1)
    beq @L_800F553C
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F5528
    lwz r0, 0x1c(r29)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r0, r3, 0x6c
    add r0, r29, r0
    stw r0, 0x8(r1)
    b @L_800F5580
    @L_800F5528:
    lwz r3, 0x18(r29)
    slwi r0, r4, 2
    add r0, r3, r0
    stw r0, 0x8(r1)
    b @L_800F5580
    @L_800F553C:
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F555C
    lwz r0, 0x1c(r29)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r3, r3, 0x6c
    add r3, r29, r3
    b @L_800F5568
    @L_800F555C:
    lwz r3, 0x18(r29)
    slwi r0, r4, 2
    add r3, r3, r0
    @L_800F5568:
    rlwinm. r0, r30, 0, 23, 23
    beq @L_800F5578
    stw r3, 0x8(r1)
    b @L_800F5580
    @L_800F5578:
    lwz r0, 0x0(r3)
    stw r0, 0x8(r1)
    @L_800F5580:
    lwz r0, 0x8(r1)
    stw r0, 0x1c(r1)
    @L_800F5588:
    lwz r4, 0x28(r29)
    cmpwi r4, 0x40
    ble @L_800F55A4
    addi r3, r31, 0x0
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F55BC
    @L_800F55A4:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r29)
    add r3, r29, r0
    lwz r0, 0x1c(r1)
    stw r0, 0x6c(r3)
    @L_800F55BC:
    lwz r0, 0x34(r1)
    li r3, 0x1
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
