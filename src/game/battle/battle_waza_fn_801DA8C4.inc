    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    beq @L_801DA900
    bl fn_801DD0C8
    mr. r31, r3
    beq @L_801DA900
    lbz r0, 0x14(r31)
    cmplwi r0, 0x0
    beq @L_801DA8F8
    bl fn_801DBB10
@L_801DA8F8:
    mr r3, r31
    bl fn_801DBDDC
@L_801DA900:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
