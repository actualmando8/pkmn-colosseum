    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    stw r30, 0x8(r1)
    beq @L_801A20B0
    cmplwi r31, 0x0
    beq @L_801A2078
    cmplwi r31, 0x0
    bne @L_801A1F68
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A1F68:
    lwz r0, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A1F88
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A1F88
    li r3, 0x1
@L_801A1F88:
    cmpwi r3, 0x0
    bne @L_801A2078
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 8, 8
    beq @L_801A2008
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 7, 7
    bne @L_801A2078
    lwz r0, 0xc(r31)
    cmplwi r0, 0x0
    beq @L_801A2078
    lwz r30, 0xc(r31)
    cmplwi r30, 0x0
    bne @L_801A1FD0
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A1FD0:
    lwz r0, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A1FF0
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A1FF0
    li r3, 0x1
@L_801A1FF0:
    cmpwi r3, 0x0
    beq @L_801A2078
    lwz r0, 0x14(r31)
    ori r0, r0, 0x40
    stw r0, 0x14(r31)
    b @L_801A2078
@L_801A2008:
    lwz r0, 0xc(r31)
    cmplwi r0, 0x0
    beq @L_801A2024
    lwz r3, 0xc(r31)
    lwz r0, 0x14(r3)
    rlwinm. r0, r0, 0, 25, 25
    bne @L_801A206C
@L_801A2024:
    lwz r0, 0x14(r31)
    rlwinm r3, r0, 0, 9, 10
    subis r0, r3, 0x20
    cmplwi r0, 0x0
    beq @L_801A206C
    lwz r0, 0x14(r31)
    rlwinm r3, r0, 0, 9, 10
    subis r0, r3, 0x40
    cmplwi r0, 0x0
    beq @L_801A206C
    lwz r0, 0x14(r31)
    rlwinm r3, r0, 0, 9, 10
    subis r0, r3, 0x60
    cmplwi r0, 0x0
    beq @L_801A206C
    lwz r0, 0x80(r31)
    cmplwi r0, 0x0
    beq @L_801A2078
@L_801A206C:
    lwz r0, 0x14(r31)
    ori r0, r0, 0x40
    stw r0, 0x14(r31)
@L_801A2078:
    lwz r5, 0x0(r31)
    mr r4, r31
    lwz r3, 0x7c(r31)
    lwz r5, 0x50(r5)
    bl fn_801C27F4
    lwz r3, 0x80(r31)
    bl fn_801B0040
    lwz r0, 0x14(r31)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_801A20B0
    lwz r3, 0x18(r31)
    bl fn_80199568
@L_801A20B0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
