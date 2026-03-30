    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    addis r0, r3, 0x1
    li r31, 0x0
    cmplwi r0, 0xffff
    beq @L_801123BC
    bl fn_80115C48
    bl fn_80115AC8
    clrlwi r0, r3, 24
    cmpwi r0, 0x2
    beq @L_801123B8
    b @L_801123BC
@L_801123B8:
    li r31, 0x1
@L_801123BC:
    mr r3, r31
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
