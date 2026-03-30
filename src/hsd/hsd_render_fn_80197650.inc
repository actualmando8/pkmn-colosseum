    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmpwi r4, 0x1
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r5
    stw r3, 0x8(r1)
    bgt @L_8019769C
    lwz r0, 0x8(r1)
    cmplwi r0, 0x0
    beq @L_80197694
    lwz r3, 0x8(r1)
    li r0, 0x0
    stwx r0, r3, r28
@L_80197694:
    lwz r3, 0x8(r1)
    b @L_80197764
@L_8019769C:
    rlwinm r0, r4, 1, 31, 31
    lwz r30, 0x8(r1)
    add r0, r0, r4
    li r3, 0x0
    srawi r0, r0, 1
    subf r29, r0, r4
    b @L_801976C0
@L_801976B8:
    lwzx r30, r30, r28
    addi r3, r3, 0x1
@L_801976C0:
    cmpw r3, r0
    blt @L_801976B8
    lwz r3, 0x8(r1)
    mr r4, r0
    mr r5, r28
    bl fn_80197650
    mr r0, r3
    mr r3, r30
    mr r31, r0
    mr r4, r29
    mr r5, r28
    bl fn_80197650
    li r0, 0x0
    mr r30, r3
    stw r0, 0x8(r1)
    addi r3, r1, 0x8
    b @L_80197734
@L_80197704:
    lfs f1, 0x2c(r31)
    lfs f0, 0x2c(r30)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_80197724
    stw r31, 0x0(r3)
    lwzx r31, r31, r28
    b @L_8019772C
@L_80197724:
    stw r30, 0x0(r3)
    lwzx r30, r30, r28
@L_8019772C:
    lwz r0, 0x0(r3)
    add r3, r0, r28
@L_80197734:
    cmplwi r31, 0x0
    beq @L_80197744
    cmplwi r30, 0x0
    bne @L_80197704
@L_80197744:
    cmplwi r31, 0x0
    beq @L_80197754
    stw r31, 0x0(r3)
    b @L_80197760
@L_80197754:
    cmplwi r30, 0x0
    beq @L_80197760
    stw r30, 0x0(r3)
@L_80197760:
    lwz r3, 0x8(r1)
@L_80197764:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
