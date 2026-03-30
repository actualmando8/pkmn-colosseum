    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x14(r1)
    li r5, 0x12
    li r6, 0x0
    bl fn_801F54A4
    cmplwi r3, 0x0
    beq @L_8020B718
    li r3, 0x1
    li r4, 0x3e8
    li r5, 0x3e8
    li r6, 0xff
    bl fn_8016597C
    li r3, 0x3c
    bl fn_801F000C
@L_8020B718:
    lwz r0, 0x14(r1)
    li r3, 0x1
    mtlr r0
    addi r1, r1, 0x10
    blr
