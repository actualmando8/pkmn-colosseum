    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr. r31, r3
    stw r30, 0x18(r1)
    mr r30, r5
    stw r29, 0x14(r1)
    mr r29, r4
    beq @L_801A3990
    cmplwi r29, 0x0
    beq @L_801A3960
    mr r12, r29
    mr r3, r31
    mr r4, r30
    li r5, 0x0
    mtctr r12
    bctrl
@L_801A3960:
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A3990
    lwz r31, 0x10(r31)
    b @L_801A3988
@L_801A3974:
    mr r3, r31
    mr r4, r29
    mr r5, r30
    bl fn_801A39AC
    lwz r31, 0x8(r31)
@L_801A3988:
    cmplwi r31, 0x0
    bne @L_801A3974
@L_801A3990:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
