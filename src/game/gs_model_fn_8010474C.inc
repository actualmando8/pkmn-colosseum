    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r3, 0x4(r31)
    bl fn_8005DA18
    mr r4, r3
    lwz r0, 0x14(r4)
    cmplwi r0, 0x0
    beq @L_80104790
    li r0, 0x5
    mr r3, r31
    stb r0, 0x1(r31)
    lwz r12, 0x14(r4)
    mtctr r12
    bctrl
@L_80104790:
    cmplwi r31, 0x0
    beq @L_80104814
    lwz r3, 0x14(r31)
    cmplwi r3, 0x0
    bne @L_801047B8
    lis r3, lbl_80404ACC@ha
    lwz r0, 0x10(r31)
    addi r3, r3, lbl_80404ACC@l
    stw r0, 0xc(r3)
    b @L_801047C0
@L_801047B8:
    lwz r0, 0x10(r31)
    stw r0, 0x10(r3)
@L_801047C0:
    lwz r3, 0x10(r31)
    cmplwi r3, 0x0
    beq @L_801047D4
    lwz r0, 0x14(r31)
    stw r0, 0x14(r3)
@L_801047D4:
    addi r3, r31, 0x1c
    bl fn_8010925C
    addi r3, r31, 0x20
    bl fn_8010925C
    lhz r3, 0xac(r31)
    cmplwi r3, 0x0
    beq @L_80104808
    bl fn_800E24B0
    lhz r3, 0xac(r31)
    bl fn_800E209C
    li r0, 0x0
    stw r0, 0xb0(r31)
    sth r0, 0xac(r31)
@L_80104808:
    li r0, 0x0
    stb r0, 0x0(r31)
    stw r0, 0x4(r31)
@L_80104814:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
