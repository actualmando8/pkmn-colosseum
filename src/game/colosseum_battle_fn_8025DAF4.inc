    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_8006B5A8
    lwz r4, 0x18(r3)
    cmplwi r4, 0x0
    beq @L_8025DB18
    subi r0, r4, 0x1
    stw r0, 0x18(r3)
@L_8025DB18:
    lwz r0, 0x14(r1)
    lwz r3, 0x18(r3)
    mtlr r0
    addi r1, r1, 0x10
    blr
