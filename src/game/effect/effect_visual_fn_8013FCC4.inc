    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_8013FD50
    lwz r0, 0x4(r31)
    cmplwi r0, 0x0
    beq @L_8013FD04
    lwz r4, 0x8(r31)
    cmplwi r4, 0x0
    beq @L_8013FD04
    lwz r3, 0x0(r31)
    bl fn_800F9318
    stw r3, 0x10(r31)
    b @L_8013FD0C
    @L_8013FD04:
    li r0, 0x0
    stw r0, 0x10(r31)
    @L_8013FD0C:
    lwz r4, 0xc(r31)
    cmplwi r4, 0x0
    beq @L_8013FD40
    lwz r3, 0x0(r31)
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_8013FD34
    bl fn_801195AC
    stw r3, 0x14(r31)
    b @L_8013FD48
    @L_8013FD34:
    li r0, 0x0
    stw r0, 0x14(r31)
    b @L_8013FD48
    @L_8013FD40:
    li r0, 0x0
    stw r0, 0x14(r31)
    @L_8013FD48:
    li r3, 0x1
    b @L_8013FD54
    @L_8013FD50:
    li r3, 0x0
    @L_8013FD54:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
