    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    beq @L_8013B084
    lwz r3, 0x50(r3)
    b @L_8013B074
    @L_8013B058:
    lhz r30, 0xc(r3)
    lwz r31, 0x10(r3)
    mr r3, r30
    bl fn_800E24B0
    mr r3, r30
    bl fn_800E209C
    mr r3, r31
    @L_8013B074:
    cmplwi r3, 0x0
    bne @L_8013B058
    li r3, 0x1
    b @L_8013B088
    @L_8013B084:
    li r3, 0x0
    @L_8013B088:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
