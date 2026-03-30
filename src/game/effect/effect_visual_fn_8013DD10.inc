    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_8013DD64
    lwz r0, 0x10(r31)
    cmpwi r0, 0x0
    beq @L_8013DD64
    li r3, 0x0
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    li r7, 0x0
    bl fn_800EF5FC
    stw r3, 0xc(r31)
    lwz r0, 0xc(r31)
    cmplwi r0, 0x0
    bne @L_8013DD64
    li r3, 0x0
    b @L_8013DD68
    @L_8013DD64:
    li r3, 0x1
    @L_8013DD68:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
