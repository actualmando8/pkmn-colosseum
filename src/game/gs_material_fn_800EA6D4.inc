    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r5
    stw r29, 0x14(r1)
    mr. r29, r4
    stw r28, 0x10(r1)
    mr r28, r3
    beq @L_800EA740
    bne @L_800EA714
    la r3, lbl_8047CC00(r2)
    li r4, 0x25d
    la r5, lbl_8047CC08(r2)
    bl fn_80196E10
    @L_800EA714:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EA730
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EA730
    li r3, 0x1
    @L_800EA730:
    cmpwi r3, 0x0
    beq @L_800EA740
    mr r3, r29
    bl fn_8019D9DC
    @L_800EA740:
    lwz r31, 0x10(r29)
    cmplwi r31, 0x0
    beq @L_800EA78C
    bne @L_800EA760
    la r3, lbl_8047CC00(r2)
    li r4, 0x25d
    la r5, lbl_8047CC08(r2)
    bl fn_80196E10
    @L_800EA760:
    lwz r4, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EA77C
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EA77C
    li r3, 0x1
    @L_800EA77C:
    cmpwi r3, 0x0
    beq @L_800EA78C
    mr r3, r31
    bl fn_8019D9DC
    @L_800EA78C:
    lwz r3, 0x10(r29)
    mr r4, r30
    addi r3, r3, 0x44
    bl fn_800A2EB4
    mr r4, r30
    mr r5, r30
    addi r3, r29, 0x44
    bl fn_800A2D98
    cmplwi r28, 0x0
    beq @L_800EA7C4
    mr r3, r28
    mr r4, r30
    mr r5, r30
    bl fn_800A2D98
    @L_800EA7C4:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
