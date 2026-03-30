    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    lwz r30, lbl_8047AE10(r13)
    li r3, 0x0
    mr r4, r30
    bl fn_801F4354
    mr r31, r3
    li r3, 0x0
    bl fn_801F18DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80131F7C
    cmplwi r31, 0x0
    beq @L_80131F7C
    mr r3, r31
    bl fn_801F8100
    mr r4, r3
    li r3, 0x4d
    bl fn_80132A38
    mr r3, r30
    bl fn_802037DC
    mr r4, r3
    li r3, 0x57
    bl fn_80132A38
    li r3, 0x7721
    bl fn_800FA280
    b @L_80131F84
    @L_80131F7C:
    mr r3, r30
    bl fn_802037DC
    @L_80131F84:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
