    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    stw r30, 0x8(r1)
    lwz r3, 0x64(r3)
    bl fn_801C25E4
    lwz r3, 0x5c(r31)
    cmplwi r3, 0x0
    beq @L_801BBC48
    li r4, 0x10
    bl fn_80193AF0
    @L_801BBC48:
    lwz r3, 0xa8(r31)
    cmplwi r3, 0x0
    beq @L_801BBC5C
    li r4, 0x20
    bl fn_80193AF0
    @L_801BBC5C:
    lwz r0, 0x6c(r31)
    cmplwi r0, 0x0
    beq @L_801BBCAC
    li r30, 0x0
    b @L_801BBC90
    @L_801BBC70:
    lwz r3, 0x6c(r31)
    slwi r0, r30, 2
    lwzx r3, r3, r0
    cmplwi r3, 0x0
    beq @L_801BBC8C
    li r4, 0x10
    bl fn_80193AF0
    @L_801BBC8C:
    addi r30, r30, 0x1
    @L_801BBC90:
    lwz r3, 0x6c(r31)
    slwi r0, r30, 2
    lwzx r0, r3, r0
    cmplwi r0, 0x0
    bne @L_801BBC70
    lwz r3, 0x6c(r31)
    bl fn_801A6960
    @L_801BBCAC:
    lis r4, lbl_8036D3F0@ha
    mr r3, r31
    addi r4, r4, lbl_8036D3F0@l
    lwz r4, 0x14(r4)
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
