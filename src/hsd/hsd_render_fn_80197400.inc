    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    lwz r31, lbl_8047B24C(r13)
    b @L_80197448
@L_8019741C:
    lwz r0, 0x30(r31)
    lwz r30, 0x44(r31)
    cmplwi r0, 0x0
    beq @L_80197434
    lwz r3, 0x30(r31)
    bl fn_801A84F0
@L_80197434:
    lis r3, lbl_80465348@ha
    mr r4, r31
    addi r3, r3, lbl_80465348@l
    bl fn_801AA498
    mr r31, r30
@L_80197448:
    cmplwi r31, 0x0
    bne @L_8019741C
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
