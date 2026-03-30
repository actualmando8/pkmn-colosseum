    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr. r31, r3
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    beq @L_801A1F0C
    cmplwi r31, 0x0
    beq @L_801A1D10
    cmplwi r31, 0x0
    beq @L_801A1CD8
    cmplwi r31, 0x0
    bne @L_801A1BC8
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A1BC8:
    lwz r0, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A1BE8
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A1BE8
    li r3, 0x1
@L_801A1BE8:
    cmpwi r3, 0x0
    bne @L_801A1CD8
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 8, 8
    beq @L_801A1C68
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 7, 7
    bne @L_801A1CD8
    lwz r0, 0xc(r31)
    cmplwi r0, 0x0
    beq @L_801A1CD8
    lwz r30, 0xc(r31)
    cmplwi r30, 0x0
    bne @L_801A1C30
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A1C30:
    lwz r0, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A1C50
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A1C50
    li r3, 0x1
@L_801A1C50:
    cmpwi r3, 0x0
    beq @L_801A1CD8
    lwz r0, 0x14(r31)
    ori r0, r0, 0x40
    stw r0, 0x14(r31)
    b @L_801A1CD8
@L_801A1C68:
    lwz r0, 0xc(r31)
    cmplwi r0, 0x0
    beq @L_801A1C84
    lwz r3, 0xc(r31)
    lwz r0, 0x14(r3)
    rlwinm. r0, r0, 0, 25, 25
    bne @L_801A1CCC
@L_801A1C84:
    lwz r0, 0x14(r31)
    rlwinm r3, r0, 0, 9, 10
    subis r0, r3, 0x20
    cmplwi r0, 0x0
    beq @L_801A1CCC
    lwz r0, 0x14(r31)
    rlwinm r3, r0, 0, 9, 10
    subis r0, r3, 0x40
    cmplwi r0, 0x0
    beq @L_801A1CCC
    lwz r0, 0x14(r31)
    rlwinm r3, r0, 0, 9, 10
    subis r0, r3, 0x60
    cmplwi r0, 0x0
    beq @L_801A1CCC
    lwz r0, 0x80(r31)
    cmplwi r0, 0x0
    beq @L_801A1CD8
@L_801A1CCC:
    lwz r0, 0x14(r31)
    ori r0, r0, 0x40
    stw r0, 0x14(r31)
@L_801A1CD8:
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
    beq @L_801A1D10
    lwz r3, 0x18(r31)
    bl fn_80199568
@L_801A1D10:
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A1F0C
    lwz r31, 0x10(r31)
    b @L_801A1F04
@L_801A1D24:
    cmplwi r31, 0x0
    beq @L_801A1F00
    cmplwi r31, 0x0
    beq @L_801A1E38
    cmplwi r31, 0x0
    beq @L_801A1E00
    mr r3, r31
    bl fn_8019D980
    cmpwi r3, 0x0
    bne @L_801A1E00
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 8, 8
    beq @L_801A1D90
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 7, 7
    bne @L_801A1E00
    lwz r0, 0xc(r31)
    cmplwi r0, 0x0
    beq @L_801A1E00
    lwz r3, 0xc(r31)
    bl fn_8019D980
    cmpwi r3, 0x0
    beq @L_801A1E00
    lwz r0, 0x14(r31)
    ori r0, r0, 0x40
    stw r0, 0x14(r31)
    b @L_801A1E00
@L_801A1D90:
    lwz r0, 0xc(r31)
    cmplwi r0, 0x0
    beq @L_801A1DAC
    lwz r3, 0xc(r31)
    lwz r0, 0x14(r3)
    rlwinm. r0, r0, 0, 25, 25
    bne @L_801A1DF4
@L_801A1DAC:
    lwz r0, 0x14(r31)
    rlwinm r3, r0, 0, 9, 10
    subis r0, r3, 0x20
    cmplwi r0, 0x0
    beq @L_801A1DF4
    lwz r0, 0x14(r31)
    rlwinm r3, r0, 0, 9, 10
    subis r0, r3, 0x40
    cmplwi r0, 0x0
    beq @L_801A1DF4
    lwz r0, 0x14(r31)
    rlwinm r3, r0, 0, 9, 10
    subis r0, r3, 0x60
    cmplwi r0, 0x0
    beq @L_801A1DF4
    lwz r0, 0x80(r31)
    cmplwi r0, 0x0
    beq @L_801A1E00
@L_801A1DF4:
    lwz r0, 0x14(r31)
    ori r0, r0, 0x40
    stw r0, 0x14(r31)
@L_801A1E00:
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
    beq @L_801A1E38
    lwz r3, 0x18(r31)
    bl fn_80199568
@L_801A1E38:
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A1F00
    lwz r30, 0x10(r31)
    b @L_801A1EF8
@L_801A1E4C:
    cmplwi r30, 0x0
    beq @L_801A1EF4
    cmplwi r30, 0x0
    beq @L_801A1E9C
    mr r3, r30
    bl fn_801A3D04
    lwz r5, 0x0(r30)
    mr r4, r30
    lwz r3, 0x7c(r30)
    lwz r5, 0x50(r5)
    bl fn_801C27F4
    lwz r3, 0x80(r30)
    bl fn_801B0040
    lwz r0, 0x14(r30)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_801A1E9C
    lwz r3, 0x18(r30)
    bl fn_80199568
@L_801A1E9C:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A1EF4
    lwz r28, 0x10(r30)
    b @L_801A1EEC
@L_801A1EB0:
    cmplwi r28, 0x0
    beq @L_801A1EE8
    mr r3, r28
    bl fn_801A1F2C
    lwz r0, 0x14(r28)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A1EE8
    lwz r29, 0x10(r28)
    b @L_801A1EE0
@L_801A1ED4:
    mr r3, r29
    bl fn_801A1B7C
    lwz r29, 0x8(r29)
@L_801A1EE0:
    cmplwi r29, 0x0
    bne @L_801A1ED4
@L_801A1EE8:
    lwz r28, 0x8(r28)
@L_801A1EEC:
    cmplwi r28, 0x0
    bne @L_801A1EB0
@L_801A1EF4:
    lwz r30, 0x8(r30)
@L_801A1EF8:
    cmplwi r30, 0x0
    bne @L_801A1E4C
@L_801A1F00:
    lwz r31, 0x8(r31)
@L_801A1F04:
    cmplwi r31, 0x0
    bne @L_801A1D24
@L_801A1F0C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
