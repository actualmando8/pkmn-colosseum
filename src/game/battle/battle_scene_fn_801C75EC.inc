    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_800E202C
    mr r31, r3
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x0
    beq @L_801C761C
    bl fn_800E24B0
    mr r3, r31
    bl fn_800E209C
@L_801C761C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
