    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stw r31, 0x3c(r1)
    stw r30, 0x38(r1)
    stw r29, 0x34(r1)
    mr r29, r3
    lis r3, lbl_80271068@ha
    lwz r4, 0x14(r29)
    addi r31, r3, lbl_80271068@l
    addi r0, r4, 0x1
    stw r0, 0x14(r29)
    lwz r3, 0x28(r29)
    lbz r30, 0x0(r4)
    cmpwi r3, 0x0
    bgt @L_800F5CF8
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r29)
    stw r0, 0x20(r1)
    b @L_800F5D10
    @L_800F5CF8:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r29)
    add r3, r29, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x20(r1)
    @L_800F5D10:
    lwz r4, 0x28(r29)
    lwz r5, 0x20(r1)
    cmpwi r4, 0x40
    stw r5, 0x24(r1)
    ble @L_800F5D34
    addi r3, r31, 0x0
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F5D48
    @L_800F5D34:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r29)
    add r3, r29, r0
    stw r5, 0x6c(r3)
    @L_800F5D48:
    ori r3, r30, 0x100
    lwz r0, lbl_8047E710(r2)
    clrlwi. r30, r3, 16
    stw r0, 0x14(r1)
    bne @L_800F5D78
    mr r4, r30
    addi r3, r31, 0x28
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x14(r1)
    stw r0, 0x1c(r1)
    b @L_800F5E98
    @L_800F5D78:
    rlwinm. r0, r30, 0, 24, 24
    beq @L_800F5DC8
    lwz r3, 0x28(r29)
    cmpwi r3, 0x0
    bgt @L_800F5DA4
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r29)
    stw r0, 0x10(r1)
    b @L_800F5DBC
    @L_800F5DA4:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r29)
    add r3, r29, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x10(r1)
    @L_800F5DBC:
    lwz r0, 0x10(r1)
    stw r0, 0x8(r1)
    b @L_800F5E90
    @L_800F5DC8:
    lwz r3, 0x28(r29)
    cmpwi r3, 0x0
    bgt @L_800F5DEC
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r29)
    stw r0, 0xc(r1)
    b @L_800F5E04
    @L_800F5DEC:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r29)
    add r3, r29, r0
    lwz r0, 0x6c(r3)
    stw r0, 0xc(r1)
    @L_800F5E04:
    lwz r4, 0xc(r1)
    rlwinm. r0, r30, 0, 26, 26
    stw r4, 0x18(r1)
    beq @L_800F5E4C
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F5E38
    lwz r0, 0x1c(r29)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r0, r3, 0x6c
    add r0, r29, r0
    stw r0, 0x8(r1)
    b @L_800F5E90
    @L_800F5E38:
    lwz r3, 0x18(r29)
    slwi r0, r4, 2
    add r0, r3, r0
    stw r0, 0x8(r1)
    b @L_800F5E90
    @L_800F5E4C:
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F5E6C
    lwz r0, 0x1c(r29)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r3, r3, 0x6c
    add r3, r29, r3
    b @L_800F5E78
    @L_800F5E6C:
    lwz r3, 0x18(r29)
    slwi r0, r4, 2
    add r3, r3, r0
    @L_800F5E78:
    rlwinm. r0, r30, 0, 23, 23
    beq @L_800F5E88
    stw r3, 0x8(r1)
    b @L_800F5E90
    @L_800F5E88:
    lwz r0, 0x0(r3)
    stw r0, 0x8(r1)
    @L_800F5E90:
    lwz r0, 0x8(r1)
    stw r0, 0x1c(r1)
    @L_800F5E98:
    lwz r4, 0x28(r29)
    cmpwi r4, 0x40
    ble @L_800F5EB4
    addi r3, r31, 0x0
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F5ECC
    @L_800F5EB4:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r29)
    add r3, r29, r0
    lwz r0, 0x1c(r1)
    stw r0, 0x6c(r3)
    @L_800F5ECC:
    lwz r0, 0x44(r1)
    li r3, 0x1
    lwz r31, 0x3c(r1)
    lwz r30, 0x38(r1)
    lwz r29, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
