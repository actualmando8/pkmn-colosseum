    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r5
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr r29, r3
    bl fn_8009B608
    cmplwi r3, 0xf
    blt @L_800EEAD0
    li r3, 0x1
    b @L_800EEB18
    @L_800EEAD0:
    clrlwi. r0, r30, 27
    bne @L_800EEAE0
    clrlwi. r0, r29, 27
    beq @L_800EEAE8
    @L_800EEAE0:
    li r3, 0x2
    b @L_800EEB18
    @L_800EEAE8:
    clrlwi. r0, r31, 27
    beq @L_800EEAF8
    li r3, 0x2
    b @L_800EEB18
    @L_800EEAF8:
    mr r3, r29
    mr r4, r30
    mr r5, r31
    bl fn_8009B55C
    lwz r0, lbl_8047ABDC(r13)
    add r0, r0, r3
    li r3, 0x0
    stw r0, lbl_8047ABDC(r13)
    @L_800EEB18:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
