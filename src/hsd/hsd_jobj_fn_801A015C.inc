    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r4
    stw r28, 0x10(r1)
    mr. r28, r3
    lis r3, lbl_80274AA0@ha
    addi r31, r3, lbl_80274AA0@l
    beq @L_801A0290
    cmplwi r29, 0x0
    bne @L_801A0198
    b @L_801A0290
    @L_801A0198:
    lwz r0, 0xc(r29)
    cmplwi r0, 0x0
    beq @L_801A01C0
    addi r3, r31, 0x178
    crxor 6, 6, 6
    bl OSReport
    addi r5, r31, 0x194
    la r3, lbl_8047DB20(r2)
    li r4, 0x552
    bl fn_80196E10
    @L_801A01C0:
    lwz r0, 0x8(r29)
    cmplwi r0, 0x0
    beq @L_801A01E8
    addi r3, r31, 0x1ac
    crxor 6, 6, 6
    bl OSReport
    addi r5, r31, 0x1cc
    la r3, lbl_8047DB20(r2)
    li r4, 0x553
    bl fn_80196E10
    @L_801A01E8:
    lwz r0, 0x10(r28)
    cmplwi r0, 0x0
    bne @L_801A01FC
    stw r29, 0x10(r28)
    b @L_801A024C
    @L_801A01FC:
    lwz r0, 0x14(r28)
    rlwinm. r0, r0, 0, 19, 19
    beq @L_801A0218
    addi r5, r31, 0x1e0
    la r3, lbl_8047DB20(r2)
    li r4, 0x559
    bl fn_80196E10
    @L_801A0218:
    lwz r30, 0x10(r28)
    b @L_801A023C
    @L_801A0220:
    cmplw r30, r29
    bne @L_801A0238
    addi r5, r31, 0x200
    la r3, lbl_8047DB20(r2)
    li r4, 0x55c
    bl fn_80196E10
    @L_801A0238:
    lwz r30, 0x8(r30)
    @L_801A023C:
    lwz r0, 0x8(r30)
    cmplwi r0, 0x0
    bne @L_801A0220
    stw r29, 0x8(r30)
    @L_801A024C:
    stw r28, 0xc(r29)
    mr r4, r28
    lwz r0, 0x14(r29)
    lwz r3, 0x14(r29)
    slwi r0, r0, 10
    or r0, r3, r0
    rlwinm r3, r0, 0, 1, 3
    b @L_801A0288
    @L_801A026C:
    lwz r0, 0x14(r4)
    andc. r0, r3, r0
    beq @L_801A0290
    lwz r0, 0x14(r4)
    or r0, r0, r3
    stw r0, 0x14(r4)
    lwz r4, 0xc(r4)
    @L_801A0288:
    cmplwi r4, 0x0
    bne @L_801A026C
    @L_801A0290:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
