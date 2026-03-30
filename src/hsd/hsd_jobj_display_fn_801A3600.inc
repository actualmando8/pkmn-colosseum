    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    stw r30, 0x18(r1)
    lwz r30, 0xc(r3)
    cmplwi r30, 0x0
    beq @L_801A366C
    cmplwi r30, 0x0
    bne @L_801A363C
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A363C:
    lwz r0, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A365C
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A365C
    li r3, 0x1
@L_801A365C:
    cmpwi r3, 0x0
    beq @L_801A366C
    mr r3, r30
    bl fn_8019D9DC
@L_801A366C:
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 28, 28
    beq @L_801A36F0
    lwz r0, 0xc(r31)
    cmplwi r0, 0x0
    beq @L_801A36D0
    lwz r3, 0xc(r31)
    lwz r0, 0x74(r3)
    cmplwi r0, 0x0
    beq @L_801A36D0
    lwz r0, 0x74(r31)
    cmplwi r0, 0x0
    bne @L_801A36A8
    bl fn_801A85A4
    stw r3, 0x74(r31)
@L_801A36A8:
    lwz r3, 0xc(r31)
    lwz r4, 0x74(r31)
    lwz r5, 0x74(r3)
    lwz r3, 0x0(r5)
    lwz r0, 0x4(r5)
    stw r3, 0x0(r4)
    stw r0, 0x4(r4)
    lwz r0, 0x8(r5)
    stw r0, 0x8(r4)
    b @L_801A3794
@L_801A36D0:
    lwz r0, 0x74(r31)
    cmplwi r0, 0x0
    beq @L_801A3794
    lwz r3, 0x74(r31)
    bl fn_801A8570
    li r0, 0x0
    stw r0, 0x74(r31)
    b @L_801A3794
@L_801A36F0:
    lwz r0, 0x74(r31)
    cmplwi r0, 0x0
    bne @L_801A3704
    bl fn_801A85A4
    stw r3, 0x74(r31)
@L_801A3704:
    lwz r0, 0xc(r31)
    cmplwi r0, 0x0
    beq @L_801A3778
    lwz r3, 0xc(r31)
    lwz r0, 0x74(r3)
    cmplwi r0, 0x0
    beq @L_801A3778
    lwz r3, 0xc(r31)
    lfs f1, 0x2c(r31)
    lwz r4, 0x74(r3)
    lwz r3, 0x74(r31)
    lfs f0, 0x0(r4)
    fmuls f0, f1, f0
    stfs f0, 0x0(r3)
    lwz r3, 0xc(r31)
    lfs f1, 0x30(r31)
    lwz r4, 0x74(r3)
    lwz r3, 0x74(r31)
    lfs f0, 0x4(r4)
    fmuls f0, f1, f0
    stfs f0, 0x4(r3)
    lwz r3, 0xc(r31)
    lfs f1, 0x34(r31)
    lwz r4, 0x74(r3)
    lwz r3, 0x74(r31)
    lfs f0, 0x8(r4)
    fmuls f0, f1, f0
    stfs f0, 0x8(r3)
    b @L_801A3794
@L_801A3778:
    lwz r4, 0x74(r31)
    lwz r3, 0x2c(r31)
    lwz r0, 0x30(r31)
    stw r3, 0x0(r4)
    stw r0, 0x4(r4)
    lwz r0, 0x34(r31)
    stw r0, 0x8(r4)
@L_801A3794:
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_801A37F4
    lwz r0, 0xc(r31)
    addi r3, r31, 0x44
    addi r4, r31, 0x2c
    addi r5, r31, 0x1c
    cmplwi r0, 0x0
    addi r6, r31, 0x38
    li r8, 0x0
    beq @L_801A37D4
    lwz r7, 0xc(r31)
    lwz r0, 0x74(r7)
    cmplwi r0, 0x0
    beq @L_801A37D4
    li r8, 0x1
@L_801A37D4:
    cmpwi r8, 0x0
    beq @L_801A37E8
    lwz r7, 0xc(r31)
    lwz r7, 0x74(r7)
    b @L_801A37EC
@L_801A37E8:
    li r7, 0x0
@L_801A37EC:
    bl fn_801A86B4
    b @L_801A3844
@L_801A37F4:
    lwz r0, 0xc(r31)
    addi r3, r31, 0x44
    addi r4, r31, 0x2c
    addi r5, r31, 0x1c
    cmplwi r0, 0x0
    addi r6, r31, 0x38
    li r8, 0x0
    beq @L_801A3828
    lwz r7, 0xc(r31)
    lwz r0, 0x74(r7)
    cmplwi r0, 0x0
    beq @L_801A3828
    li r8, 0x1
@L_801A3828:
    cmpwi r8, 0x0
    beq @L_801A383C
    lwz r7, 0xc(r31)
    lwz r7, 0x74(r7)
    b @L_801A3840
@L_801A383C:
    li r7, 0x0
@L_801A3840:
    bl fn_801A8884
@L_801A3844:
    lwz r0, 0xc(r31)
    cmplwi r0, 0x0
    beq @L_801A3864
    lwz r3, 0xc(r31)
    addi r4, r31, 0x44
    addi r5, r31, 0x44
    addi r3, r3, 0x44
    bl fn_800A2D98
@L_801A3864:
    lwz r0, 0x7c(r31)
    cmplwi r0, 0x0
    beq @L_801A3900
    lwz r3, 0x7c(r31)
    lwz r0, 0x18(r3)
    cmplwi r0, 0x0
    beq @L_801A3900
    lwz r3, 0x7c(r31)
    lwz r30, 0x18(r3)
    cmplwi r30, 0x0
    beq @L_801A38D8
    cmplwi r30, 0x0
    bne @L_801A38A8
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A38A8:
    lwz r0, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A38C8
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A38C8
    li r3, 0x1
@L_801A38C8:
    cmpwi r3, 0x0
    beq @L_801A38D8
    mr r3, r30
    bl fn_8019D9DC
@L_801A38D8:
    addi r3, r30, 0x44
    addi r4, r31, 0x38
    addi r5, r1, 0x8
    bl fn_800A37CC
    lfs f0, 0x8(r1)
    stfs f0, 0x50(r31)
    lfs f0, 0xc(r1)
    stfs f0, 0x60(r31)
    lfs f0, 0x10(r1)
    stfs f0, 0x70(r31)
@L_801A3900:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
