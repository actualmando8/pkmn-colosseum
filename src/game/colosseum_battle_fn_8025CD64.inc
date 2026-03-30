    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r3, -20944(r13)
    cmplwi r3, 0x0
    beq @L_8025CDA4
    bl fn_800E202C
    rlwinm. r0, r3, 0, 16, 31
    mr r31, r3
    beq @L_8025CD9C
    bl fn_800E24B0
    mr r3, r31
    bl fn_800E209C
@L_8025CD9C:
    li r0, 0x0
    stw r0, -20944(r13)
@L_8025CDA4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
