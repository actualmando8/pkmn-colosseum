    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lbz r0, -21548(r13)
    cmplwi r0, 0x0
    beq @L_801D2D14
    lwz r3, -21552(r13)
    cmplwi r3, 0x0
    bne @L_801D2D14
    lwz r0, -21556(r13)
    cmplwi r0, 0x0
    beq @L_801D2D08
    cmplwi r3, 0x0
    bne @L_801D2CC0
    li r3, 0x8
    bl fn_80177A44
    b @L_801D2CE4
@L_801D2CC0:
    lwz r0, 0x18(r3)
    cmplwi r0, 0x0
    beq @L_801D2CDC
    lwz r0, 0x20(r3)
    cmplwi r0, 0x0
    beq @L_801D2CDC
    bl fn_80176AE4
@L_801D2CDC:
    li r0, 0x0
    stw r0, -21552(r13)
@L_801D2CE4:
    li r3, 0x0
    bl fn_801765F4
    li r0, 0x0
    stw r0, -21556(r13)
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D2D08
    li r3, 0x2
    bl fn_80177A44
@L_801D2D08:
    mr r3, r31
    li r4, 0x0
    bl fn_801D30BC
@L_801D2D14:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
