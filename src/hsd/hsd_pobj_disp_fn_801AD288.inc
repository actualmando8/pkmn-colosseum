    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr. r30, r3
    beq @L_801AD338
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    beq @L_801AD2C0
    lwz r3, 0x0(r30)
    bl fn_80193748
    cmplwi r3, 0x0
    bne @L_801AD2FC
@L_801AD2C0:
    lwz r0, lbl_8047B2E8(r13)
    cmplwi r0, 0x0
    beq @L_801AD2D4
    lwz r3, lbl_8047B2E8(r13)
    b @L_801AD2DC
@L_801AD2D4:
    lis r3, lbl_8036CCD0@ha
    addi r3, r3, lbl_8036CCD0@l
@L_801AD2DC:
    bl fn_80193828
    mr. r31, r3
    bne @L_801AD318
    addi r3, r2, -23016
    li r4, 0x2a9
    addi r5, r2, -22928
    bl fn_80196E10
    b @L_801AD318
@L_801AD2FC:
    bl fn_80193828
    mr. r31, r3
    bne @L_801AD318
    addi r3, r2, -23016
    li r4, 0x247
    addi r5, r2, -22928
    bl fn_80196E10
@L_801AD318:
    lwz r5, 0x0(r31)
    mr r3, r31
    mr r4, r30
    lwz r12, 0x44(r5)
    mtctr r12
    bctrl
    mr r3, r31
    b @L_801AD33C
@L_801AD338:
    li r3, 0x0
@L_801AD33C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
