    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    lis r4, lbl_80271068@ha
    stw r0, 0x44(r1)
    stw r31, 0x3c(r1)
    addi r31, r4, lbl_80271068@l
    stw r30, 0x38(r1)
    stw r29, 0x34(r1)
    stw r28, 0x30(r1)
    mr r28, r3
    lwz r3, 0x14(r3)
    addi r0, r3, 0x1
    stw r0, 0x14(r28)
    lbz r29, 0x0(r3)
    lwz r0, lbl_8047E710(r2)
    cmplwi r29, 0x0
    stw r0, 0x14(r1)
    bne @L_800F5854
    addi r3, r31, 0x28
    clrlwi r4, r29, 16
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x14(r1)
    stw r0, 0x1c(r1)
    b @L_800F5978
    @L_800F5854:
    rlwinm. r0, r29, 0, 24, 24
    clrlwi r30, r29, 16
    beq @L_800F58A8
    lwz r3, 0x28(r28)
    cmpwi r3, 0x0
    bgt @L_800F5884
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r28)
    stw r0, 0x10(r1)
    b @L_800F589C
    @L_800F5884:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r28)
    add r3, r28, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x10(r1)
    @L_800F589C:
    lwz r0, 0x10(r1)
    stw r0, 0x8(r1)
    b @L_800F5970
    @L_800F58A8:
    lwz r3, 0x28(r28)
    cmpwi r3, 0x0
    bgt @L_800F58CC
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r28)
    stw r0, 0xc(r1)
    b @L_800F58E4
    @L_800F58CC:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r28)
    add r3, r28, r0
    lwz r0, 0x6c(r3)
    stw r0, 0xc(r1)
    @L_800F58E4:
    lwz r4, 0xc(r1)
    rlwinm. r0, r30, 0, 26, 26
    stw r4, 0x18(r1)
    beq @L_800F592C
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F5918
    lwz r0, 0x1c(r28)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r0, r3, 0x6c
    add r0, r28, r0
    stw r0, 0x8(r1)
    b @L_800F5970
    @L_800F5918:
    lwz r3, 0x18(r28)
    slwi r0, r4, 2
    add r0, r3, r0
    stw r0, 0x8(r1)
    b @L_800F5970
    @L_800F592C:
    rlwinm. r0, r30, 0, 25, 25
    beq @L_800F594C
    lwz r0, 0x1c(r28)
    add r0, r0, r4
    slwi r3, r0, 2
    addi r3, r3, 0x6c
    add r3, r28, r3
    b @L_800F5958
    @L_800F594C:
    lwz r3, 0x18(r28)
    slwi r0, r4, 2
    add r3, r3, r0
    @L_800F5958:
    rlwinm. r0, r30, 0, 23, 23
    beq @L_800F5968
    stw r3, 0x8(r1)
    b @L_800F5970
    @L_800F5968:
    lwz r0, 0x0(r3)
    stw r0, 0x8(r1)
    @L_800F5970:
    lwz r0, 0x8(r1)
    stw r0, 0x1c(r1)
    @L_800F5978:
    clrlwi r0, r29, 26
    lwz r3, 0x1c(r1)
    cmpwi r0, 0x2
    stw r3, 0x24(r1)
    bne @L_800F5998
    cntlzw r0, r3
    srwi r0, r0, 5
    stw r0, 0x20(r1)
    @L_800F5998:
    clrlwi r0, r29, 26
    cmpwi r0, 0x2
    beq @L_800F59E4
    lfs f1, 0x24(r1)
    lfs f0, lbl_8047CCBC(r2)
    fcmpu cr0, f1, f0
    mfcr r3
    lis r0, 0x4330
    extrwi r3, r3, 1, 2
    stw r0, 0x28(r1)
    xori r0, r3, 0x1
    lfd f1, lbl_8047CCC0(r2)
    cntlzw r0, r0
    srwi r0, r0, 5
    xoris r0, r0, 0x8000
    stw r0, 0x2c(r1)
    lfd f0, 0x28(r1)
    fsubs f0, f0, f1
    stfs f0, 0x20(r1)
    @L_800F59E4:
    lwz r4, 0x28(r28)
    cmpwi r4, 0x40
    ble @L_800F5A00
    addi r3, r31, 0x0
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F5A18
    @L_800F5A00:
    addi r3, r4, 0x1
    slwi r0, r4, 2
    stw r3, 0x28(r28)
    add r3, r28, r0
    lwz r0, 0x20(r1)
    stw r0, 0x6c(r3)
    @L_800F5A18:
    lwz r0, 0x44(r1)
    li r3, 0x1
    lwz r31, 0x3c(r1)
    lwz r30, 0x38(r1)
    lwz r29, 0x34(r1)
    lwz r28, 0x30(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
