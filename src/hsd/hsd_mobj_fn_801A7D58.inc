    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    lwz r0, 0x4(r4)
    stw r0, 0x4(r3)
    lwz r3, 0x8(r4)
    bl fn_801BE4CC
    mr r0, r3
    li r3, 0x14
    stw r0, 0x8(r29)
    bl fn_80193B10
    mr. r31, r3
    bne @L_801A7DB0
    la r3, lbl_8047DC18(r2)
    li r4, 0x466
    la r5, lbl_8047DC28(r2)
    bl fn_80196E10
    @L_801A7DB0:
    mr r3, r31
    li r4, 0x0
    li r5, 0x14
    bl memset
    lfs f0, lbl_8047DC2C(r2)
    mr r0, r31
    li r5, 0x14
    stfs f0, 0xc(r31)
    stw r0, 0xc(r29)
    lwz r3, 0xc(r29)
    lwz r4, 0xc(r30)
    bl memcpy
    lwz r0, 0x4(r29)
    ori r0, r0, 0x1000
    stw r0, 0x4(r29)
    lwz r0, 0x14(r30)
    cmplwi r0, 0x0
    beq @L_801A7E14
    li r3, 0xc
    bl fn_80193B10
    stw r3, 0x10(r29)
    li r5, 0xc
    lwz r3, 0x10(r29)
    lwz r4, 0x14(r30)
    bl memcpy
    @L_801A7E14:
    li r0, 0x0
    li r3, 0x0
    stw r0, 0x14(r29)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
