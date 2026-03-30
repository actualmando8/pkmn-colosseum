    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    bl fn_800ECCA8
    addi r3, r29, 0x4c
    bl fn_80191460
    lwz r31, 0x144(r29)
    li r0, 0x0
    stw r0, 0x144(r29)
    lwz r0, 0x0(r29)
    rlwinm. r0, r0, 0, 29, 29
    beq @L_800EB308
    lfs f1, lbl_8047CC28(r2)
    mr r3, r29
    bl fn_800ECA78
    mr r3, r29
    bl fn_800EC134
    lwz r30, 0x8(r29)
    cmplwi r30, 0x0
    beq @L_800EB308
    bne @L_800EB2DC
    la r3, lbl_8047CC2C(r2)
    li r4, 0x25d
    la r5, lbl_8047CC34(r2)
    bl fn_80196E10
    @L_800EB2DC:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EB2F8
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EB2F8
    li r3, 0x1
    @L_800EB2F8:
    cmpwi r3, 0x0
    beq @L_800EB308
    mr r3, r30
    bl fn_8019D9DC
    @L_800EB308:
    lis r4, fn_800EB414@ha
    mr r3, r29
    addi r5, r4, fn_800EB414@l
    mr r6, r29
    li r4, 0x1
    bl fn_800E9E34
    stw r31, 0x144(r29)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
