    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    beq @L_801386C4
    lhz r31, 0x0(r3)
    bl fn_800B8DF4
    bl fn_800B856C
    cmplwi r31, 0x0
    beq @L_801386BC
    mr r3, r31
    bl fn_800E24B0
    mr r3, r31
    bl fn_800E209C
    @L_801386BC:
    li r3, 0x1
    b @L_801386C8
    @L_801386C4:
    li r3, 0x0
    @L_801386C8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
