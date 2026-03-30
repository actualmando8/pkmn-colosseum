    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r3, lbl_8047A610(r13)
    addi r3, r3, 0x144
    bl fn_8010A420
    li r3, 0xd8
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lwz r3, lbl_8047A610(r13)
    bl fn_800E202C
    mr r31, r3
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x0
    beq @L_800756AC
    bl fn_800E24B0
    mr r3, r31
    bl fn_800E209C
@L_800756AC:
    li r0, 0x0
    stw r0, lbl_8047A610(r13)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
