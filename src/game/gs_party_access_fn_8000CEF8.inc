    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    cmpwi r3, 0x111
    beq @L_8000CF30
    bge @L_8000CF44
    cmpwi r3, 0x110
    bge @L_8000CF1C
    b @L_8000CF44
    @L_8000CF1C:
    li r3, 0x1
    li r4, 0x0
    li r5, 0x0
    bl fn_8001BDF4
    b @L_8000CF54
    @L_8000CF30:
    li r3, 0x2
    li r4, 0x0
    li r5, 0x0
    bl fn_8001BDF4
    b @L_8000CF54
    @L_8000CF44:
    li r3, 0x2
    li r4, 0x0
    li r5, 0x0
    bl fn_8001BDF4
    @L_8000CF54:
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
