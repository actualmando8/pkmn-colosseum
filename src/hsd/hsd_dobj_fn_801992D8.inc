    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr. r30, r3
    bne @L_801992FC
    li r3, 0x0
    b @L_8019938C
@L_801992FC:
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    beq @L_80199318
    lwz r3, 0x0(r30)
    bl fn_80193748
    cmplwi r3, 0x0
    bne @L_80199354
@L_80199318:
    lwz r0, lbl_8047B260(r13)
    cmplwi r0, 0x0
    beq @L_8019932C
    lwz r3, lbl_8047B260(r13)
    b @L_80199334
@L_8019932C:
    lis r3, lbl_8036C7A0@ha
    addi r3, r3, lbl_8036C7A0@l
@L_80199334:
    bl fn_80193828
    mr. r31, r3
    bne @L_80199370
    la r3, -23688(r2)
    li r4, 0x214
    la r5, -23680(r2)
    bl __assert
    b @L_80199370
@L_80199354:
    bl fn_80193828
    mr. r31, r3
    bne @L_80199370
    la r3, -23688(r2)
    li r4, 0x181
    la r5, -23680(r2)
    bl __assert
@L_80199370:
    lwz r5, 0x0(r31)
    mr r3, r31
    mr r4, r30
    lwz r12, 0x40(r5)
    mtctr r12
    bctrl
    mr r3, r31
@L_8019938C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
