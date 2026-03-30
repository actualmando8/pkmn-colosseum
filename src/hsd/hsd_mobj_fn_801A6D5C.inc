    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    beq @L_801A6D90
    cmplwi r4, 0x0
    beq @L_801A6D90
    cmplwi r5, 0x0
    bne @L_801A6D84
    b @L_801A6D90
    @L_801A6D84:
    mr r3, r4
    mr r4, r5
    bl fn_801BBE3C
    @L_801A6D90:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
