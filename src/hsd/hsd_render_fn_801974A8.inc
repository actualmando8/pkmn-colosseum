    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    bl fn_801942B8
    addi r0, r3, 0x54
    lwz r30, lbl_8047B250(r13)
    mr r31, r0
    b @L_80197510
@L_801974D0:
    lwz r0, 0x30(r30)
    lwz r3, 0x34(r30)
    cmplwi r0, 0x0
    beq @L_801974E8
    lwz r4, 0x30(r30)
    b @L_801974EC
@L_801974E8:
    mr r4, r31
@L_801974EC:
    lwz r8, 0x34(r30)
    mr r5, r30
    lwz r7, 0x38(r30)
    li r6, 0x4
    lwz r8, 0x0(r8)
    lwz r12, 0x48(r8)
    mtctr r12
    bctrl
    lwz r30, 0x3c(r30)
@L_80197510:
    cmplwi r30, 0x0
    bne @L_801974D0
    lwz r30, lbl_8047B258(r13)
    b @L_80197560
@L_80197520:
    lwz r0, 0x30(r30)
    lwz r3, 0x34(r30)
    cmplwi r0, 0x0
    beq @L_80197538
    lwz r4, 0x30(r30)
    b @L_8019753C
@L_80197538:
    mr r4, r31
@L_8019753C:
    lwz r8, 0x34(r30)
    mr r5, r30
    lwz r7, 0x38(r30)
    li r6, 0x2
    lwz r8, 0x0(r8)
    lwz r12, 0x48(r8)
    mtctr r12
    bctrl
    lwz r30, 0x40(r30)
@L_80197560:
    cmplwi r30, 0x0
    bne @L_80197520
    lwz r30, lbl_8047B24C(r13)
    b @L_8019759C
@L_80197570:
    lwz r0, 0x30(r30)
    lwz r31, 0x44(r30)
    cmplwi r0, 0x0
    beq @L_80197588
    lwz r3, 0x30(r30)
    bl fn_801A84F0
@L_80197588:
    lis r3, lbl_80465348@ha
    mr r4, r30
    addi r3, r3, lbl_80465348@l
    bl fn_801AA498
    mr r30, r31
@L_8019759C:
    cmplwi r30, 0x0
    bne @L_80197570
    li r9, 0x0
    la r8, lbl_8047B24C(r13)
    li r7, 0x0
    la r6, lbl_8047B250(r13)
    li r5, 0x0
    li r4, 0x0
    la r3, lbl_8047B258(r13)
    li r0, 0x0
    stw r9, lbl_8047B24C(r13)
    stw r8, lbl_80478C64(r13)
    stw r7, lbl_8047B250(r13)
    stw r6, lbl_80478C68(r13)
    stw r5, lbl_8047B254(r13)
    stw r4, lbl_8047B258(r13)
    stw r3, lbl_80478C6C(r13)
    stw r0, lbl_8047B25C(r13)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
