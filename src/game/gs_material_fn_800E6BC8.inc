    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    mr r30, r3
    stw r29, 0x24(r1)
    lbz r0, 0x80(r3)
    cmplwi r0, 0x0
    bne @L_800E6D94
    lwz r29, 0x8(r30)
    bl fn_8019F718
    mr. r31, r3
    bne @L_800E6C08
    li r3, 0x0
    b @L_800E6DA4
    @L_800E6C08:
    lwz r0, 0x0(r30)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E6C18
    lwz r29, 0x10(r29)
    @L_800E6C18:
    cmplwi r29, 0x0
    bne @L_800E6C28
    li r29, 0x0
    b @L_800E6C2C
    @L_800E6C28:
    lwz r29, 0x10(r29)
    @L_800E6C2C:
    cmplwi r29, 0x0
    bne @L_800E6C5C
    mr r3, r29
    mr r4, r31
    bl fn_801A015C
    b @L_800E6C84
    b @L_800E6C5C
    @L_800E6C48:
    cmplwi r29, 0x0
    bne @L_800E6C58
    li r29, 0x0
    b @L_800E6C5C
    @L_800E6C58:
    lwz r29, 0x8(r29)
    @L_800E6C5C:
    cmplwi r29, 0x0
    bne @L_800E6C6C
    li r0, 0x0
    b @L_800E6C70
    @L_800E6C6C:
    lwz r0, 0x8(r29)
    @L_800E6C70:
    cmplwi r0, 0x0
    bne @L_800E6C48
    mr r3, r29
    mr r4, r31
    bl fn_8019FF74
    @L_800E6C84:
    mr r3, r29
    bl fn_8019FF30
    mr r4, r3
    mr r3, r31
    bl fn_8019FE8C
    lfs f1, lbl_8047CB98(r2)
    addi r3, r1, 0x8
    addi r4, r30, 0x5c
    addi r5, r30, 0x68
    bl fn_800E090C
    cmplwi r31, 0x0
    bne @L_800E6CC4
    la r3, lbl_8047CB9C(r2)
    li r4, 0x3a9
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E6CC4:
    addic. r0, r1, 0x8
    bne @L_800E6CE0
    lis r4, lbl_80270E60@ha
    la r3, lbl_8047CB9C(r2)
    addi r5, r4, lbl_80270E60@l
    li r4, 0x3aa
    bl fn_80196E10
    @L_800E6CE0:
    lwz r3, 0x8(r1)
    lwz r0, 0xc(r1)
    stw r3, 0x38(r31)
    stw r0, 0x3c(r31)
    lwz r0, 0x10(r1)
    stw r0, 0x40(r31)
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E6D4C
    cmplwi r31, 0x0
    beq @L_800E6D4C
    bne @L_800E6D20
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E6D20:
    lwz r4, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E6D3C
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E6D3C
    li r3, 0x1
    @L_800E6D3C:
    cmpwi r3, 0x0
    bne @L_800E6D4C
    mr r3, r31
    bl fn_8019D620
    @L_800E6D4C:
    cmplwi r31, 0x0
    beq @L_800E6D94
    bne @L_800E6D68
    la r3, lbl_8047CB9C(r2)
    li r4, 0x25d
    la r5, lbl_8047CBA4(r2)
    bl fn_80196E10
    @L_800E6D68:
    lwz r4, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E6D84
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E6D84
    li r3, 0x1
    @L_800E6D84:
    cmpwi r3, 0x0
    beq @L_800E6D94
    mr r3, r31
    bl fn_8019D9DC
    @L_800E6D94:
    lbz r4, 0x80(r30)
    li r3, 0x1
    addi r0, r4, 0x1
    stb r0, 0x80(r30)
    @L_800E6DA4:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
